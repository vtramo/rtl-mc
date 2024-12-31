#include "discretization.h"
#include "benchmark_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spot/tl/parse.hh>

TEST_CASE("Discretization benchmarks", "[benchmark][discretization]")
{
    std::string formulaStr { readBenchmarkFileAsString("discretization-benchmark-1.txt") };
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };

    BENCHMARK("Discretization benchmark 1")
    {
        spot::formula formula { parsedFormula.f };
        REQUIRE(parsedFormula.errors.empty());
        discretize(std::move(formula));
    };
}