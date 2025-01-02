#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "PolyhedralSystem.h"
#include "test_utils.h"

TEST_CASE("PolyhedralSystem Parser benchmarks")
{
    std::istringstream input { readTestFileAsString("polyhedral-system-parser-benchmark-1.txt") };

    BENCHMARK("PolyhedralSystem Parser benchmark 1")
    {
        PolyhedralSystem polyhedralSystem {};
        input >> std::move(polyhedralSystem);
    };
}
