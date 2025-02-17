#pragma once

#include "PolyhedralSystemStats.h"
#include "PolyhedralSystem.h"
#include "RtlFormulaStats.h"
#include "DiscretisationStats.h"
#include "Denot.h"
#include "DenotStats.h"
#include <spot/tl/length.hh>

inline PolyhedralSystemStats collectPolyhedralSystemStats(const PolyhedralSystem& polyhedralSystem)
{
    return {
        polyhedralSystem.totalAtoms(),
        static_cast<int>(polyhedralSystem.spaceDimension())
    };
}

inline RtlFormulaStats collectRtlfStats(const spot::formula& rtlf)
{
    return {
        static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(rtlf))->size()),
        spot::length(rtlf)
    };
}

inline DiscretisationStats collectDiscretisationStats(const DiscreteLtlFormula& discreteLtlFormula, const double executionTimeSeconds)
{
    return {
        static_cast<int>(std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(discreteLtlFormula.formula()))->size()),
        spot::length(discreteLtlFormula.formula()),
        executionTimeSeconds
    };
}

template<typename D>
DenotStats collectDenotStats(const D& denot, const double executionTimeSeconds)
{
    return {
        denot.totalIterations(),
        executionTimeSeconds
    };
}
