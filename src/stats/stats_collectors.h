#pragma once


#include "DenotOnTheFlyStats.h"
#include "PolyhedralSystemStats.h"
#include "PolyhedralSystem.h"
#include "RtlFormulaStats.h"
#include "DiscretisationStats.h"
#include "DiscreteLtlFormula.h"
#include "Denot.h"
#include <spot/tl/length.hh>

inline PolyhedralSystemStats collectPolyhedralSystemStats(const PolyhedralSystem& polyhedralSystem)
{
    PolyhedralSystemStats polyhedralSystemStats {};
    polyhedralSystemStats.setTotalAtomicPropositions(polyhedralSystem.totalAtoms());
    polyhedralSystemStats.setSpaceDimension(polyhedralSystem.spaceDimension());
    return polyhedralSystemStats;
}

inline RtlFormulaStats collectRtlStats(const spot::formula& rtl)
{
    RtlFormulaStats rtlStats {};
    rtlStats.setTotalAtomicPropositions(static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(rtl))->size()));
    rtlStats.setLength(spot::length(rtl));
    return rtlStats;
}

inline DiscretisationStats collectDiscretisationStats(const DiscreteLtlFormula& discreteLtlFormula, const double executionTimeSeconds)
{
    DiscretisationStats discretisationStats {};
    discretisationStats.setDiscreteLtlFormulaTotalAtomicPropositions(static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(discreteLtlFormula.formula()))->size()));
    discretisationStats.setDiscreteLtlFormulaLength(spot::length(discreteLtlFormula.formula()));
    discretisationStats.setDiscretisationExecutionTimeSeconds(executionTimeSeconds);
    return discretisationStats;
}

template<typename D>
DenotOnTheFlyStats collectDenotStats(const D& denot, const double executionTimeSeconds)
{
    DenotOnTheFlyStats denotStats {};
    denotStats.setTotalIterations(denot.totalIterations());
    denotStats.setExecutionTimeSeconds(executionTimeSeconds);
    return denotStats;
}
