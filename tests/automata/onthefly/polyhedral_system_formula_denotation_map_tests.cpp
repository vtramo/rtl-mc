#include <catch2/catch_test_macros.hpp>
#include <spot/twaalgos/reachiter.hh>
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "formula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"

TEST_CASE("Formula formulaDenotation map TEST 1")
{
    PolyhedralSystemSharedPtr polyhedralSystem {
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
    PolyhedralSystemSymbolTable symbolTable { polyhedralSystem->symbolTable() };
    const Powerset& pPowerset { (*polyhedralSystem->getAtomInterpretation("p"))->interpretation() };
    const Powerset& qPowerset { (*polyhedralSystem->getAtomInterpretation("q"))->interpretation() };
    const Powerset& t0Powerset { (*polyhedralSystem->getAtomInterpretation("t0"))->interpretation() };
    const Powerset& t1Powerset { (*polyhedralSystem->getAtomInterpretation("t1"))->interpretation() };

    const Powerset& not_pPowerset { (*polyhedralSystem->getAtomInterpretation("p"))->notInterpretation() };
    const Powerset& not_qPowerset { (*polyhedralSystem->getAtomInterpretation("q"))->notInterpretation() };
    const Powerset& not_t0Powerset { (*polyhedralSystem->getAtomInterpretation("t0"))->notInterpretation() };
    const Powerset& not_t1Powerset { (*polyhedralSystem->getAtomInterpretation("t1"))->notInterpretation() };

    SECTION("top")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { top() };

        Powerset expectedFormulaDenotation { polyhedralSystem->spaceDimension(), PPL::UNIVERSE };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("bottom")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { bottom() };

        Powerset expectedFormulaDenotation { polyhedralSystem->spaceDimension(), PPL::EMPTY };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("p & q & t0 & t1")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("p & q & t0 & t1").f };
        std::vector powersets { pPowerset, qPowerset, t0Powerset, t1Powerset };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::intersect(powersets) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(p & q) | (t0 & t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & q) | (t0 & t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::intersect(pPowerset, qPowerset) };
        PowersetConstUniquePtr t0t1 { PPLUtils::intersect(t0Powerset, t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(pq, t0t1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(p & !q) | (t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & !q) | (t0 & !t1)").f };

        PowersetConstUniquePtr p_notq { PPLUtils::intersect(pPowerset, not_qPowerset) };
        PowersetConstUniquePtr t0_nott1 { PPLUtils::intersect(t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(p_notq, t0_nott1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(!p & q) | (!t0 & t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(!p & q) | (!t0 & t1)").f };

        PowersetConstUniquePtr notp_q { PPLUtils::intersect(not_pPowerset, qPowerset) };
        PowersetConstUniquePtr nott0_t1 { PPLUtils::intersect(not_t0Powerset, t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(notp_q, nott0_t1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(!p & !q) | (!t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(!p & !q) | (!t0 & !t1)").f };

        PowersetConstUniquePtr notp_notq { PPLUtils::intersect(not_pPowerset, not_qPowerset) };
        PowersetConstUniquePtr nott0_nott1 { PPLUtils::intersect(not_t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(notp_notq, nott0_nott1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(p & q) | (t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & q) | (t0 & !t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::intersect(pPowerset, qPowerset) };
        PowersetConstUniquePtr t0_nott1 { PPLUtils::intersect(t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(pq, t0_nott1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("p | q | (!t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("p | q | (!t0 & !t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::fusion(pPowerset, qPowerset) };
        PowersetConstUniquePtr nott0_nott1 { PPLUtils::intersect(not_t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(pq, nott0_nott1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }

    SECTION("(!q & t0 & t1) | (!p & t0 & t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(!q & t0 & t1) | (!p & t0 & t1)").f };

        PowersetConstUniquePtr t0_t1 { PPLUtils::intersect(t0Powerset, t1Powerset) };
        PowersetConstUniquePtr notq_t0_t1 { PPLUtils::intersect(not_qPowerset, *t0_t1) };
        PowersetConstUniquePtr notp_t0_t1 { PPLUtils::intersect(not_pPowerset, *t0_t1) };
        PowersetConstUniquePtr expectedFormulaDenotation { PPLUtils::fusion(notq_t0_t1, notp_t0_t1) };

        PowersetConstSharedPtr formulaDenotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedFormulaDenotation == *formulaDenotation);
    }
}

