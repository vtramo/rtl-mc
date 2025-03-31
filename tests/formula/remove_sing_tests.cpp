#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>
#include "formula.h"

TEST_CASE("Remove sing from formula")
{
    SECTION("(a & b) | (c & d)")
    {
        spot::formula formula { spot::parse_infix_psl("(a & b) | (c & d)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == formula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("sing")
    {
        spot::formula formula { spot::parse_infix_psl("sing").f };
        spot::formula expectedFormula { top() };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("!sing")
    {
        spot::formula formula { spot::parse_infix_psl("!sing").f };
        spot::formula expectedFormula { top() };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("sing & p0")
    {
        spot::formula formula { spot::parse_infix_psl("sing & p0").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("p0 & sing")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & sing").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("p0 & !sing")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & !sing").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("(p5 & (sing | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p5 & (sing | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p5 & p6)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & sing & p1 & p2) | (!sing & p3 & p4) | (p5 & (sing | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & sing & p1 & p2) | (!sing & p3 & p4) | (p5 & (sing | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2) | (p3 & p4) | (p5 & p6)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & sing & p1 & p2 & (p3 | p4 | sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & sing & p1 & p2 & (p3 | p4 | sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2 & (p3 | p4)) | ((p1 | p8 | p9) & p3 & p4) | (p5 & ((p9 & p7) | p6))").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & !sing & p1 & p2 & (p3 | p4 | !sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & !sing & p1 & p2 & (p3 | p4 | !sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2 & (p3 | p4)) | ((p1 | p8 | p9) & p3 & p4) | (p5 & ((p9 & p7) | p6))").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("p0")
    {
        spot::formula formula { spot::parse_infix_psl("p0").f };
        spot::formula expectedFormula { formula };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);

    }

    SECTION("!p0")
    {
        spot::formula formula { spot::parse_infix_psl("!p0").f };
        spot::formula expectedFormula { formula };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);

    }

    SECTION("(sing & !t0) | (sing & !t1) | (!p & sing) | (!q & sing)")
    {
        spot::formula formula { spot::parse_infix_psl("(sing & !t0) | (sing & !t1) | (!p & sing) | (!q & sing)").f };
        spot::formula expectedFormula { spot::parse_infix_psl("!t0 | !t1 | !p | !q").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("sing & !t1")
    {
        spot::formula formula { spot::parse_infix_psl("sing & !t1").f };
        spot::formula expectedFormula { spot::parse_infix_psl("!t1").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(!p & sing) | (!q & sing)")
    {
        spot::formula formula { spot::parse_infix_psl("(!p & sing) | (!q & sing)").f };
        spot::formula expectedFormula { spot::parse_infix_psl("!p | !q").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("!q & sing")
    {
        spot::formula formula { spot::parse_infix_psl("!q & sing").f };
        spot::formula expectedFormula { spot::parse_infix_psl("!q").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }
}