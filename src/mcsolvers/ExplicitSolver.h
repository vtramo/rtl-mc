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
        const TranslationOptimizationFlags translationOptimizationFlags,
        const bool universalDenotation = false,
        const std::string_view solverName = "ExplicitSolver"
    )
      : Solver(polyhedralSystem, rtlFormula, translationOptimizationFlags, universalDenotation, solverName)
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