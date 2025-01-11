#include <test_utils.h>
#include <catch2/catch_test_macros.hpp>
#include <spot/tl/ltlf.hh>
#include <spot/tl/parse.hh>

#include "discretization.h"
#include "spot_utils.h"

using namespace SpotUtils;

TEST_CASE("Discretize to LTL from RTLf", "[discretization-ltl]")
{
    SECTION("X p0")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "((sing & X[!](p0)) | (!sing & p0)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("X p0").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "F p0 &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("F p0").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "F p0 &"
                "F p1 &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "G p0 &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("G p0").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0 & F p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "G p0 &"
                "F p1 &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("G p0 & F p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "G(p0 & p1) & p2 &"
                "G p3 &"
                "F(p4 & F p5) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 U p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "p0 U (p1 & (p0 | sing)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("p0 U p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 R p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "(p0 R (p1 | (p0 & !sing))) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("p0 R p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 M p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "(p0 R (p1 | (p0 & !sing))) &"
                "F(p0 & p1) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("p0 M p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 W p1")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "((p0 U (p1 & (p0 | sing))) | G (p0)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("p0 W p1").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("(p0 W p1) & (p2 M p3) & (p4 R p5) & (p6 U p7) & G(p8) & F(p9) & X(p10)")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "((p0 U (p1 & (p0 | sing))) | G (p0)) &"
                "(p2 R (p3 | (p2 & !sing))) & F(p2 & p3) &"
                "(p4 R (p5 | (p4 & !sing))) &"
                "p6 U (p7 & (p6 | sing)) &"
                "G(p8) &"
                "F(p9) &"
                "((sing & X[!](p10)) | (!sing & p10)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("(p0 W p1) & (p2 M p3) & (p4 R p5) & (p6 U p7) & G(p8) & F(p9) & X(p10)").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G(a & b) & t & G(t1) & F(ab & F(ba))")
    {
        spot::formula expectedFormula {
            spot::from_ltlf(spot::parse_infix_psl(
                "G(a & b) & t & G(t1) & F(ab & F(ba)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f)
        };

        spot::formula formula { spot::parse_infix_psl("G(a & b) & t & G(t1) & F(ab & F(ba))").f };
        DiscreteLtlFormula discretizedFormula { discretizeToLtl(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("Discretize large RTLf formula from discretization-test-1.txt")
    {
        std::string formulaStr { readTestFileAsString("discretization-test-1.txt") };
        spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };
        REQUIRE(parsedFormula.errors.empty());

        discretizeToLtl(std::move(parsedFormula.f));
    }
}

TEST_CASE("Discretize to LTLf from RTLf", "[discretization-ltlf]")
{
    SECTION("X p0")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "((sing & X[!](p0)) | (!sing & p0)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("X p0").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "F(p0) & F(p1) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(!X[!] true & sing)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "G(p0) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!] true) &"
                "F(!X[!]true & sing)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("G p0").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 U p1")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "(p0 U (p1 & (p0 | sing))) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(!X[!]true & sing)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("p0 U p1").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 R p1")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "(p0 R (p1 | (p0 & !sing))) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("p0 R p1").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 M p1")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "(p0 R (p1 | (p0 & !sing))) &"
                "F(p0 & p1) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("p0 M p1").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 W p1")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "((p0 U (p1 & (p0 | sing))) | G (p0)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("p0 W p1").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("(p0 W p1) & (p2 M p3) & (p4 R p5) & (p6 U p7) & G(p8) & F(p9) & X(p10)")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "((p0 U (p1 & (p0 | sing))) | G (p0)) &"
                "(p2 R (p3 | (p2 & !sing))) & F(p2 & p3) &"
                "(p4 R (p5 | (p4 & !sing))) &"
                "p6 U (p7 & (p6 | sing)) &"
                "G(p8) &"
                "F(p9) &"
                "((sing & X[!](p10)) | (!sing & p10)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("(p0 W p1) & (p2 M p3) & (p4 R p5) & (p6 U p7) & G(p8) & F(p9) & X(p10)").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G(a & b) & t & G(t1) & F(ab & F(ba))")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "G(a & b) & t & G(t1) & F(ab & F(ba)) &"
                "sing &"
                "G((sing <-> X[!]!sing) | !X[!]true) &"
                "F(sing & !X[!]true)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("G(a & b) & t & G(t1) & F(ab & F(ba))").f };
        DiscreteFiniteLtlFormula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("Discretize large RTLf formula from discretization-test-1.txt")
    {
        std::string formulaStr { readTestFileAsString("discretization-test-1.txt") };
        spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };
        REQUIRE(parsedFormula.errors.empty());

        discretize(std::move(parsedFormula.f));
    }
}