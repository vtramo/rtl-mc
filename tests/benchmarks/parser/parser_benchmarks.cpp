#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "PolyhedralSystem.h"
#include "test_utils.h"

TEST_CASE("Parser benchmarks", "[benchmark][parser]")
{
    std::istringstream input { readTestFileAsString("polyhedral-system-benchmark-1.txt") };

    BENCHMARK("Parser benchmark 1")
    {
        PolyhedralSystem polyhedralSystem {};
        input >> std::move(polyhedralSystem);
    };
}
