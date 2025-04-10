#pragma once

#include <PolyhedralSystem.h>

#include "ppl_aliases.h"
#include "ClosedInterval.h"

/**
 * \brief Constructs a polyhedral system modeling the tank experiment.
 *
 * This system models a set of tanks connected in sequence, with atomic propositions `p` and `q`
 * indicating whether the sum of odd-indexed tanks is greater than the sum of even-indexed tanks (plus a gap),
 * and vice versa. If `maxTime > 0`, a clock variable `t` with derivative 1 is included along with two atomic
 * propositions:
 *
 * - `t0` ≡ { t = 0 }
 * - `t1` ≡ { t ≤ maxTime }
 *
 * These propositions can be used in RTL formulas to express timing constraints. Setting `maxTime = 0` disables the clock (default).
 *
 * \param totalTanks Number of tanks in the system (must be ≥ 2).
 * \param gapThickness Minimum difference required between the sums of even- and odd-indexed tank variables (must be ≥ 1).
 * \param maxTime Maximum value for the clock variable `t` (0 disables time).
 * \param inPumpInterval Interval of inflow rates for the first tank.
 * \param transferPumpInterval Interval of flow rates for internal tank-to-tank transfers.
 * \param outPumpInterval Interval of outflow rates for the last tank.
 * \return A fully constructed PolyhedralSystem representing the experiment.
 * \throws std::invalid_argument if `gapThickness < 1`.
 */
PolyhedralSystem gap(
    unsigned totalTanks,
    unsigned gapThickness = 1,
    unsigned maxTime = 0,
    ClosedInterval<int> inPumpInterval = { 1, 2 },
    ClosedInterval<int> transferPumpInterval = { 1, 2 },
    ClosedInterval<int> outPumpInterval = { 0, 3 }
);

/**
 * \brief Constructs the polyhedral flow system representing the differential constraints of the tank experiment.
 * \param totalTanks Number of tanks (must be ≥ 2).
 * \param includeClock If true, adds a clock variable with derivative 1.
 * \param inPumpInterval Flow interval into the first tank.
 * \param transferPumpInterval Flow interval between internal tanks.
 * \param outPumpInterval Flow interval out of the last tank.
 * \return A Poly object representing the flow constraints of the system.
 * \throws std::invalid_argument if `totalTanks < 2`.
 */
Poly tankExperimentFlow(
    unsigned totalTanks,
    bool includeClock = true,
    ClosedInterval<int> inPumpInterval = { 1, 2 },
    ClosedInterval<int> transferPumpInterval = { 1, 2 },
    ClosedInterval<int> outPumpInterval = { 0, 3 }
);