#include <DiscreteFiniteLtlFormula.h>
#include <argparse/argparse.hpp>
#include <ppl_output.h>
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
    RtlMcProgram rtlMcProgram { "rtl-mc", argc, argv };
    RtlMcProgram rtlMcProgram { argc, argv };
    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    const bool verbose { rtlMcProgram.verbose() };

    if (verbose)
    {
        std::cout << "POLYHEDRAL SYSTEM: \n";
        polyhedralSystem->setConstraintOutputMinimized(false);
        std::cout << *polyhedralSystem << "\n\n";
        std::cout << "RTLf Formula: " << rtlFormula << "\n\n";
    }

    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    DiscreteLtlFormula discreteLtlFormula {
        rtlMcProgram.directLtl()
            ? DiscreteLtlFormula::discretizeToLtl(std::move(rtlFormula))
            : DiscreteFiniteLtlFormula::discretize(std::move(rtlFormula)).toLtl()
    };

    if (verbose) std::cout << "DiscreteLtlFormula: " << discreteLtlFormula << "\n\n";

    try
    {
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        if (verbose)
        {
            std::cout << "BACKWARD NFA\n";
            std::cout << "Optimization level: " << SpotUtils::toOptimizationLevelString(backwardNfa.optimizationLevel()) << '\n';
            std::cout << "Total states: " << backwardNfa.totalStates() << '\n';
            std::cout << "Total edges: " << backwardNfa.totalEdges() << '\n';
            std::cout << "Initial initial states: " << backwardNfa.totalInitialStates() << '\n';
            std::cout << "Total final states: " << backwardNfa.totalFinalStates() << '\n';
        }

        if (verbose) std::cout << "\n\nStarting Denot algorithm..." << '\n';
        Denot denot { polyhedralSystem, backwardNfa };
        PowersetUniquePtr denotResult {
            rtlMcProgram.universal()
                ? PPLUtils::complement(*denot())
                : denot()
        };

        if (verbose)
        {
            std::cout << "Denot total iterations: " << denot.totalIterations() << '\n';
            std::cout << "RESULT ";
        }

        std::cout << PPLOutput::toString(result, polyhedralSystem->getSymbolTable()) << '\n';
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
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
