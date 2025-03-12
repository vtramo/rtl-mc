#include <spdlog/spdlog.h>

#include "interface.h"
#include "Timer.h"
#include "logger.h"
#include "Verbosity.h"
#include "stats_collectors.h"
#include "StatsFormatter.h"
#include "spot_utils.h"
#include "DenotOnTheFly.h"
#include "ppl_aliases.h"
#include "ppl_utils.h"
#include "DenotConcurrentV1.h"
#include "DiscreteFiniteLtlFormula.h"
#include "ppl_output.h"
#include "automata_builder.h"

int onlyOnTheFlyDenotInterface(const int argc, char *argv[])
{
    Log::configureLogger();
    RtlMcProgram rtlMcProgram { argc, argv };
    Log::setVerbosityLevel(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    Log::log(Verbosity::verbose, "[Polyhedral System]\n{}", *polyhedralSystem);
    PolyhedralSystemStats polyhedralSystemStats { collectPolyhedralSystemStats(*polyhedralSystem) };

    Log::log(Verbosity::verbose, "[RTL Formula] Formula: {}.", rtlFormula);
    RtlFormulaStats rtlFormulaStats { collectRtlStats(rtlFormula) };
    Log::log(Verbosity::verbose, "[RTL Formula] Total atomic propositions: {}.", rtlFormulaStats.totalAtomicPropositions);
    Log::log(Verbosity::verbose, "[RTL Formula] Length: {}.\n", rtlFormulaStats.length);

    Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
    Timer timer {};

    DiscreteLtlFormula discreteLtlFormula {
        rtlMcProgram.directLtl()
            ? DiscreteLtlFormula::discretiseFromFiniteLtl(std::move(rtlFormula))
            : DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl()
    };

    const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
    DiscretisationStats discretisationStats { collectDiscretisationStats(discreteLtlFormula, discretisationExecutionTimeSeconds) };
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", discreteLtlFormula);
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", discretisationStats.discreteLtlFormulaTotalAtomicPropositions);
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", discretisationStats.discreteLtlFormulaLength);

    try
    {
        Log::log(Verbosity::verbose, ">>> BackwardNFA automaton construction started.");
        timer.reset();

        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFAConstSharedPtr backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        Log::log(Verbosity::verbose, "<<< BackwardNFA automaton construction completed. Elapsed time: {} s.\n", timer.elapsedInSeconds());
        Log::log(Verbosity::debug, "[BackwardNFA]\n{}\n", *backwardNfa);

        Log::log(Verbosity::verbose, ">>> Denot algorithm started.");
        timer.reset();

        std::unique_ptr denotUniquePtr { createDenot(rtlMcProgram, polyhedralSystem, backwardNfa) };
        Denot& denot { *denotUniquePtr };

        PowersetUniquePtr denotResult {
            rtlMcProgram.universal()
                ? PPLUtils::minus(polyhedralSystem->invariant(), *denot())
                : denot()
        };

        const double denotExecutionTimeSeconds { timer.elapsedInSeconds() };
        DenotStats denotStats { collectDenotStats(denot, denotExecutionTimeSeconds) };
        Log::log(Verbosity::verbose, "<<< Denot algorithm terminated. Elapsed time: {} s.", denotStats.executionTimeSeconds);
        Log::log(Verbosity::verbose, "<<< Denot algorithm total iterations: {}.\n", denotStats.totalIterations);

        Log::log(Verbosity::verbose, "[Result]");

        switch (rtlMcProgram.outputFormat())
        {
        case OutputFormat::normal:
            if (rtlMcProgram.modelChecking())
            {
                Poly point { rtlMcProgram.modelCheckingPoint() };
                std::cout << std::boolalpha << denotResult->contains(Powerset { point }) << std::noboolalpha << '\n';
            }
            else
            {
                std::cout << PPLOutput::toString(*denotResult, polyhedralSystem->symbolTable()) << '\n';
            }
            break;
        case OutputFormat::quiet:
            break;
        case OutputFormat::stats:
            StatsFormatter statsFormatter {
                std::move(polyhedralSystemStats),
                std::move(rtlFormulaStats),
                std::move(discretisationStats),
                AutomatonStats { backwardNfa->stats() },
                std::move(denotStats)
            };
            std::cout << statsFormatter(rtlMcProgram.statsFormat()) << '\n';
            break;
        }

        Log::log(Verbosity::verbose, "[Result] Existential Denotation: {}.", rtlMcProgram.existential());
        Log::log(Verbosity::verbose, "[Result] Universal Denotation: {}.", rtlMcProgram.universal());
        Log::log(Verbosity::verbose, "[Result] Model-checking problem: {}.", rtlMcProgram.modelChecking());
    }
    catch (const std::exception& e)
    {
        spdlog::error(e.what());
        exit(1);
    }

    return 0;
}

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    BackwardNFAConstSharedPtr backwardNfa
) {
    if (rtlMcProgram.concurrent()) {
        return std::make_unique<DenotConcurrentV1>(polyhedralSystem, backwardNfa);
    }

    return std::make_unique<DenotOnTheFly>(polyhedralSystem, backwardNfa);
}