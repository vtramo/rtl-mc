#pragma once

#include "OmnidirectionalSolver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalFiniteSolver.h"
#include "brink_stay_atoms.h"

class OmnidirectionalMaySolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalMaySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    ~OmnidirectionalMaySolver() override = default;

    PowersetSharedPtr run() override
    {
        OmnidirectionalInfiniteSolver infiniteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        PowersetConstSharedPtr infiniteResult { infiniteSolver.run() };

        preprocessPolyhedralSystem();
        preprocessRtlFormula();

        OmnidirectionalFiniteSolver finiteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        PowersetConstSharedPtr finiteResult { finiteSolver.run() };

        return PPLUtils::fusion(*finiteResult, *infiniteResult);
    }
protected:
    void preprocessRtlFormula() override
    {
        m_rtlFormula = eventuallyBrinkAndLast(m_rtlFormula);
    }

    double discretiseRtlFormula() override
    {
        return 0;
    }

    void preprocessPolyhedralSystem() override
    {
        addBrinkAtomInPolyhedralSystem();
    }

    virtual void addBrinkAtomInPolyhedralSystem()
    {
        const auto& [brinkAtom, brinkInterpretation] { brinkMay(m_polyhedralSystem) };
        m_polyhedralSystem->addAtomInterpretation(brinkAtom, *brinkInterpretation);
    }

    void constructPolyhedralLtlAutomaton() override {}
};