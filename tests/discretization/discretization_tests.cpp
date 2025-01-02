#include <test_utils.h>
#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>
#include <spot/twaalgos/contains.hh>

#include "discretization.h"
#include "spot_utils.h"

using namespace SpotUtils;

TEST_CASE("Discretization tests", "[discretization]")
{
    SECTION("F p0")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty( // <-- It forces the alternation between singular and open intervals and F(last & sing) and alive property.
                F(And({ ap("p0"), alive() }))
            )
        };

        spot::formula formula { spot::parse_infix_psl("F p0").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                And({
                    F(And({ ap("p0"), alive() })),
                    F(And({ ap("p1"), alive() }))
                })
            )
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1 using spot parser as oracle")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "F(p0 & alive) & "
                    "F(p1 & alive)"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1 using spot parser as oracle (2)")
    {
        spot::formula expectedFormula {
            spot::parse_infix_psl(
                "F(p0 & alive) &"
                "F(p1 & alive) &"
                "sing & alive & (alive U G!alive) &"
                "G((sing <-> X(!sing & alive)) | (alive & XG!alive) | !alive) & F((alive & XG!alive) & sing & alive)"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(spot::parse_infix_psl("G(p0 | !alive)").f)
        };

        spot::formula formula { spot::parse_infix_psl("G p0").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0 & F p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "G(p0 | !alive) &"
                    "F(p1 & alive)"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("G p0 & F p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "G((p0 & p1) | !alive) & p2 &"
                    "G(p3 | !alive) &"
                    "F((p4 & F(p5 & alive)) & alive)"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 U p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl("p0 U (p1 & (p0 | sing) & alive))").f
            )
        };

        spot::formula formula { spot::parse_infix_psl("p0 U p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 S p1") // This requires our modified version of Spot (strict until operator)
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "(sing & X((p0 U (p1 & (p0 | sing) & alive)) & alive)) | "
                    "(!sing & (p0 U (p1 & (p0 | sing) & alive)))"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("p0 S p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 R p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(spot::parse_infix_psl("(p0 R (p1 | (p0 & !sing) | !alive))").f)
        };

        spot::formula formula { spot::parse_infix_psl("p0 R p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 M p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "(p0 R (p1 | (p0 & !sing) | !alive)) &"
                    "F(p0 & p1 & alive)"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("p0 M p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 W p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastAndAliveProperty(
                spot::parse_infix_psl(
                    "(p0 U (p1 & (p0 | sing) & alive)) |"
                    "G(p0 | !alive)"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("p0 W p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

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