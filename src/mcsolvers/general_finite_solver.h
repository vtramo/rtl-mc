#pragma once

#include "Observable.h"
#include "PolyhedralSystem.h"
#include "AutomatonOptimizationFlags.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralFiniteLtlAutomaton.h"
#include "automata_builder.h"
#include "GeneralPolyhedralAbstraction.h"

inline int maxPatches(const std::vector<Observable>& observables)
{
    int maxPatches {};

    for (const Observable& observable: observables)
    {
        maxPatches = std::max(maxPatches, observable.totalPatches());
    }

    return maxPatches;
}

inline PowersetSharedPtr generalFiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralFiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildPolyhedralFiniteLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };
    std::vector observables { polyhedralSystem->generateObservables() };
    const unsigned sufficientHorizon { 2 * finiteLtlAutomaton->totalStates() * maxPatches(observables) };
    GeneralPolyhedralAbstraction generalPolyhedralAbstraction {
        polyhedralSystem,
        std::move(observables),
        static_cast<int>(sufficientHorizon)
    };

    return std::make_shared<Powerset>(PPL::EMPTY);
}