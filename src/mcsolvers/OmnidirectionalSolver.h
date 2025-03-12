#pragma once

#include "ExplicitSolver.h"
#include "OmnidirectionalPolyhedralAbstraction.h"

class OmnidirectionalSolver: public ExplicitSolver
{
public:
    OmnidirectionalSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : ExplicitSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    ~OmnidirectionalSolver() override = default;
protected:

    void constructPolyhedralAbstraction() override
    {
        m_polyhedralAbstraction = std::make_shared<OmnidirectionalPolyhedralAbstraction>(m_polyhedralSystem);
    }
};