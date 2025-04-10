#include <catch2/catch_test_macros.hpp>
#include "BackwardNFA.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralSystemParsingResult.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "BackwardNFADepthFirstSearch.h"
#include "spot_utils.h"

using namespace SpotUtils;

std::unordered_set<unsigned> predecessors(BackwardNFAConstSharedPtr backwardNfa, unsigned state);

TEST_CASE("GAP experiment t0 & G(t1) & F(p & F(q)) with HIGH optimization")
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

    DiscreteFiniteLtlFormula discreteFiniteLtlFormula { DiscreteFiniteLtlFormula::discretiseRtlFinite(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f) };
    DiscreteLtlFormula discreteLtlFormula { discreteFiniteLtlFormula.toLtl() };
    PolyhedralSystemFormulaDenotationMap formulaDenotationMap { polyhedralSystem };

    BackwardNFAConstSharedPtr backwardNfa {
        BackwardNFA::buildAutomaton(
            std::move(discreteLtlFormula),
            std::move(formulaDenotationMap),
            spot::postprocessor::High
        )
    };
    REQUIRE(backwardNfa->totalStates() == 14);
    REQUIRE(backwardNfa->totalInitialStates() == 4);
    REQUIRE(backwardNfa->totalAcceptingStates() == 2);
    REQUIRE(backwardNfa->acceptingStates() == std::unordered_set<unsigned>{ 12, 5 });
    REQUIRE(backwardNfa->initialStates() == std::unordered_set<unsigned>{ 0, 1, 2, 3 });
    REQUIRE(backwardNfa->totalEdges() == 22);

    constexpr unsigned stateZero = 0;
    const StateDenotation& zeroStateDenotation { backwardNfa->stateDenotation(0) };
    REQUIRE(!zeroStateDenotation.isSingular());
    REQUIRE(zeroStateDenotation.formula() == spot::parse_infix_psl("p & !q & !sing & t0 & t1").f);
    REQUIRE(zeroStateDenotation.labels() == AP({ "p", "q", "t0", "t1" }));
    REQUIRE(backwardNfa->isInitialState(stateZero));
    REQUIRE(!backwardNfa->isAcceptingState(stateZero));
    REQUIRE(!backwardNfa->hasSuccessors(stateZero));
    REQUIRE(predecessors(backwardNfa, stateZero) == std::unordered_set<unsigned>{});
    REQUIRE(backwardNfa->countSuccessors(stateZero) == 0);

    constexpr unsigned stateOne = 1;
    const StateDenotation& oneStateDenotation { backwardNfa->stateDenotation(stateOne) };
    REQUIRE(!oneStateDenotation.isSingular());
    REQUIRE(oneStateDenotation.formula() == spot::parse_infix_psl("!p & !sing & t0 & t1").f);
    REQUIRE(oneStateDenotation.labels() == AP({ "p", "t0", "t1" }));
    REQUIRE(backwardNfa->isInitialState(stateOne));
    REQUIRE(!backwardNfa->isAcceptingState(stateOne));
    REQUIRE(!backwardNfa->hasSuccessors(stateOne));
    REQUIRE(predecessors(backwardNfa, stateOne) == std::unordered_set<unsigned>{});
    REQUIRE(backwardNfa->countSuccessors(stateOne) == 0);

    constexpr unsigned stateTwo = 2;
    const StateDenotation& twoStateDenotation { backwardNfa->stateDenotation(stateTwo) };
    REQUIRE(twoStateDenotation.isSingular());
    REQUIRE(twoStateDenotation.formula() == spot::parse_infix_psl("p & !q & sing & t0 & t1").f);
    REQUIRE(twoStateDenotation.labels() == AP({ "p", "q", "sing", "t0", "t1" }));
    REQUIRE(backwardNfa->isInitialState(stateTwo));
    REQUIRE(!backwardNfa->isAcceptingState(stateTwo));
    REQUIRE(!backwardNfa->hasSuccessors(stateTwo));
    REQUIRE(predecessors(backwardNfa, stateTwo) == std::unordered_set<unsigned>{});
    REQUIRE(backwardNfa->countSuccessors(stateTwo) == 0);

    constexpr unsigned stateThree = 3;
    const StateDenotation& threeStateDenotation { backwardNfa->stateDenotation(stateThree) };
    REQUIRE(threeStateDenotation.isSingular());
    REQUIRE(threeStateDenotation.formula() == spot::parse_infix_psl("!p & sing & t0 & t1").f);
    REQUIRE(threeStateDenotation.labels() == AP({ "p", "sing", "t0", "t1" }));
    REQUIRE(backwardNfa->isInitialState(stateThree));
    REQUIRE(!backwardNfa->isAcceptingState(stateThree));
    REQUIRE(!backwardNfa->hasSuccessors(stateThree));
    REQUIRE(predecessors(backwardNfa, stateThree) == std::unordered_set<unsigned>{ });
    REQUIRE(backwardNfa->countSuccessors(stateThree) == 0);

    constexpr unsigned stateFour = 4;
    const StateDenotation& fourStateDenotation { backwardNfa->stateDenotation(stateFour) };
    REQUIRE(fourStateDenotation.isSingular());
    REQUIRE(fourStateDenotation.formula() == spot::parse_infix_psl("!q & sing & t1").f);
    REQUIRE(fourStateDenotation.labels() == AP({ "q", "sing", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateFour));
    REQUIRE(!backwardNfa->isAcceptingState(stateFour));
    REQUIRE(backwardNfa->hasSuccessors(stateFour));
    REQUIRE(predecessors(backwardNfa, stateFour) == std::unordered_set<unsigned>{ 10, 8, 0 });
    REQUIRE(backwardNfa->countSuccessors(stateFour) == 3);

    constexpr unsigned stateFive = 5;
    const StateDenotation& fiveStateDenotation { backwardNfa->stateDenotation(stateFive) };
    REQUIRE(fiveStateDenotation.isSingular());
    REQUIRE(fiveStateDenotation.formula() == spot::parse_infix_psl("q & sing & t1").f);
    REQUIRE(fiveStateDenotation.labels() == AP({ "q", "sing", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateFive));
    REQUIRE(backwardNfa->isAcceptingState(stateFive));
    REQUIRE(backwardNfa->hasSuccessors(stateFive));
    REQUIRE(predecessors(backwardNfa, stateFive) == std::unordered_set<unsigned>{ 10, 8, 0 });
    REQUIRE(backwardNfa->countSuccessors(stateFive) == 3);

    constexpr unsigned stateSix = 6;
    const StateDenotation& sixStateDenotation { backwardNfa->stateDenotation(stateSix) };
    REQUIRE(sixStateDenotation.isSingular());
    REQUIRE(sixStateDenotation.formula() == spot::parse_infix_psl("p & !q & sing & t1").f);
    REQUIRE(sixStateDenotation.labels() == AP({ "p", "q", "sing", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateSix));
    REQUIRE(!backwardNfa->isAcceptingState(stateSix));
    REQUIRE(backwardNfa->hasSuccessors(stateSix));
    REQUIRE(predecessors(backwardNfa, stateSix) == std::unordered_set<unsigned>{ 11, 1 });
    REQUIRE(backwardNfa->countSuccessors(stateSix) == 2);

    constexpr unsigned stateSeven = 7;
    const StateDenotation& sevenStateDenotation { backwardNfa->stateDenotation(stateSeven) };
    REQUIRE(sevenStateDenotation.isSingular());
    REQUIRE(sevenStateDenotation.formula() == spot::parse_infix_psl("!p & sing & t1").f);
    REQUIRE(sevenStateDenotation.labels() == AP({ "p", "sing", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateSeven));
    REQUIRE(!backwardNfa->isAcceptingState(stateSeven));
    REQUIRE(backwardNfa->hasSuccessors(stateSeven));
    REQUIRE(predecessors(backwardNfa, stateSeven) == std::unordered_set<unsigned>{ 11, 1 });
    REQUIRE(backwardNfa->countSuccessors(stateSeven) == 2);

    constexpr unsigned stateEight = 8;
    const StateDenotation& eightStateDenotation { backwardNfa->stateDenotation(stateEight) };
    REQUIRE(!eightStateDenotation.isSingular());
    REQUIRE(eightStateDenotation.formula() == spot::parse_infix_psl("!q & !sing & t1").f);
    REQUIRE(eightStateDenotation.labels() == AP({ "q", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateEight));
    REQUIRE(!backwardNfa->isAcceptingState(stateEight));
    REQUIRE(backwardNfa->hasSuccessors(stateEight));
    REQUIRE(predecessors(backwardNfa, stateEight) == std::unordered_set<unsigned>{ 6, 4, 2 });
    REQUIRE(backwardNfa->countSuccessors(stateEight) == 3);

    constexpr unsigned stateNine = 9;
    const StateDenotation& nineStateDenotation { backwardNfa->stateDenotation(stateNine) };
    REQUIRE(!nineStateDenotation.isSingular());
    REQUIRE(nineStateDenotation.formula() == spot::parse_infix_psl("q & !sing & t1").f);
    REQUIRE(nineStateDenotation.labels() == AP({ "q", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateNine));
    REQUIRE(!backwardNfa->isAcceptingState(stateNine));
    REQUIRE(backwardNfa->hasSuccessors(stateNine));
    REQUIRE(predecessors(backwardNfa, stateNine) == std::unordered_set<unsigned>{ 6, 4, 2 });
    REQUIRE(backwardNfa->countSuccessors(stateNine) == 3);

    constexpr unsigned stateTen = 10;
    const StateDenotation& tenStateDenotation { backwardNfa->stateDenotation(stateTen) };
    REQUIRE(!tenStateDenotation.isSingular());
    REQUIRE(tenStateDenotation.formula() == spot::parse_infix_psl("p & !q & !sing & t1").f);
    REQUIRE(tenStateDenotation.labels() == AP({ "p", "q", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateTen));
    REQUIRE(!backwardNfa->isAcceptingState(stateTen));
    REQUIRE(backwardNfa->hasSuccessors(stateTen));
    REQUIRE(predecessors(backwardNfa, stateTen) == std::unordered_set<unsigned>{ 7, 3 });
    REQUIRE(backwardNfa->countSuccessors(stateTen) == 2);

    constexpr unsigned stateEleven = 11;
    const StateDenotation& elevenStateDenotation { backwardNfa->stateDenotation(stateEleven) };
    REQUIRE(!elevenStateDenotation.isSingular());
    REQUIRE(elevenStateDenotation.formula() == spot::parse_infix_psl("!p & !sing & t1").f);
    REQUIRE(elevenStateDenotation.labels() == AP({ "p", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateEleven));
    REQUIRE(!backwardNfa->isAcceptingState(stateEleven));
    REQUIRE(backwardNfa->hasSuccessors(stateEleven));
    REQUIRE(predecessors(backwardNfa, stateEleven) == std::unordered_set<unsigned>{ 7, 3 });
    REQUIRE(backwardNfa->countSuccessors(stateEleven) == 2);

    constexpr unsigned stateTwelve = 12;
    const StateDenotation& twelveStateDenotation { backwardNfa->stateDenotation(stateTwelve) };
    REQUIRE(twelveStateDenotation.isSingular());
    REQUIRE(twelveStateDenotation.formula() == spot::parse_infix_psl("sing & t1").f);
    REQUIRE(twelveStateDenotation.labels() == AP({ "sing", "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateTwelve));
    REQUIRE(backwardNfa->isAcceptingState(stateTwelve));
    REQUIRE(backwardNfa->hasSuccessors(stateTwelve));
    REQUIRE(predecessors(backwardNfa, stateTwelve) == std::unordered_set<unsigned>{ 13, 9 });
    REQUIRE(backwardNfa->countSuccessors(stateTwelve) == 2);

    constexpr unsigned stateThirteen = 13;
    const StateDenotation& thirteenStateDenotation { backwardNfa->stateDenotation(stateThirteen) };
    REQUIRE(!thirteenStateDenotation.isSingular());
    REQUIRE(thirteenStateDenotation.formula() == spot::parse_infix_psl("!sing & t1").f);
    REQUIRE(thirteenStateDenotation.labels() == AP({ "t1" }));
    REQUIRE(!backwardNfa->isInitialState(stateThirteen));
    REQUIRE(!backwardNfa->isAcceptingState(stateThirteen));
    REQUIRE(!backwardNfa->hasSuccessors(stateThirteen));
    REQUIRE(predecessors(backwardNfa, stateThirteen) == std::unordered_set<unsigned>{ });
    REQUIRE(backwardNfa->countSuccessors(stateThirteen) == 0);
}

std::unordered_set<unsigned> predecessors(BackwardNFAConstSharedPtr backwardNfa, const unsigned state)
{
    REQUIRE(state < backwardNfa->totalStates());
    std::unordered_set<unsigned> predecessors {};
    for (const auto& predecessorEdge: backwardNfa->successors(state))
        predecessors.insert(predecessorEdge.dst);
    return predecessors;
}


void testBackwardNfaInvariant(BackwardNFAConstSharedPtr backwardNfa);

TEST_CASE("BackwardNFA invariant GAP Experiment")
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

    SECTION("t0 & G(t1) & F(p & F(q))")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(spot::parse_infix_psl("t0 & G(t1) & F(p & F(q))").f).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=20")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(20) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=50")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(50) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=55")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(55) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=58")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(58) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=59")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(59) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=60")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(60) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=100")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(100) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=253")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(253) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=254")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(254) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=500")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(500) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("GAP k=1000")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(1000) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not GAP k=1")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(1) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not GAP k=10")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(10) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not GAP k=20")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(20) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }
}


TEST_CASE("BackwardNFA invariant NOGAP Experiment")
{
    PolyhedralSystemSharedPtr polyhedralSystem {
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

    SECTION("NO GAP k=1")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(1) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=10")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(10) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=20")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(20) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=50")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(50) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=55")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(55) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=58")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(58) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=59")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(59) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=60")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(60) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=100")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(100) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=500")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(500) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("NO GAP k=1000")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(1000) })).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not NO GAP k=1")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(1) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not NO GAP k=10")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(10) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }

    SECTION("not NO GAP k=20")
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(Not(andFormulae({ ap("t0"), G(ap("t1")), generateAlternatingFormula(20) }))).toLtl() };
        BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
        testBackwardNfaInvariant(backwardNfa);
    }
}

TEST_CASE("BackwardNFA invariant GAP Experiment with formula t0 & F(p & F(q & t1)")
{
    PolyhedralSystemSharedPtr polyhedralSystem {
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

    spot::formula rtlf { spot::parse_infix_psl("t0 & F(p & F(q & t1)").f };
    DiscreteLtlFormula formula { DiscreteFiniteLtlFormula::discretiseRtlFinite(std::move(rtlf)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    BackwardNFAConstSharedPtr backwardNfa { BackwardNFA::buildAutomaton(std::move(formula), std::move(polyhedralSystemFormulaDenotationMap)) };
    testBackwardNfaInvariant(backwardNfa);
}

class BackwardNFADepthFirstSearchInvariantCheck final : public BackwardNFADepthFirstSearch {
public:
    explicit BackwardNFADepthFirstSearchInvariantCheck(BackwardNFAConstSharedPtr backwardNfa)
        : BackwardNFADepthFirstSearch(backwardNfa)
    {
    }
private:
    void processEdge(const unsigned src, const unsigned dst) override
    {
        INFO("Edge (" << src << ", " << dst << ")");
        const bool srcIsSing { m_backwardNfa->stateDenotation(src).isSingular() };
        const bool dstIsSing { m_backwardNfa->stateDenotation(dst).isSingular() };
        REQUIRE(srcIsSing != dstIsSing);
    }
};

void testBackwardNfaInvariant(BackwardNFAConstSharedPtr backwardNfa)
{
    for (unsigned state { 0 }; state < backwardNfa->totalStates(); state++)
    {
        INFO("State is " << state);
        const StateDenotation& stateDenotation { backwardNfa->stateDenotation(state) };
        if (backwardNfa->isInitialState(state))
        {
            REQUIRE(!backwardNfa->hasSuccessors(state)); // Initial state => no predecessors
            REQUIRE(!stateDenotation.isEmpty()); // Initial state => state denotation IS NOT empty
            continue;
        }

        if (backwardNfa->isAcceptingState(state))
        {
            REQUIRE(!stateDenotation.isEmpty()); // Final state => denotation IS NOT empty
        }

        bool currentStateIsSingular { stateDenotation.isSingular() };
        for (const auto& edgePredecessor: backwardNfa->successors(state))
        {
            const unsigned predecessor { edgePredecessor.dst };
            REQUIRE(state != predecessor); // No self-loop
            REQUIRE(!backwardNfa->isAcceptingState(predecessor)); // The predecessor is not a final state

            const StateDenotation& predecessorStateDenotation { backwardNfa->stateDenotation(predecessor) };
            REQUIRE(predecessorStateDenotation.isSingular() != currentStateIsSingular);
        }
    }

    BackwardNFADepthFirstSearchInvariantCheck backwardNfaDfs { backwardNfa };
    backwardNfaDfs();
}