#pragma once

#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralBuchiLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "automata_builder.h"
#include <spot/twaalgos/couvreurnew.hh>
#include <spot/twaalgos/se05.hh>

inline PowersetSharedPtr omnidirectionalInfiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretiseInfinite(std::move(rtlFormula)) };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralBuchiLtlAutomatonConstSharedPtr buchiLtlAutomaton {
        buildPolyhedralBuchiLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };
    auto abstraction { std::make_shared<OmnidirectionalPolyhedralAbstraction>(polyhedralSystem) };
    auto synchronousProduct { std::make_shared<PolyhedralSynchronousProductAutomaton>(buchiLtlAutomaton, abstraction) };

    PowersetSharedPtr result { std::make_shared<Powerset>(synchronousProduct->spaceDimension(), PPL::EMPTY) };
    spot::emptiness_check_ptr explicitSe05Search { spot::explicit_se05_search(synchronousProduct->twa()) };
    spot::emptiness_check_result_ptr emptinessCheckResult { explicitSe05Search->check() };
    while (emptinessCheckResult != nullptr)
    {
        spot::twa_run_ptr twaRun { emptinessCheckResult->accepting_run() };
        std::list<spot::twa_run::step>::iterator prefixStepIterator { twaRun->prefix.begin() };
        ++prefixStepIterator; // skip dummy initial state

        const spot::state* initialState { prefixStepIterator->s };
        const unsigned initialStateNumber { synchronousProduct->twa()->state_number(initialState) };
        assert(synchronousProduct->isInitialState(initialStateNumber));

        PowersetConstSharedPtr initialStatePoints { synchronousProduct->points(initialStateNumber) };
        PPLUtils::fusion(*result, *initialStatePoints);

        emptinessCheckResult = explicitSe05Search->check();
    }

    return result;
}