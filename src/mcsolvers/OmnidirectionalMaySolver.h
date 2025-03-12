#pragma once

#include "OmnidirectionalSolver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalFiniteSolver.h"
#include "reach.h"

class OmnidirectionalMaySolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalMaySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        AutomatonOptimizationFlags automatonOptimizationFlags,
        bool universalDenotation = false
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    PowersetSharedPtr run() override
    {
        OmnidirectionalInfiniteSolver infiniteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        PowersetConstSharedPtr infiniteResult { infiniteSolver.run() };

        preprocessPolyhedralSystem();

        OmnidirectionalFiniteSolver finiteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        PowersetConstSharedPtr finiteResult { finiteSolver.run() };

        return PPLUtils::fusion(*finiteResult, *infiniteResult);
    }
protected:
    void preprocessRtlFormula() override
    {

    }

    void preprocessPolyhedralSystem() override
    {
        addBrinkAtomInPolyhedralSystem();
    }

    virtual void addBrinkAtomInPolyhedralSystem()
    {
        spot::formula brink { ap("brink") };
        const Powerset& invariant { m_polyhedralSystem->invariant() };
        PowersetUniquePtr invariantComplement { PPLUtils::complement(invariant) };
        PowersetUniquePtr brinkInterpretation { reach0(invariant, *invariantComplement, m_polyhedralSystem->preFlow()) };
        m_polyhedralSystem->addAtomInterpretation(brink, *brinkInterpretation);
    }
};