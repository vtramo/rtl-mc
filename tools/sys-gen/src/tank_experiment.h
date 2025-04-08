#pragma once

#include <PolyhedralSystem.h>

#include "ppl_aliases.h"
#include "ClosedInterval.h"

PolyhedralSystem gap(
    unsigned totalTanks,
    bool includeClock = true,
    ClosedInterval<int> inPumpInterval = { 1, 2 },
    ClosedInterval<int> transferPumpInterval = { 1, 2 },
    ClosedInterval<int> outPumpInterval = { 0, 3 }
);

Poly tankExperimentFlow(
    unsigned totalTanks,
    bool includeClock = true,
    ClosedInterval<int> inPumpInterval = { 1, 2 },
    ClosedInterval<int> transferPumpInterval = { 1, 2 },
    ClosedInterval<int> outPumpInterval = { 0, 3 }
);