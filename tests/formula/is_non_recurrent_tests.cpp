#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>
#include "formula.h"

TEST_CASE("Is non-recurrent LTL formula")
{
    SECTION("G(F(p0))")
    {
        spot::formula formula { spot::parse_infix_psl("G(F(p0))").f };
        REQUIRE(!isNonRecurrent(formula));
    }

    SECTION("false R (F(p))")
    {
        spot::formula formula { spot::parse_infix_psl("false R (F(p))").f };
        REQUIRE(!isNonRecurrent(formula));
    }

    SECTION("G(p0))")
    {
        spot::formula formula { spot::parse_infix_psl("G(p0))").f };
        REQUIRE(isNonRecurrent(formula));
    }

    SECTION("false R (p & q)")
    {
        spot::formula formula { spot::parse_infix_psl("false R (p & q)").f };
        REQUIRE(isNonRecurrent(formula));
    }

    SECTION("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(F(p2))))")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(F(p2))))").f };
        REQUIRE(!isNonRecurrent(formula));
    }

    SECTION("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(p2)))")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(p2)))").f };
        REQUIRE(isNonRecurrent(formula));
    }

    SECTION("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(p2)))")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & p1 & (p2 | p3) & F(p0 & F(p1 & G(p2)))").f };
        REQUIRE(isNonRecurrent(formula));
    }

    SECTION("(false R p0) & (p1 R (G(p2)))")
    {
        spot::formula formula { spot::parse_infix_psl("(false R p0) & (p1 R (G(p2)))").f };
        REQUIRE(!isNonRecurrent(formula));
    }
}
