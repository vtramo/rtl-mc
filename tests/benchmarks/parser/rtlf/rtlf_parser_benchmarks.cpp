#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "test_utils.h"
#include "parsertlf.h"
#include "RtlfParsingResult.h"

TEST_CASE("RTLf Parser benchmarks")
{
    std::string rtlf { readTestFileAsString("rtlf-parser-benchmark-1.txt") };

    BENCHMARK("RTLf Parser benchmark 1")
    {
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };
        REQUIRE(rtlfParsingResult.ok());
    };
}
