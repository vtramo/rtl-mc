#include <catch2/catch_test_macros.hpp>
#include <spot/tl/parse.hh>
#include "spot_utils.h"

TEST_CASE("Remove sing from formula util function")
{
    SECTION("(a & b) | (c & d)")
    {
        spot::formula formula { spot::parse_infix_psl("(a & b) | (c & d)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == formula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("sing")
    {
        spot::formula formula { spot::parse_infix_psl("sing").f };
        spot::formula expectedFormula { spot::formula::ff() };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("!sing")
    {
        spot::formula formula { spot::parse_infix_psl("sing").f };
        spot::formula expectedFormula { spot::formula::ff() };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("sing & p0")
    {
        spot::formula formula { spot::parse_infix_psl("sing & p0").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("p0 & sing")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & sing").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("p0 & !sing")
    {
        spot::formula formula { spot::parse_infix_psl("p0 & !sing").f };
        spot::formula expectedFormula { spot::parse_infix_psl("p0").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);
    }

    SECTION("(p5 & (sing | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p5 & (sing | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p5 & p6)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & sing & p1 & p2) | (!sing & p3 & p4) | (p5 & (sing | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & sing & p1 & p2) | (!sing & p3 & p4) | (p5 & (sing | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2) | (p3 & p4) | (p5 & p6)").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & sing & p1 & p2 & (p3 | p4 | sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & sing & p1 & p2 & (p3 | p4 | sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2 & (p3 | p4)) | ((p1 | p8 | p9) & p3 & p4) | (p5 & ((p9 & p7) | p6))").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(removedAtLeastOneSing);
    }

    SECTION("(p0 & !sing & p1 & p2 & (p3 | p4 | !sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))")
    {
        spot::formula formula { spot::parse_infix_psl("(p0 & !sing & p1 & p2 & (p3 | p4 | !sing)) | ((p1 | (!sing & (p8 | p9))) & p3 & p4) | (p5 & ((p9 & p7 & !sing) | p6))").f };
        spot::formula expectedFormula { spot::parse_infix_psl("(p0 & p1 & p2 & (p3 | p4)) | ((p1 | p8 | p9) & p3 & p4) | (p5 & ((p9 & p7) | p6))").f };

        const auto& [formulaWithoutSing, removedAtLeastOneSing] { SpotUtils::removeSing(std::move(formula)) };

        REQUIRE(formulaWithoutSing == expectedFormula);
        REQUIRE(!removedAtLeastOneSing);
    }
}