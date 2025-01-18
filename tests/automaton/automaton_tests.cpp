#include <PolyhedralSystemFormulaDenotationMap.h>
#include <catch2/catch_test_macros.hpp>
#include "PolyhedralSystemParsingResult.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"

using PPL::IO_Operators::operator<<;

TEST_CASE("Formula denotation map TEST 1")
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
    PolyhedralSystemSymbolTable symbolTable { polyhedralSystem->getSymbolTable() };
    const Powerset& pPowerset { (*polyhedralSystem->getInterpretation("p"))->interpretation() };
    const Powerset& qPowerset { (*polyhedralSystem->getInterpretation("q"))->interpretation() };
    const Powerset& t0Powerset { (*polyhedralSystem->getInterpretation("t0"))->interpretation() };
    const Powerset& t1Powerset { (*polyhedralSystem->getInterpretation("t1"))->interpretation() };

    const Powerset& not_pPowerset { (*polyhedralSystem->getInterpretation("p"))->notInterpretation() };
    const Powerset& not_qPowerset { (*polyhedralSystem->getInterpretation("q"))->notInterpretation() };
    const Powerset& not_t0Powerset { (*polyhedralSystem->getInterpretation("t0"))->notInterpretation() };
    const Powerset& not_t1Powerset { (*polyhedralSystem->getInterpretation("t1"))->notInterpretation() };

    SECTION("p & q & t0 & t1")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("p & q & t0 & t1").f };
        std::vector powersets { pPowerset, qPowerset, t0Powerset, t1Powerset };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::intersect(powersets) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("(p & q) | (t0 & t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & q) | (t0 & t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::intersect(pPowerset, qPowerset) };
        PowersetConstUniquePtr t0t1 { PPLUtils::intersect(t0Powerset, t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(pq, t0t1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("(p & !q) | (t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & !q) | (t0 & !t1)").f };

        PowersetConstUniquePtr p_notq { PPLUtils::intersect(pPowerset, not_qPowerset) };
        PowersetConstUniquePtr t0_nott1 { PPLUtils::intersect(t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(p_notq, t0_nott1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("(!p & q) | (!t0 & t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(!p & q) | (!t0 & t1)").f };

        PowersetConstUniquePtr notp_q { PPLUtils::intersect(not_pPowerset, qPowerset) };
        PowersetConstUniquePtr nott0_t1 { PPLUtils::intersect(not_t0Powerset, t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(notp_q, nott0_t1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("(!p & !q) | (!t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(!p & !q) | (!t0 & !t1)").f };

        PowersetConstUniquePtr notp_notq { PPLUtils::intersect(not_pPowerset, not_qPowerset) };
        PowersetConstUniquePtr nott0_nott1 { PPLUtils::intersect(not_t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(notp_notq, nott0_nott1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("(p & q) | (t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("(p & q) | (t0 & !t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::intersect(pPowerset, qPowerset) };
        PowersetConstUniquePtr t0_nott1 { PPLUtils::intersect(t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(pq, t0_nott1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }

    SECTION("p | q | (!t0 & !t1)")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        spot::formula formula { spot::parse_infix_psl("p | q | (!t0 & !t1)").f };

        PowersetConstUniquePtr pq { PPLUtils::fusion(pPowerset, qPowerset) };
        PowersetConstUniquePtr nott0_nott1 { PPLUtils::intersect(not_t0Powerset, not_t1Powerset) };
        PowersetConstUniquePtr expectedPowerset { PPLUtils::fusion(pq, nott0_nott1) };

        PowersetConstSharedPtr denotation { polyhedralSystemFormulaDenotationMap.getOrComputeDenotation(formula) };

        REQUIRE(*expectedPowerset == *denotation);
    }
}