#pragma once

#include "FiniteOnTheFlySolver.h"
#include "brink_stay_atoms.h"

class StayFiniteOnTheFlySolver: public FiniteOnTheFlySolver
{
public:
    StayFiniteOnTheFlySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const bool concurrent = false,
        const bool discretiseRtlfDirectToLtl = false
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent, discretiseRtlfDirectToLtl)
    {}

    ~StayFiniteOnTheFlySolver() override = default;
protected:

    void preprocessPolyhedralSystem() override
    {
        const auto& [stayAtom, stayInterpretation] { stay(m_polyhedralSystem) };
        m_polyhedralSystem = m_polyhedralSystem->extend(stayAtom, *stayInterpretation);
    }

    void preprocessRtlFormula() override
    {
        m_rtlFormula = eventuallyStayAndLast(m_rtlFormula);
        Solver::preprocessRtlFormula();
    }
};