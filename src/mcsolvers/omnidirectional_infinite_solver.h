#pragma once

#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "FiniteLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "automata_builder.h"

inline int maxPatches(const std::vector<Observable>& observables)
{
    int maxPatches {};

    for (const Observable& observable: observables)
    {
        maxPatches = std::max(maxPatches, observable.totalPatches());
    }

    return maxPatches;
}

inline PowersetSharedPtr generalInfiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    FiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildFiniteLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };
    return {};
}