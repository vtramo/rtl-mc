#include <ppl_output.h>
#include <ppl_utils.h>
#include <spot/tl/parse.hh>
#include <spot/tl/print.hh>
#include <spot/twaalgos/hoa.hh>
#include "RtlfParsingResult.h"
#include "BackwardNFA.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "parsertlf.h"
#include "PolyhedralSystem.h"
#include "discretization.h"
#include "ppl_aliases.h"
#include "reach.h"
#include "Denot.h"
#include <argparse/argparse.hpp>

using PPL::IO_Operators::operator<<;

int main(const int argc, char *argv[])
{
    argparse::ArgumentParser rtlMcProgram { "rtl-mc" };

    std::string polyhedralSystemFilename {};
    rtlMcProgram.add_argument("system")
        .help("Polyhedral System file")
        .store_into(polyhedralSystemFilename);

    std::string rtlfFilename {};
    rtlMcProgram.add_argument("formula")
        .help("RTLf file")
        .store_into(rtlfFilename);

    auto& automatonOptimizationGroup { rtlMcProgram.add_mutually_exclusive_group() };
    bool low {};
    bool medium {};
    bool high {};
    automatonOptimizationGroup.add_argument("--low")
        .help("minimal optimizations (fast)")
        .store_into(low);
    automatonOptimizationGroup.add_argument("--medium")
        .help("moderate optimizations")
        .store_into(medium);
    automatonOptimizationGroup.add_argument("--high")
        .help("all available optimizations (slow, default)")
        .store_into(high);

    bool anyOption {};
    rtlMcProgram.add_argument("--any")
        .help("tells the translator that it should attempt to "
              "reduce or produce a deterministic result: any automaton denoting the "
              "given formula is OK.  This effectively disables post-processings and "
              "speeds up the translation")
        .flag()
        .store_into(anyOption);

    try
    {
        rtlMcProgram.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << rtlMcProgram;
        exit(1);
    }

    std::ifstream polyhedralSystemFile { polyhedralSystemFilename };
    PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem(polyhedralSystemFile) };
    if (!polyhedralSystemParsingResult.ok())
    {
        std::cerr << "Failed to parse polyhedral system.\n";
        exit(1);
    }
    PolyhedralSystemSharedPtr polyhedralSystem { std::make_shared<PolyhedralSystem>(std::move(*polyhedralSystemParsingResult)) };
    polyhedralSystemFile.close();
    polyhedralSystem->setConstraintOutputMinimized(false);
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    std::cout << *polyhedralSystem << '\n';

    std::ifstream rtlfFile { rtlfFilename };
    std::stringstream buffer {};
    buffer << rtlfFile.rdbuf();
    std::string rtlf { buffer.str() };
    rtlfFile.close();

    RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf, polyhedralSystem->getAtoms() ) };
    if (!rtlfParsingResult.ok())
    {
        std::cerr << "Failed to parse rtlf file.\n";
        exit(1);
    }

    std::cout << "Formula: " << *rtlfParsingResult << '\n';
    DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretizeToLtl(std::move(*rtlfParsingResult)) };
    try
    {
        spot::postprocessor::optimization_level optimizationLevel {};
        if (low) optimizationLevel = spot::postprocessor::optimization_level::Low;
        if (medium) optimizationLevel = spot::postprocessor::optimization_level::Medium;
        if (high) optimizationLevel = spot::postprocessor::optimization_level::High;
        BackwardNFA backwardNfa {
            std::move(discreteLtlFormula),
            std::move(polyhedralSystemFormulaDenotationMap),
            optimizationLevel,
            anyOption
        };

        // std::cout << backwardNfa << '\n';
        std::cout << "Total states: " << backwardNfa.totalStates() << '\n';
        std::cout << "Total edges: " << backwardNfa.totalEdges() << '\n';
        std::cout << "Total final states: " << backwardNfa.totalFinalStates() << '\n';
        // backwardNfa.printDotFormat(std::cout);

        assert(polyhedralSystem->getSpaceDimension() == polyhedralSystem->getPreFlow().space_dimension());
        Denot denot { polyhedralSystem, backwardNfa };
        Powerset result { denot.run() };
        std::cout << "RESULT " << PPLOutput::toString(result, polyhedralSystem->getSymbolTable()) << '\n';
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
