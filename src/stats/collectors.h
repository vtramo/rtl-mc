#pragma once

#include "PolyhedralSystemStats.h"
#include "PolyhedralSystem.h"
#include "RtlFormulaStats.h"
#include "DiscretizationStats.h"
#include "Denot.h"
#include "DenotStats.h"
#include <spot/tl/length.hh>

inline PolyhedralSystemStats collectPolyhedralSystemStats(const PolyhedralSystem& polyhedralSystem)
{
    return {
        polyhedralSystem.getTotalAtoms(),
        static_cast<int>(polyhedralSystem.getSpaceDimension())
    };
}

inline RtlFormulaStats collectRtlfStats(const spot::formula& rtlf)
{
    return {
        static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(rtlf))->size()),
        spot::length(rtlf)
    };
}

inline DiscretizationStats collectDiscretizationStats(const DiscreteLtlFormula& discreteLtlFormula, const double executionTimeSeconds)
{
    return {
        static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(discreteLtlFormula.formula()))->size()),
        spot::length(discreteLtlFormula.formula()),
        executionTimeSeconds
    };
}

inline DenotStats collectDenotStats(const Denot& denot, const double executionTimeSeconds)
{
    return {
        denot.totalIterations(),
        executionTimeSeconds
    };
}
