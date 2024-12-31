#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>

#include "discretization.h"
#include "spot_utils.h"

TEST_CASE("Discretization tests", "[discretization]")
{
    SECTION("F p0")
    {
        spot::formula formula { spot::parse_infix_psl("F p0").f };
        spot::formula expectedFormula {
            imposeSingOpenLastProperty(
                U(top(), And({
                                ap("p0"),
                                Or({ top(), sing() }),
                                alive()
                         }))
            )
        };

        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }
}