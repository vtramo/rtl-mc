#include <spot_utils.h>
#include <catch2/catch_test_macros.hpp>
#include "BackwardNFA.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralSystemParsingResult.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"

using PPL::IO_Operators::operator<<;
using namespace SpotUtils;

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


std::unordered_set<int> predecessors(const BackwardNFA& backwardNfa, int state);

TEST_CASE("t0 & G(t1) & F(p & F(q)) with HIGH optimization")
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

    DiscreteFiniteLtlFormula discreteFiniteLtlFormula { DiscreteFiniteLtlFormula::discretize(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f) };
    DiscreteLtlFormula discreteLtlFormula { discreteFiniteLtlFormula.toLtl() };
    PolyhedralSystemFormulaDenotationMap formulaDenotationMap { polyhedralSystem };

    BackwardNFA backwardNfa { discreteLtlFormula, std::move(formulaDenotationMap), spot::postprocessor::High };
    REQUIRE(backwardNfa.totalStates() == 15);
    REQUIRE(backwardNfa.totalInitialStates() == 3);
    REQUIRE(backwardNfa.totalFinalStates() == 4);
    REQUIRE(backwardNfa.finalStates() == std::unordered_set<int>{ 2, 4, 7, 13 });
    REQUIRE(backwardNfa.initialStates() == std::unordered_set<int>{ 0, 1, 2 });
    REQUIRE(backwardNfa.totalEdges() == 26);

    constexpr int stateZero = 0;
    const StateDenotation& zeroStateDenotation { backwardNfa.stateDenotation(0) };
    REQUIRE(zeroStateDenotation.isSingular());
    REQUIRE(zeroStateDenotation.formula() == spot::parse_infix_psl("p & !q & t0 & t1").f);
    REQUIRE(backwardNfa.isInitialState(stateZero));
    REQUIRE(!backwardNfa.isFinalState(stateZero));
    REQUIRE(!backwardNfa.hasPredecessors(stateZero));
    REQUIRE(predecessors(backwardNfa, stateZero) == std::unordered_set<int>{});

    constexpr int stateOne = 1;
    const StateDenotation& oneStateDenotation { backwardNfa.stateDenotation(stateOne) };
    REQUIRE(oneStateDenotation.isSingular());
    REQUIRE(oneStateDenotation.formula() == spot::parse_infix_psl("!p & t0 & t1").f);
    REQUIRE(backwardNfa.isInitialState(stateOne));
    REQUIRE(!backwardNfa.isFinalState(stateOne));
    REQUIRE(!backwardNfa.hasPredecessors(stateOne));
    REQUIRE(predecessors(backwardNfa, stateOne) == std::unordered_set<int>{});

    constexpr int stateTwo = 2;
    const StateDenotation& twoStateDenotation { backwardNfa.stateDenotation(stateTwo) };
    REQUIRE(twoStateDenotation.isSingular());
    REQUIRE(twoStateDenotation.formula() == spot::parse_infix_psl("p & q & t0 & t1").f);
    REQUIRE(backwardNfa.isInitialState(stateTwo));
    REQUIRE(backwardNfa.isFinalState(stateTwo));
    REQUIRE(!backwardNfa.hasPredecessors(stateTwo));
    REQUIRE(predecessors(backwardNfa, stateTwo) == std::unordered_set<int>{});

    constexpr int stateThree = 3;
    const StateDenotation& threeStateDenotation { backwardNfa.stateDenotation(stateThree) };
    REQUIRE(threeStateDenotation.isSingular());
    REQUIRE(threeStateDenotation.formula() == spot::parse_infix_psl("!q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateThree));
    REQUIRE(!backwardNfa.isFinalState(stateThree));
    REQUIRE(backwardNfa.hasPredecessors(stateThree));
    REQUIRE(predecessors(backwardNfa, stateThree) == std::unordered_set<int>{ 8, 10 });

    constexpr int stateFour = 4;
    const StateDenotation& fourStateDenotation { backwardNfa.stateDenotation(stateFour) };
    REQUIRE(fourStateDenotation.isSingular());
    REQUIRE(fourStateDenotation.formula() == spot::parse_infix_psl("q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateFour));
    REQUIRE(backwardNfa.isFinalState(stateFour));
    REQUIRE(backwardNfa.hasPredecessors(stateFour));
    REQUIRE(predecessors(backwardNfa, stateFour) == std::unordered_set<int>{ 8, 10 });

    constexpr int stateFive = 5;
    const StateDenotation& fiveStateDenotation { backwardNfa.stateDenotation(stateFive) };
    REQUIRE(fiveStateDenotation.isSingular());
    REQUIRE(fiveStateDenotation.formula() == spot::parse_infix_psl("p & !q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateFive));
    REQUIRE(!backwardNfa.isFinalState(stateFive));
    REQUIRE(backwardNfa.hasPredecessors(stateFive));
    REQUIRE(predecessors(backwardNfa, stateFive) == std::unordered_set<int>{ 11 });

    constexpr int stateSix = 6;
    const StateDenotation& sixStateDenotation { backwardNfa.stateDenotation(stateSix) };
    REQUIRE(sixStateDenotation.isSingular());
    REQUIRE(sixStateDenotation.formula() == spot::parse_infix_psl("!p & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateSix));
    REQUIRE(!backwardNfa.isFinalState(stateSix));
    REQUIRE(backwardNfa.hasPredecessors(stateSix));
    REQUIRE(predecessors(backwardNfa, stateSix) == std::unordered_set<int>{ 11 });

    constexpr int stateSeven = 7;
    const StateDenotation& sevenStateDenotation { backwardNfa.stateDenotation(stateSeven) };
    REQUIRE(sevenStateDenotation.isSingular());
    REQUIRE(sevenStateDenotation.formula() == spot::parse_infix_psl("p & q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateSeven));
    REQUIRE(backwardNfa.isFinalState(stateSeven));
    REQUIRE(backwardNfa.hasPredecessors(stateSeven));
    REQUIRE(predecessors(backwardNfa, stateSeven) == std::unordered_set<int>{ 11 });

    constexpr int stateEight = 8;
    const StateDenotation& eightStateDenotation { backwardNfa.stateDenotation(stateEight) };
    REQUIRE(!eightStateDenotation.isSingular());
    REQUIRE(eightStateDenotation.formula() == spot::parse_infix_psl("!q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateEight));
    REQUIRE(!backwardNfa.isFinalState(stateEight));
    REQUIRE(backwardNfa.hasPredecessors(stateEight));
    REQUIRE(predecessors(backwardNfa, stateEight) == std::unordered_set<int>{ 0, 3, 5 });

    constexpr int stateNine = 9;
    const StateDenotation& nineStateDenotation { backwardNfa.stateDenotation(stateNine) };
    REQUIRE(!nineStateDenotation.isSingular());
    REQUIRE(nineStateDenotation.formula() == spot::parse_infix_psl("q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateNine));
    REQUIRE(!backwardNfa.isFinalState(stateNine));
    REQUIRE(backwardNfa.hasPredecessors(stateNine));
    REQUIRE(predecessors(backwardNfa, stateNine) == std::unordered_set<int>{ 0, 3, 5 });

    constexpr int stateTen = 10;
    const StateDenotation& tenStateDenotation { backwardNfa.stateDenotation(stateTen) };
    REQUIRE(!tenStateDenotation.isSingular());
    REQUIRE(tenStateDenotation.formula() == spot::parse_infix_psl("p & !q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateTen));
    REQUIRE(!backwardNfa.isFinalState(stateTen));
    REQUIRE(backwardNfa.hasPredecessors(stateTen));
    REQUIRE(predecessors(backwardNfa, stateTen) == std::unordered_set<int>{ 1, 6 });

    constexpr int stateEleven = 11;
    const StateDenotation& elevenStateDenotation { backwardNfa.stateDenotation(stateEleven) };
    REQUIRE(!elevenStateDenotation.isSingular());
    REQUIRE(elevenStateDenotation.formula() == spot::parse_infix_psl("!p & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateEleven));
    REQUIRE(!backwardNfa.isFinalState(stateEleven));
    REQUIRE(backwardNfa.hasPredecessors(stateEleven));
    REQUIRE(predecessors(backwardNfa, stateEleven) == std::unordered_set<int>{ 1, 6 });

    constexpr int stateTwelve = 12;
    const StateDenotation& twelveStateDenotation { backwardNfa.stateDenotation(stateTwelve) };
    REQUIRE(!twelveStateDenotation.isSingular());
    REQUIRE(twelveStateDenotation.formula() == spot::parse_infix_psl("p & q & t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateTwelve));
    REQUIRE(!backwardNfa.isFinalState(stateTwelve));
    REQUIRE(backwardNfa.hasPredecessors(stateTwelve));
    REQUIRE(predecessors(backwardNfa, stateTwelve) == std::unordered_set<int>{ 1, 6 });

    constexpr int stateThirteen = 13;
    const StateDenotation& thirteenStateDenotation { backwardNfa.stateDenotation(stateThirteen) };
    REQUIRE(thirteenStateDenotation.isSingular());
    REQUIRE(thirteenStateDenotation.formula() == spot::parse_infix_psl("t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateThirteen));
    REQUIRE(backwardNfa.isFinalState(stateThirteen));
    REQUIRE(backwardNfa.hasPredecessors(stateThirteen));
    REQUIRE(predecessors(backwardNfa, stateThirteen) == std::unordered_set<int>{ 9, 12, 14 });

    constexpr int stateFourteen = 14;
    const StateDenotation& fourteenStateDenotation { backwardNfa.stateDenotation(stateFourteen) };
    REQUIRE(!fourteenStateDenotation.isSingular());
    REQUIRE(fourteenStateDenotation.formula() == spot::parse_infix_psl("t1").f);
    REQUIRE(!backwardNfa.isInitialState(stateFourteen));
    REQUIRE(!backwardNfa.isFinalState(stateFourteen));
    REQUIRE(backwardNfa.hasPredecessors(stateFourteen));
    REQUIRE(predecessors(backwardNfa, stateFourteen) == std::unordered_set<int>{ 2, 4, 7, 13 });
}

std::unordered_set<int> predecessors(const BackwardNFA& backwardNfa, const int state)
{
    REQUIRE(state < backwardNfa.totalStates());
    std::unordered_set<int> predecessors {};
    for (const auto& predecessorEdge: backwardNfa.predecessors(state))
        predecessors.insert(predecessorEdge.dst);
    return predecessors;
}


void testBackwardNfaInvariant(const BackwardNFA& backwardNfa);

TEST_CASE("BackwardNFA invariant")
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

    SECTION("t0 & G(t1) & F(p & F(q))")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=20")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(20) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=50")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(50) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=55")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(55) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=58")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(58) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=59")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(59) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=60")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(60) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=100")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(100) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=500")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretize(And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(500) })).toLtl() };
        BackwardNFA backwardNfa { formula, std::move(polyhedralSystemFormulaDenotationMap) };
        testBackwardNfaInvariant(backwardNfa);
    }
}

void testBackwardNfaInvariant(const BackwardNFA& backwardNfa)
{
    for (int state = 0; state < backwardNfa.totalStates(); state++)
    {
        const StateDenotation& stateDenotation { backwardNfa.stateDenotation(state) };
        if (backwardNfa.isInitialState(state))
        {
            BackwardNFA::EdgeIterator edgeIterator { backwardNfa.predecessors(state) };
            REQUIRE(edgeIterator.begin() == edgeIterator.end()); // Initial state => no predecessors
            REQUIRE(stateDenotation.isSingular()); // Initial state => is singular
            continue;
        }


        bool currentStateIsSingular { stateDenotation.isSingular() };
        for (const auto& edgePredecessor: backwardNfa.predecessors(state))
        {
            INFO("State is " << state);
            REQUIRE(state != edgePredecessor.dst); // No self-loop

            const int predecessor { static_cast<int>(edgePredecessor.dst) };
            const StateDenotation& predecessorStateDenotation { backwardNfa.stateDenotation(predecessor) };

            REQUIRE(predecessorStateDenotation.isSingular() != currentStateIsSingular);
        }
    }
}