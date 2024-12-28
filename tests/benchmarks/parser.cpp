#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "PolyhedralSystem.h"

std::ifstream openBenchmarkFile()
{
    std::ifstream input {};
    std::string testCaseFilePath { "polyhedral-system-benchmark-1.txt" };
    input.open(testCaseFilePath);
    REQUIRE(input);
    return input;
}

TEST_CASE("Parser benchmarks", "[benchmark][parser]")
{
    BENCHMARK("Parser benchmark 1")
    {
        std::ifstream input { openBenchmarkFile() };
        PolyhedralSystem polyhedralSystem {};
        input >> std::move(polyhedralSystem);
        REQUIRE(!input.fail());
        input.clear();
    };
}
