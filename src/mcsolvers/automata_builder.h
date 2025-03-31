#pragma once

#include "PolyhedralLtlFiniteAutomaton.h"
#include "PolyhedralLtlBüchiAutomaton.h"
#include "AutomatonOptimization.h"

inline spot::postprocessor::optimization_level toSpotAutomatonOptimizationLevel(const AutomatonOptimizationFlags& optimizationFlags)
{
    switch (optimizationFlags.optimizationLevel)
    {
    case low:
        return spot::postprocessor::optimization_level::Low;
    case medium:
        return spot::postprocessor::optimization_level::Medium;
    case high:
        return spot::postprocessor::optimization_level::High;
    default:
        throw std::invalid_argument("Invalid automaton optimization level");
    }
}

inline PolyhedralLtlFiniteAutomatonConstSharedPtr buildPolyhedralLtlFiniteAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel { toSpotAutomatonOptimizationLevel(optimizationFlags) };
    return PolyhedralLtlFiniteAutomaton::buildAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    );
}

inline PolyhedralLtlBüchiAutomatonConstSharedPtr buildPolyhedralLtlBüchiAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel { toSpotAutomatonOptimizationLevel(optimizationFlags) };
    return PolyhedralLtlBüchiAutomaton::buildAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    );
}

inline BackwardNFAConstSharedPtr buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel { toSpotAutomatonOptimizationLevel(optimizationFlags) };
    return BackwardNFA::buildAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    );
}