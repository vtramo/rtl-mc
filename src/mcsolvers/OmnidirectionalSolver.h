#pragma once

#include "ExplicitSolver.h"
#include "OmnidirectionalPolyhedralAbstraction.h"

class OmnidirectionalSolver: public ExplicitSolver
{
public:
    OmnidirectionalSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        AutomatonOptimizationFlags automatonOptimizationFlags,
        bool universalDenotation = false
    )
      : ExplicitSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

protected:

    void constructPolyhedralAbstraction() override
    {
        m_polyhedralAbstraction = std::make_shared<OmnidirectionalPolyhedralAbstraction>(m_polyhedralSystem);
    }
};