#pragma once

#include "Solver.h"
#include "PolyhedralAbstraction.h"
#include "PolyhedralLtlAutomaton.h"
#include "PolyhedralSynchronousProductAutomaton.h"

class ExplicitSolver: public Solver
{
public:
    ExplicitSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : Solver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    ~ExplicitSolver() override = default;

protected:
    PolyhedralAbstractionSharedPtr m_polyhedralAbstraction {};
    PolyhedralLtlAutomatonSharedPtr m_ltlAutomaton {};
    PolyhedralSynchronousProductAutomatonSharedPtr m_polyhedralSynchronousProduct {};

    virtual void constructPolyhedralAbstraction() = 0;
    virtual void constructPolyhedralLtlAutomaton() = 0;
    virtual void constructSynchronousProductAutomaton()
    {
        m_polyhedralSynchronousProduct = std::make_shared<PolyhedralSynchronousProductAutomaton>(m_ltlAutomaton, m_polyhedralAbstraction);
    }

};