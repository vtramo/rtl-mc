#pragma once

#include "PolyhedralSystem.h"
#include "OmnidirectionalPolyhedralAbstraction.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralFiniteLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "PolyhedralSynchronousProductAutomaton.h"
#include "GeneralPolyhedralAbstraction.h"
#include "FiniteSemanticsDfs.h"
#include "automata_builder.h"

inline PowersetSharedPtr omnidirectionalFiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    auto abstraction { std::make_shared<OmnidirectionalPolyhedralAbstraction>(polyhedralSystem) };
    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralFiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildPolyhedralFiniteLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };

    auto polyhedralSynchronousProduct { std::make_shared<PolyhedralSynchronousProductAutomaton>(finiteLtlAutomaton, abstraction) };
    FiniteSemanticsDfs dfs { polyhedralSynchronousProduct };
    return dfs.run();
}