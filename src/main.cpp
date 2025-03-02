#include <spot/tl/parse.hh>
#include <spot/twaalgos/postproc.hh>
#include <spdlog/spdlog.h>

#include "discretisation/DiscreteFiniteLtlFormula.h"
#include "utils/ppl/ppl_output.h"
#include "automaton/BackwardNFA.h"
#include "automaton/PolyhedralSystemFormulaDenotationMap.h"
#include "discretisation/DiscreteLtlFormula.h"
#include "system/PolyhedralSystem.h"
#include "utils/ppl/ppl_aliases.h"
#include "utils/ppl/ppl_utils.h"
#include "core/Denot.h"
#include "cli/RtlMcProgram.h"
#include "utils/Timer.h"
#include "logger/logger.h"
#include "logger/Verbosity.h"
#include "stats/collectors.h"
#include "stats/StatsFormatter.h"
#include "utils/spot/spot_utils.h"
#include "DenotOnTheFly.h"

class DiscreteLtlFormula;
using PPL::IO_Operators::operator<<;
using namespace SpotUtils;

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
);

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    const BackwardNFA& backwardNfa
);

int main(const int argc, char *argv[])
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

    Log::log(Verbosity::verbose, "[RTLf Formula] Formula: {}.", rtlFormula);
    RtlFormulaStats rtlfFormulaStats { collectRtlfStats(rtlFormula) };
    Log::log(Verbosity::verbose, "[RTLf Formula] Total atomic propositions: {}.", rtlfFormulaStats.totalAtomicPropositions);
    Log::log(Verbosity::verbose, "[RTLf Formula] Length: {}.\n", rtlfFormulaStats.length);

    Log::log(Verbosity::verbose, ">>> RTLf formula discretisation started.");
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
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        Log::log(Verbosity::verbose, "<<< BackwardNFA automaton construction completed. Elapsed time: {} s.\n", timer.elapsedInSeconds());
        Log::log(Verbosity::debug, "[BackwardNFA]\n{}\n", backwardNfa);

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
                std::move(rtlfFormulaStats),
                std::move(discretisationStats),
                AutomatonStats { backwardNfa.stats() },
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
}

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel {};
    if (optimizationFlags.low) optimizationLevel = spot::postprocessor::optimization_level::Low;
    if (optimizationFlags.medium) optimizationLevel = spot::postprocessor::optimization_level::Medium;
    if (optimizationFlags.high) optimizationLevel = spot::postprocessor::optimization_level::High;
    return {
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    };
}

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    const BackwardNFA& backwardNfa
) {
    if (rtlMcProgram.concurrent()) {
        return std::make_unique<DenotConcurrentV1>(polyhedralSystem, backwardNfa);
    }

    return std::make_unique<DenotOnTheFly>(polyhedralSystem, backwardNfa);
}
