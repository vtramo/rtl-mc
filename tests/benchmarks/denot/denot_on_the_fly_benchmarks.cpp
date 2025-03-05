#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <spdlog/spdlog.h>

#include "spot_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "BackwardNFA.h"
#include "DenotOnTheFly.h"
#include "Denot.h"

using namespace SpotUtils;

TEST_CASE("Denot on the fly benchmark GAP")
{
    PolyhedralSystemConstSharedPtr polyhedralSystem {
        std::make_shared<PolyhedralSystem>(
            std::move(
                *parsePolyhedralSystem(
                    "Inv ( { a >= 0 & b >= 0 } )"
                    "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                    "p { a >= b + 1 }"
                    "q { b >= a + 1 }"
                    "t0 { t = 0 }"
                    "t1 { t <= 100 }"
                )
            )
        )
    };

    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(50) })).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    BackwardNFAConstSharedPtr backwardNfa {
        BackwardNFA::buildAutomaton(
            std::move(discreteLtlFormula),
            std::move(polyhedralSystemFormulaDenotationMap)
        )
    };
    DenotOnTheFly denot { polyhedralSystem, backwardNfa };

    BENCHMARK("k=50")
    {
        auto _ = denot();
    };
}