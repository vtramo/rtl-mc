#pragma once

#include "FiniteOnTheFlySolver.h"
#include "brink_stay_atoms.h"

class BrinkFiniteOnTheFlySolver: public FiniteOnTheFlySolver
{
public:
    BrinkFiniteOnTheFlySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const bool concurrent = false
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent)
    {}

    ~BrinkFiniteOnTheFlySolver() override = default;
protected:

    void preprocessPolyhedralSystem() override
    {
        const auto& [brinkAtom, brinkInterpretation] { brink(m_polyhedralSystem) };
        m_polyhedralSystem->addAtomInterpretation(brinkAtom, *brinkInterpretation);
    }

    void preprocessRtlFormula() override
    {
        m_rtlFormula = eventuallyBrinkAndLast(m_rtlFormula);
        Solver::preprocessRtlFormula();
    }
};