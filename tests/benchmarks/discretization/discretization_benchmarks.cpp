#include "discretization.h"
#include "test_utils.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spot/tl/parse.hh>

TEST_CASE("Discretization benchmarks", "[benchmark][discretization]")
{
    std::string formulaStr { readTestFileAsString("discretization-benchmark-1.txt") };
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };

    BENCHMARK("RTLf to LTL (our version)")
    {
        spot::formula formula { parsedFormula.f };
        REQUIRE(parsedFormula.errors.empty());
        DiscreteLtlFormula::discretizeToLtl(std::move(formula));
    };

    BENCHMARK("RTLf to LTL (spot)")
    {
        spot::formula formula { parsedFormula.f };
        REQUIRE(parsedFormula.errors.empty());
        DiscreteFiniteLtlFormula discreteFormula { DiscreteFiniteLtlFormula::discretize(std::move(formula)) };
        DiscreteLtlFormula __ { discreteFormula.toLtl() };
    };
}