#pragma once

#include "OmnidirectionalPolyhedralAbstraction.h"
#include "GeneralSolver.h"
#include "automata_builder.h"
#include "FiniteSemanticsDfs.h"
#include "Timer.h"

class GeneralFiniteSolver: public GeneralSolver
{
public:
    GeneralFiniteSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        AutomatonOptimizationFlags automatonOptimizationFlags,
        bool universalDenotation = false
    )
      : GeneralSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    virtual ~GeneralFiniteSolver() = default;

    PowersetSharedPtr run() override
    {
        preprocessPolyhedralSystem();
        logPolyhedralSystemAndCollectStats();

        preprocessRtlFormula();
        logRtlFormulaAndCollectStats();

        constructPolyhedralLtlAutomaton();
        constructPolyhedralAbstraction();
        constructSynchronousProductAutomaton();

        const double discretisationExecutionTimeSeconds { discretiseRtlFormula() };
        logAndCollectDiscretisationStats(discretisationExecutionTimeSeconds);

        return runFiniteSemanticsDfs();
    }
protected:

    double discretiseRtlFormula() override
    {
        Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
        Timer timer {};
        m_discreteLtlFormula = DiscreteLtlFormula::discretiseFromFiniteLtl(std::move(m_rtlFormula));
        const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
        Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
        return discretisationExecutionTimeSeconds;
    }

    void constructPolyhedralLtlAutomaton() override
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { m_polyhedralSystem };
        m_ltlAutomaton = buildPolyhedralFiniteLtlAutomaton(
            std::move(m_discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            m_automatonOptimizationFlags
        );
    }

    virtual PowersetSharedPtr runFiniteSemanticsDfs()
    {
        FiniteSemanticsDfs dfs { m_polyhedralSynchronousProduct };
        return dfs.run();
    }
};