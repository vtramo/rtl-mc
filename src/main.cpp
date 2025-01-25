#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
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
#include "logging/setup.h"

class DiscreteLtlFormula;
using PPL::IO_Operators::operator<<;
using namespace SpotUtils;

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    RtlMcProgram::AutomatonOptimizationFlags optimizationFlags
);

int main(const int argc, char *argv[])
{
    RtlMcProgram rtlMcProgram { argc, argv };
    configureLogger(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    spdlog::info("[Polyhedral System]\n{}", *polyhedralSystem);
    spdlog::info("[RTLf Formula] Formula: {}.", rtlFormula);
    spdlog::info("[RTLf Formula] Total atomic propositions: {}.",
        std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(rtlFormula))->size());
    spdlog::info("[RTLf Formula] Length: {}.\n", spot::length(rtlFormula));

    spdlog::info(">>> RTLf formula discretization started.");
    Timer timer {};

    DiscreteLtlFormula discreteLtlFormula {
        rtlMcProgram.directLtl()
            ? DiscreteLtlFormula::discretizeToLtl(std::move(rtlFormula))
            : DiscreteFiniteLtlFormula::discretize(std::move(rtlFormula)).toLtl()
    };

    spdlog::info("<<< Discretization completed. Elapsed time: {} ms.", timer.elapsed());
    spdlog::info("[Discrete LTL formula] Formula: {}.", discreteLtlFormula);
    spdlog::info("[Discrete LTL formula] Total atomic propositions: {}.",
        std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(discreteLtlFormula.formula()))->size());
    spdlog::info("[Discrete LTL formula] Length: {}.\n", spot::length(discreteLtlFormula.formula()));

    try
    {
        spdlog::info(">>> BackwardNFA automaton construction started.");
        timer.reset();

        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        spdlog::info("<<< BackwardNFA automaton construction completed. Elapsed time: {} ms.\n", timer.elapsed());
        spdlog::trace("[BackwardNFA]\n{}\n", backwardNfa);

        spdlog::info(">>> Denot algorithm started.");
        timer.reset();

        Denot denot { polyhedralSystem, backwardNfa };
        PowersetUniquePtr denotResult {
            rtlMcProgram.universal()
                ? PPLUtils::complement(*denot())
                : denot()
        };

        spdlog::info("<<< Denot algorithm terminated. Elapsed time: {} ms.", timer.elapsed());
        spdlog::info("<<< Denot algorithm total iterations: {}.\n", denot.totalIterations());

        spdlog::info("[Result] Points: ");
        std::cout << PPLOutput::toString(*denotResult, polyhedralSystem->getSymbolTable()) << '\n';
        spdlog::info("[Result] Existential Denotation: {}.", rtlMcProgram.existential());
        spdlog::info("[Result] Universal Denotation: {}.", rtlMcProgram.universal());
    }
    catch (const std::exception& e)
    {
        spdlog::error(e.what());
    }
}

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    RtlMcProgram::AutomatonOptimizationFlags optimizationFlags
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
