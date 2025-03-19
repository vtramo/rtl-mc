#pragma once

#include "PolyhedralFiniteLtlAutomaton.h"
#include "PolyhedralBuchiLtlAutomaton.h"
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

inline PolyhedralFiniteLtlAutomatonConstSharedPtr buildPolyhedralFiniteLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel { toSpotAutomatonOptimizationLevel(optimizationFlags) };
    return PolyhedralFiniteLtlAutomaton::buildAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    );
}

inline PolyhedralBuchiLtlAutomatonConstSharedPtr buildPolyhedralBuchiLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel { toSpotAutomatonOptimizationLevel(optimizationFlags) };
    return PolyhedralBuchiLtlAutomaton::buildAutomaton(
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