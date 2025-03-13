#pragma once

#include "FiniteOnTheFlySolver.h"
#include "BrinkFiniteOnTheFlySolver.h"
#include "StayFiniteOnTheFlySolver.h"
#include "brink_stay_atoms.h"
#include "ppl_utils.h"

class BrinkStayFiniteOnTheFlySolver: public FiniteOnTheFlySolver
{
public:
    BrinkStayFiniteOnTheFlySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const bool concurrent = false
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent)
    {
        PolyhedralSystemSharedPtr stayPolyhedralSystem { m_polyhedralSystem };
        PolyhedralSystemSharedPtr brinkPolyhedralSystem { std::make_shared<PolyhedralSystem>(*m_polyhedralSystem) };
        m_staySolver = std::make_shared<StayFiniteOnTheFlySolver>(stayPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent);
        m_brinkSolver = std::make_shared<BrinkFiniteOnTheFlySolver>(brinkPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent);
    }

    ~BrinkStayFiniteOnTheFlySolver() override = default;

    PowersetSharedPtr run() override
    {
        PolyhedralSystemSharedPtr stayPolyhedralSystem { m_polyhedralSystem };
        PolyhedralSystemSharedPtr brinkPolyhedralSystem { std::make_shared<PolyhedralSystem>(*m_polyhedralSystem) };

        StayFiniteOnTheFlySolver staySolver { stayPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent };
        BrinkFiniteOnTheFlySolver brinkSolver { brinkPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent };

        PowersetConstSharedPtr stayResult { staySolver.run() };
        PowersetConstSharedPtr brinkResult { brinkSolver.run() };

        return PPLUtils::fusion(*stayResult, *brinkResult);
    }
protected:
    std::shared_ptr<StayFiniteOnTheFlySolver> m_staySolver {};
    std::shared_ptr<BrinkFiniteOnTheFlySolver> m_brinkSolver {};
};