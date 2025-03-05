#pragma once

#include "BackwardNFA.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystem.h"
#include "Denot.h"
#include "RtlMcProgram.h"


BackwardNFAConstSharedPtr buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
);

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    BackwardNFAConstSharedPtr backwardNfa
);

int onlyOnTheFlyDenotInterface(int argc, char *argv[]);

BackwardNFAConstSharedPtr buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
);

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    BackwardNFAConstSharedPtr backwardNfa
);