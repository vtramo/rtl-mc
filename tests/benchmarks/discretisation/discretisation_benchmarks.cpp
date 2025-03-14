#include "discretisation.h"
#include "test_utils.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spot/tl/parse.hh>

TEST_CASE("Discretisation benchmarks", "[benchmark][discretisation]")
{
    std::string formulaStr { readTestFileAsString("discretisation-benchmark-1.txt") };
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };

    BENCHMARK("RTLf to LTL (our version)")
    {
        REQUIRE(parsedFormula.errors.empty());
        spot::formula formulaFiniteLtl { parsedFormula.f };
        DiscreteLtlFormula::discretiseRtlFinite(std::move(formulaFiniteLtl));
    };

    BENCHMARK("RTLf to LTL (spot)")
    {
        REQUIRE(parsedFormula.errors.empty());
        spot::formula formulaFiniteLtl { parsedFormula.f };
        DiscreteFiniteLtlFormula discreteFormula { DiscreteFiniteLtlFormula::discretiseRtlFinite(std::move(formulaFiniteLtl)) };
        DiscreteLtlFormula __ { discreteFormula.toLtl() };
    };
}