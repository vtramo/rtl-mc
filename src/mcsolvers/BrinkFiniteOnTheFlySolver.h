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
        const bool concurrent = false,
        const BrinkSemantics brinkSemantics = BrinkSemantics::may,
        const bool discretiseRtlfDirectToLtl = false
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent, discretiseRtlfDirectToLtl)
      , m_brinkSemantics { brinkSemantics }
    {}

    ~BrinkFiniteOnTheFlySolver() override = default;
protected:
    BrinkSemantics m_brinkSemantics {};

    void preprocessPolyhedralSystem() override
    {
        const auto& [brinkAtom, brinkInterpretation]
        {
            m_brinkSemantics == BrinkSemantics::may
                ? brinkMay(m_polyhedralSystem)
                : brinkMust(m_polyhedralSystem)
        };

        m_polyhedralSystem = m_polyhedralSystem->extend(brinkAtom, *brinkInterpretation);
    }

    void preprocessRtlFormula() override
    {
        m_rtlFormula = eventuallyBrinkAndLast(m_rtlFormula);
        Solver::preprocessRtlFormula();
    }
};