#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "test_utils.h"
#include "parsertl.h"
#include "RtlParsingResult.h"

TEST_CASE("RTLf Parser benchmarks")
{
    std::string rtlf { readTestFileAsString("rtlf-parser-benchmark-1.txt") };

    BENCHMARK("RTLf Parser benchmark 1")
    {
        RtlParsingResult rtlfParsingResult { parseRtl(rtlf) };
        REQUIRE(rtlfParsingResult.ok());
    };
}
