#pragma once

#include "ExplicitSolver.h"
#include "GeneralPolyhedralAbstraction.h"

class GeneralSolver: public ExplicitSolver
{
public:
    GeneralSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const std::string_view solverName = "GeneralSolver"
    )
      : ExplicitSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, solverName)
    {}

    ~GeneralSolver() override = default;

protected:

    void constructPolyhedralAbstraction() override
    {
        std::vector observables { m_polyhedralSystem->getOrGenerateObservables() };
        const unsigned sufficientHorizon { 2 * m_ltlAutomaton->totalStates() * maxPatches(observables) };
        m_polyhedralAbstraction = std::make_shared<GeneralPolyhedralAbstraction>(m_polyhedralSystem, std::move(observables), sufficientHorizon);
    }

    virtual int maxPatches(const std::vector<Observable>& observables)
    {
        int maxPatches {};

        for (const Observable& observable: observables)
        {
            maxPatches = std::max(maxPatches, observable.totalPatches());
        }

        return maxPatches;
    }
};