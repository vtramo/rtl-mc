#include <spot/tl/parse.hh>
#include <spot/tl/length.hh>
#include "discretization/DiscreteFiniteLtlFormula.h"
#include "utils/ppl/ppl_output.h"
#include "automaton/BackwardNFA.h"
#include "automaton/PolyhedralSystemFormulaDenotationMap.h"
#include "discretization/DiscreteLtlFormula.h"
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

class DiscreteLtlFormula;
using PPL::IO_Operators::operator<<;
using namespace SpotUtils;

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
);

int main(const int argc, char *argv[])
{
    RtlMcProgram rtlMcProgram { argc, argv };
    Logger::configureLogger(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    Logger::log(Verbosity::verbose, "[Polyhedral System]\n{}", *polyhedralSystem);
    PolyhedralSystemStats polyhedralSystemStats { collectPolyhedralSystemStats(*polyhedralSystem) };

    Logger::log(Verbosity::verbose, "[RTLf Formula] Formula: {}.", rtlFormula);
    RtlFormulaStats rtlfFormulaStats { collectRtlfStats(rtlFormula) };
    Logger::log(Verbosity::verbose, "[RTLf Formula] Total atomic propositions: {}.", rtlfFormulaStats.totalAtomicPropositions);
    Logger::log(Verbosity::verbose, "[RTLf Formula] Length: {}.\n", rtlfFormulaStats.length);

    Logger::log(Verbosity::verbose, ">>> RTLf formula discretization started.");
    Timer timer {};

    DiscreteLtlFormula discreteLtlFormula {
        rtlMcProgram.directLtl()
            ? DiscreteLtlFormula::discretizeToLtl(std::move(rtlFormula))
            : DiscreteFiniteLtlFormula::discretize(std::move(rtlFormula)).toLtl()
    };

    const double discretizationExecutionTimeSeconds { timer.elapsedInSeconds() };
    Logger::log(Verbosity::verbose, "<<< Discretization completed. Elapsed time: {} s.", discretizationExecutionTimeSeconds);
    DiscretizationStats discretizationStats { collectDiscretizationStats(discreteLtlFormula, discretizationExecutionTimeSeconds) };
    Logger::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", discreteLtlFormula);
    Logger::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", discretizationStats.discreteLtlFormulaTotalAtomicPropositions);
    Logger::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", discretizationStats.discreteLtlFormulaLength);

    try
    {
        Logger::log(Verbosity::verbose, ">>> BackwardNFA automaton construction started.");
        timer.reset();

        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        Logger::log(Verbosity::verbose, "<<< BackwardNFA automaton construction completed. Elapsed time: {} s.\n", timer.elapsedInSeconds());
        Logger::log(Verbosity::debug, "[BackwardNFA]\n{}\n", backwardNfa);

        Logger::log(Verbosity::verbose, ">>> Denot algorithm started.");
        timer.reset();

        Denot denot { polyhedralSystem, backwardNfa };
        PowersetUniquePtr denotResult {
            rtlMcProgram.universal()
                ? PPLUtils::minus(polyhedralSystem->getInvariant(), *denot())
                : denot()
        };

        const double denotExecutionTimeSeconds { timer.elapsedInSeconds() };
        DenotStats denotStats { collectDenotStats(denot, denotExecutionTimeSeconds) };
        Logger::log(Verbosity::verbose, "<<< Denot algorithm terminated. Elapsed time: {} s.", denotStats.executionTimeSeconds);
        Logger::log(Verbosity::verbose, "<<< Denot algorithm total iterations: {}.\n", denotStats.totalIterations);

        Logger::log(Verbosity::verbose, "[Result] Points: ");

        switch (rtlMcProgram.outputFormat())
        {
        case OutputFormat::normal:
            std::cout << PPLOutput::toString(*denotResult, polyhedralSystem->getSymbolTable()) << '\n';
            break;
        case OutputFormat::quiet:
            break;
        case OutputFormat::stats:
            StatsFormatter statsFormatter {
                std::move(polyhedralSystemStats),
                std::move(rtlfFormulaStats),
                std::move(discretizationStats),
                AutomatonStats { backwardNfa.stats() },
                std::move(denotStats)
            };
            std::cout << statsFormatter(rtlMcProgram.statsFormat()) << '\n';
            break;
        }

        Logger::log(Verbosity::verbose, "[Result] Existential Denotation: {}.", rtlMcProgram.existential());
        Logger::log(Verbosity::verbose, "[Result] Universal Denotation: {}.", rtlMcProgram.universal());
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