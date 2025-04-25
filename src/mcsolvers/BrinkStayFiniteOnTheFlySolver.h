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
        const bool concurrent = false,
        const BrinkSemantics brinkSemantics = BrinkSemantics::may,
        const bool discretiseRtlfDirectToLtl = false,
        const bool collectPaths = false,
        const std::string_view solverName = "BrinkStayFiniteOnTheFlySolver"
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent, discretiseRtlfDirectToLtl, collectPaths, solverName)
      , m_brinkSemantics { brinkSemantics }
    {
        PolyhedralSystemSharedPtr stayPolyhedralSystem { m_polyhedralSystem };
        PolyhedralSystemSharedPtr brinkPolyhedralSystem { std::make_shared<PolyhedralSystem>(*m_polyhedralSystem) };
        m_staySolver = std::make_shared<StayFiniteOnTheFlySolver>(stayPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent);
        m_brinkSolver = std::make_shared<BrinkFiniteOnTheFlySolver>(brinkPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent);
    }

    ~BrinkStayFiniteOnTheFlySolver() override = default;

    SolverResult run() override
    {
        Timer timer {};

        logPolyhedralSystemAndCollectStats();
        logRtlFormulaAndCollectStats();

        PolyhedralSystemSharedPtr stayPolyhedralSystem { m_polyhedralSystem };
        PolyhedralSystemSharedPtr brinkPolyhedralSystem { std::make_shared<PolyhedralSystem>(*m_polyhedralSystem) };

        StayFiniteOnTheFlySolver staySolver { stayPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent, m_discretiseRtlfDirectToLtl, m_collectPaths };
        BrinkFiniteOnTheFlySolver brinkSolver { brinkPolyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation, m_concurrent, m_brinkSemantics, m_discretiseRtlfDirectToLtl, m_collectPaths };

        SolverResult staySolverResult { staySolver.run() };
        SolverResult brinkSolverResult { brinkSolver.run() };

        const FiniteOnTheFlySolverStats& staySolverStats { staySolver.stats() };
        const FiniteOnTheFlySolverStats& brinkSolverStats { brinkSolver.stats() };
        FiniteOnTheFlySolverStats brinkStayMergedStats { staySolverStats.merge(brinkSolverStats) };
        m_finiteOnTheFlySolverStats->merge(brinkStayMergedStats);

        PowersetConstSharedPtr stayResult { staySolverResult.result() };
        PowersetSharedPtr result { PPLUtils::fusion(*stayResult, *brinkSolverResult.result()) };
        m_finiteOnTheFlySolverStats->addExecutionTime(timer.elapsedInSeconds());

        return SolverResult { staySolverResult.isIncompleteResult() || brinkSolverResult.isIncompleteResult(), result };
    }

protected:
    std::shared_ptr<StayFiniteOnTheFlySolver> m_staySolver {};
    BrinkSemantics m_brinkSemantics {};
    std::shared_ptr<BrinkFiniteOnTheFlySolver> m_brinkSolver {};

    void logPolyhedralSystemAndCollectStats() override
    {
        Log::log(Verbosity::verbose, "[{} - Original Polyhedral System]\n{}", name(), *m_polyhedralSystem);
        m_solverStats->addPolyhedralSystemStats(collectPolyhedralSystemStats(*m_polyhedralSystem));
    }

    void logRtlFormulaAndCollectStats() override
    {
        Log::log(Verbosity::verbose, "[{} - Original RTL Formula] Formula: {}.", name(), m_rtlFormula);
        RtlFormulaStats rtlFormulaStats { collectRtlStats(m_rtlFormula) };
        m_solverStats->addRtlFormulaStats(rtlFormulaStats);
        Log::log(Verbosity::verbose, "[{} - Original RTL Formula] Total atomic propositions: {}.", name(), rtlFormulaStats.getTotalAtomicPropositions());
        Log::log(Verbosity::verbose, "[{} - Original RTL Formula] Length: {}.\n", name(), rtlFormulaStats.getLength());
    }
};