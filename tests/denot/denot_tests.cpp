#include <ppl_output.h>
#include <catch2/catch_test_macros.hpp>

#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "BackwardNFA.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralSystemParsingResult.h"
#include "Denot.h"

TEST_CASE("GAP Test Cases")
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
                    "t1 { t <= 10 }"
                )
            )
        )
    };
    const PolyhedralSystemSymbolTable& symbolTable { polyhedralSystem->getSymbolTable() };
    auto a { *symbolTable.getVariable("a") };
    auto b { *symbolTable.getVariable("b") };
    auto t { *symbolTable.getVariable("t") };

    SECTION("GAP k=1")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f).toLtl() };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -29 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -27 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=2")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p)))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -27 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -25 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=3")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -25 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -23 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=4")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p)))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -23 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -21 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=5")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -21 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -19 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=6")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -19 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -17 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=7")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -17 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -15 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=8")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -15 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -13 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=9")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q))))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b > -13 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b > -11 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("GAP k=10")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, a - b >= 1, -a + b >= -11 },
                { t == 0, b >= 0, -a + b > -1, a >= 0, a - b >= -9 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }
}

TEST_CASE("NO GAP Test Cases")
{
    PolyhedralSystemConstSharedPtr polyhedralSystem {
        std::make_shared<PolyhedralSystem>(
            std::move(
                *parsePolyhedralSystem(
                    "Inv ( { a >= 0 & b >= 0 } )"
                    "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                    "p { a > b }"
                    "q { b > a }"
                    "t0 { t = 0 }"
                    "t1 { t <= 10 }"
                )
            )
        )
    };
    const PolyhedralSystemSymbolTable& symbolTable { polyhedralSystem->getSymbolTable() };
    auto a { *symbolTable.getVariable("a") };
    auto b { *symbolTable.getVariable("b") };
    auto t { *symbolTable.getVariable("t") };


    SECTION("NO GAP k=1")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f).toLtl() };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=2")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p)))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=3")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=4")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p)))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=5")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=6")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=7")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=8")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=9")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q))))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }

    SECTION("NO GAP k=10")
    {
        DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretize(
            spot::parse_infix_psl(
                "t0 & G(t1) & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p & F(q & F(p)))))))))))"
            ).f).toLtl()
        };
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa { discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap) };
        Denot denot { polyhedralSystem, backwardNfa };

        Powerset expectedResult {
            PPLUtils::powerset({
                { t == 0, b >= 0, -a + b > -30, a - b > 0 },
                { t == 0, b >= 0, -a + b >= 0, a >= 0, a - b > -30 }
            })
        };

        Powerset result { denot() };
        REQUIRE(result == expectedResult);
    }
}