#include "test_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spot/tl/parse.hh>
#include "discretization.h"
#include "PolyhedralSystem.h"
#include "LabelDenotationMap.h"
#include "BackwardNFA.h"

TEST_CASE("Automaton construction benchmarks", "[benchmark][automaton]")
{
    std::string formulaStr { readTestFileAsString("automaton-formula-benchmark-1.txt") };
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };
    DiscreteLtlFormula discretizedLtlFormula { discretize(std::move(parsedFormula.f)) };
    REQUIRE(parsedFormula.errors.empty());

    std::istringstream polyhedralSystemInput { readTestFileAsString("automaton-polyhedral-system-benchmark-1.txt") };
    PolyhedralSystem polyhedralSystem {};
    polyhedralSystemInput >> std::move(polyhedralSystem);
    polyhedralSystemInput >> polyhedralSystem;

    BENCHMARK("Automaton construction benchmark 1")
    {
        LabelDenotationMap labelDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discretizedLtlFormula, labelDenotationMap };
    };
}