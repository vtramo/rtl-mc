#pragma once

#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralBuchiLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "automata_builder.h"

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
    buchiLtlAutomaton->printDotFormat(std::cout);
    return {};
}