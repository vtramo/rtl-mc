#include <argparse/argparse.hpp>
#include <ppl_output.h>
#include <spot/tl/parse.hh>
#include "BackwardNFA.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystem.h"
#include "ppl_aliases.h"
#include "Denot.h"
#include "RtlMcProgram.h"

using PPL::IO_Operators::operator<<;

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    RtlMcProgram::AutomatonOptimizationFlags optimizationFlags
);

int main(const int argc, char *argv[])
{
    RtlMcProgram rtlMcProgram { "rtl-mc", argc, argv };
    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula { rtlMcProgram.rtlFormula() };

    std::cout << "POLYHEDRAL SYSTEM: \n";
    polyhedralSystem->setConstraintOutputMinimized(false);
    std::cout << *polyhedralSystem << "\n\n";
    std::cout << "RTLf Formula: " << rtlFormula << "\n\n";

    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretizeToLtl(std::move(rtlFormula)) };

    try
    {
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        std::cout << "Total states: " << backwardNfa.totalStates() << '\n';
        std::cout << "Total edges: " << backwardNfa.totalEdges() << '\n';
        std::cout << "Total final states: " << backwardNfa.totalFinalStates() << '\n';

        Denot denot { polyhedralSystem, backwardNfa };
        Powerset result { denot.run() };
        std::cout << "Denot total iterations: " << denot.totalIterations() << '\n';
        std::cout << "RESULT " << PPLOutput::toString(result, polyhedralSystem->getSymbolTable()) << '\n';
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
