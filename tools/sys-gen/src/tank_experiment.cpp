#include "tank_experiment.h"
#include "Tank.h"
#include "formula.h"
#include "ppl_utils.h"

/**
 * This system models a set of tanks connected in sequence, with atomic propositions `p` and `q`
 * indicating whether the sum of odd-indexed tanks is greater than the sum of even-indexed tanks (plus a gap),
 * and vice versa. If `maxTime > 0`, a clock variable `t` with derivative 1 is included along with two atomic
 * propositions:
 *
 * - `t0` ≡ { t = 0 }
 * - `t1` ≡ { t ≤ maxTime }
 *
 * These propositions can be used in RTL formulas to express timing constraints. Setting `maxTime = 0` disables the clock (default).
 */
PolyhedralSystem gap(
    const unsigned totalTanks,
    const unsigned gapThickness,
    const unsigned maxTime,
    const ClosedInterval<int> inPumpInterval,
    const ClosedInterval<int> transferPumpInterval,
    const ClosedInterval<int> outPumpInterval
)
{
    if (gapThickness < 1)
    {
        throw std::invalid_argument("Gap thickness must be at least 1!");
    }

    const bool includeClock { maxTime != 0 };
    Poly flow { tankExperimentFlow(totalTanks, includeClock, inPumpInterval, transferPumpInterval, outPumpInterval) };

    PPL::dimension_type spaceDimension { maxTime ? totalTanks + 1 : totalTanks };
    PPL::Constraint_System constraintSystemInvariant {};
    constraintSystemInvariant.set_space_dimension(spaceDimension);
    PolyhedralSystemSymbolTable symbolTable {};
    char variableName { 'a' };
    PPL::Linear_Expression evenTankVariablesSum {};
    PPL::Linear_Expression oddTankVariablesSum {};
    for (unsigned i { 0 }; i < totalTanks; ++i)
    {
        PPL::Variable tankVariable { i };
        constraintSystemInvariant.insert(tankVariable >= 0);
        symbolTable.addVariable(std::string { variableName++ });
        if (i % 2 == 0) evenTankVariablesSum += tankVariable;
        else oddTankVariablesSum += tankVariable;
    }

    symbolTable.addAtom("p");
    symbolTable.addAtom("q");
    std::unordered_map<spot::formula, Powerset> denotation {
        {
            { ap("p"), PPLUtils::powerset({{ oddTankVariablesSum >= evenTankVariablesSum + gapThickness }}, spaceDimension) },
            { ap("q"), PPLUtils::powerset({{ evenTankVariablesSum >= oddTankVariablesSum + gapThickness }}, spaceDimension) },
        }
    };

    if (includeClock)
    {
        PPL::Variable t { totalTanks };
        symbolTable.addVariable("t");
        symbolTable.addAtoms({ "t0", "t1" });
        denotation.insert({ ap("t0"), PPLUtils::powerset({{ t == 0 }}, spaceDimension) });
        denotation.insert({ ap("t1"), PPLUtils::powerset({{ t <= maxTime }}, spaceDimension) });
    }
    Powerset invariant { Poly { constraintSystemInvariant }};
    return PolyhedralSystem::builder()
        .flow(flow)
        .invariant(invariant)
        .denotation(denotation)
        .symbolTable(symbolTable)
        .build();
}

/**
 * Each tank contributes a set of constraints based on its input and output pump intervals. Additional window-based
 * constraints are added to model the conservation of flow across contiguous subchains of tanks. Optionally includes
 * a clock variable `t` with constant derivative 1 if `includeClock` is true.
 */
Poly tankExperimentFlow(
    const unsigned totalTanks,
    bool includeClock,
    const ClosedInterval<int> inPumpInterval,
    const ClosedInterval<int> transferPumpInterval,
    const ClosedInterval<int> outPumpInterval
)
{
    if (totalTanks < 2)
    {
        throw std::invalid_argument("Total tanks must be at least 2");
    }

    std::vector<Tank> tanks {};
    tanks.reserve(totalTanks);

    PPL::Constraint_System constraints {};
    for (unsigned i { 0 }; i < totalTanks; ++i)
    {
        Tank tank {};

        if (i == 0)
        {
            tank = { PPL::Variable { i }, inPumpInterval, transferPumpInterval };
        }
        else if (i == totalTanks - 1)
        {
            tank = { PPL::Variable { i }, transferPumpInterval, outPumpInterval };
        }
        else
        {
            tank = { PPL::Variable { i }, transferPumpInterval, transferPumpInterval };
        }

        tanks.emplace_back(tank);
        for (const PPL::Constraint& tankConstraint: tank.variabilityConstraints())
        {
            constraints.insert(tankConstraint);
        }
    }


    for (unsigned i { 0 }; i < totalTanks; ++i)
    {
        PPL::Linear_Expression currentSumExpression {};

        for (unsigned k { i }; k < totalTanks; ++k)
        {
            unsigned int window { k - i + 1 };
            if (window < 2) continue;

            const Tank& firstTank { tanks[i] };
            const Tank& lastTank { tanks[k] };

            const ClosedInterval<int>& inputPumpInterval { firstTank.inputPumpInterval() };
            const ClosedInterval<int>& outputPumpInterval { lastTank.outputPumpInterval() };
            const int min { inputPumpInterval.min() - outputPumpInterval.max() };
            const int max { inputPumpInterval.max() - outputPumpInterval.min() };

            PPL::Linear_Expression expression {};
            for (unsigned j { i }; j <= k; ++j)
            {
                const Tank& tank { tanks[j] };
                const PPL::Variable& tankVariable { tank.variable() };
                expression += tankVariable;
            }

            PPL::Constraint minConstraint { expression >= min };
            PPL::Constraint maxConstraint { expression <= max };
            constraints.insert(minConstraint);
            constraints.insert(maxConstraint);
        }
    }

    if (includeClock)
    {
        constraints.insert(PPL::Variable { totalTanks } == 1);
    }

    PPL::Constraint_System constraintSystem { constraints };
    Poly flow { constraintSystem };
    return flow;
}