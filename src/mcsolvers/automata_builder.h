#pragma once

#include "PolyhedralFiniteLtlAutomaton.h"
#include "PolyhedralBuchiLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"

inline spot::postprocessor::optimization_level toSpotAutomatonOptimizationLevel(const AutomatonOptimizationFlags& optimizationFlags)
{
    if (optimizationFlags.low) return spot::postprocessor::optimization_level::Low;
    if (optimizationFlags.medium) return spot::postprocessor::optimization_level::Medium;
    return spot::postprocessor::optimization_level::High;
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