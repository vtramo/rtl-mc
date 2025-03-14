#include "test_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spot/tl/parse.hh>
#include "discretisation.h"
#include "PolyhedralSystem.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "BackwardNFA.h"

TEST_CASE("Automaton construction benchmarks", "[benchmark][automaton]")
{
    std::string formulaStr { readTestFileAsString("automaton-formula-benchmark-1.txt") };
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };
    DiscreteLtlFormula discretisedLtlFormula { DiscreteLtlFormula::discretiseRtlFinite(std::move(parsedFormula.f)) };
    REQUIRE(parsedFormula.errors.empty());

    std::istringstream polyhedralSystemInput { readTestFileAsString("automaton-polyhedral-system-benchmark-1.txt") };
    PolyhedralSystem polyhedralSystem {};
    polyhedralSystemInput >> polyhedralSystem;
    PolyhedralSystemSharedPtr polyhedralSystemPtr { std::make_shared<PolyhedralSystem>(std::move(polyhedralSystem)) };

    BENCHMARK("Automaton construction benchmark 1")
    {
        PolyhedralSystemFormulaDenotationMap PolyhedralSystemLabelDenotationMap { polyhedralSystemPtr };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(discretisedLtlFormula, std::move(PolyhedralSystemLabelDenotationMap)) };
    };
}