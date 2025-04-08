#include "tank_experiment.h"
#include "Tank.h"
#include "formula.h"
#include "ppl_utils.h"

PolyhedralSystem gap(
    const unsigned totalTanks,
    bool includeClock,
    const ClosedInterval<int> inPumpInterval,
    const ClosedInterval<int> transferPumpInterval,
    const ClosedInterval<int> outPumpInterval
)
{
    Poly flow { tankExperimentFlow(totalTanks, includeClock, inPumpInterval, transferPumpInterval, outPumpInterval) };

    PPL::dimension_type spaceDimension { includeClock ? totalTanks + 1 : totalTanks };
    PPL::Constraint_System constraintSystemInvariant {};
    constraintSystemInvariant.set_space_dimension(spaceDimension);
    PolyhedralSystemSymbolTable symbolTable {};
    char variableName { 'a' };
    for (unsigned i { 0 }; i < totalTanks; ++i)
    {
        constraintSystemInvariant.insert(PPL::Variable { i } >= 0);
        symbolTable.addVariable(std::string { 1, variableName });
        ++variableName;
    }
    Powerset invariant { Poly { constraintSystemInvariant }};
    if (includeClock)
    {
        symbolTable.addVariable("t");
    }

    symbolTable.addAtom("p");
    symbolTable.addAtom("q");
    PPL::Variable firstTankVariable { 0 };
    PPL::Variable lastTankVariable { totalTanks - 1 };
    return PolyhedralSystem::builder()
        .flow(flow)
        .invariant(invariant)
        .denotation({
            { ap("p"), PPLUtils::powerset({{ lastTankVariable >= firstTankVariable + 1 }}, spaceDimension) },
            { ap("q"), PPLUtils::powerset({{ firstTankVariable >= lastTankVariable + 1 }}, spaceDimension) }
        }).symbolTable(symbolTable)
        .build();
}

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