#pragma once

#include "PolyhedralSystem.h"
#include "AutomatonOptimizationFlags.h"
#include "omnidirectional_infinite_solver.h"
#include "omnidirectional_finite_solver.h"
#include "spot_utils.h"
#include "reach.h"
#include "ppl_utils.h"

using namespace SpotUtils;

inline PowersetSharedPtr omnidirectionalMaySemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    PowersetConstSharedPtr resultInfinite { omnidirectionalInfiniteTimeSemanticsSolver(polyhedralSystem, rtlFormula, optimizationFlags) };

    spot::formula brink { ap("brink") };
    const Powerset& invariant { polyhedralSystem->invariant() };
    PowersetUniquePtr invariantComplement { PPLUtils::complement(invariant) };
    PowersetUniquePtr brinkInterpretation { reach0(invariant, *invariantComplement, polyhedralSystem->preFlow()) };
    polyhedralSystem->addAtomInterpretation(brink, *brinkInterpretation);

    spot::formula brinkRtlFormula { And({ std::move(rtlFormula), F(And({ brink, ap("last") }))}) };
    PowersetConstSharedPtr resultFinite { omnidirectionalFiniteTimeSemanticsSolver(polyhedralSystem, brinkRtlFormula, optimizationFlags) };

    return PPLUtils::fusion(*resultInfinite, *resultFinite);
}