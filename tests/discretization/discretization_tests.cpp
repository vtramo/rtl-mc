#include <test_utils.h>
#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>
#include <spot/twaalgos/contains.hh>

#include "discretization.h"
#include "spot_utils.h"

TEST_CASE("Discretization tests", "[discretization]")
{
    SECTION("F p0")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastProperty( // <-- It forces the alternation between singular and open intervals and F(last & sing).
                U(top(), And({
                                ap("p0"),
                                Or({ top(), sing() }), // <-- automatically simplified by Spot
                                alive()
                         }))
            )
        };

        spot::formula formula { spot::parse_infix_psl("F p0").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("F p0 & F p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastProperty(
                And({
                    U(top(), And({ ap("p0"), alive() })),
                    U(top(), And({ ap("p1"), alive() })),
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
            imposeSingOpenLastProperty(
                spot::parse_infix_psl(
                    "(true U (p0 & alive)) & "
                    "(true U (p1 & alive))"
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
                "(true U (p0 & alive)) & "
                "(true U (p1 & alive)) & "
                "sing & !(1 U (alive & !((alive & X!G!alive) | (sing <-> X!sing)))) & !(1 U (alive & !(1 U (alive & sing & X!G!alive))))"
            ).f
        };

        spot::formula formula { spot::parse_infix_psl("F p0 & F p1").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastProperty(spot::parse_infix_psl("!(true U (!p0 & sing & alive))").f)
        };

        spot::formula formula { spot::parse_infix_psl("G p0").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("G p0 & F p1")
    {
        spot::formula expectedFormula {
            imposeSingOpenLastProperty(
                spot::parse_infix_psl(
                    "!(true U (!p0 & sing & alive)) &"
                    "(true U (p1 & alive))"
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
            imposeSingOpenLastProperty(
                spot::parse_infix_psl(
                    "!(true U (!(p0 & p1) & sing & alive)) & p2 &"
                    "!(true U (!p3 & sing & alive)) &"
                    "(true U ((p4 & (true U (p5 & alive))) & alive))"
                ).f
            )
        };

        spot::formula formula { spot::parse_infix_psl("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)").f };
        spot::formula discretizedFormula { discretize(std::move(formula)) };

        REQUIRE(discretizedFormula == expectedFormula);
    }

    SECTION("p0 S p1") // This requires our modified version of Spot (strict until operator)
    {
        spot::formula expectedFormula {
            imposeSingOpenLastProperty(
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

    SECTION("Discretize large RTLf formula from discretization-test-1.txt")
    {
        std::string formulaStr { readTestFileAsString("discretization-test-1.txt") };
        spot::parsed_formula parsedFormula { spot::parse_infix_psl(formulaStr) };
        REQUIRE(parsedFormula.errors.empty());

        discretize(std::move(parsedFormula.f));
    }
}