#pragma once

#include "OmnidirectionalSolver.h"
#include "Timer.h"
#include "automata_builder.h"
#include "infinite_semantics_emptiness.h"
#include <spot/twaalgos/se05.hh>

class OmnidirectionalInfiniteSolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalInfiniteSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        AutomatonOptimizationFlags automatonOptimizationFlags,
        bool universalDenotation = false
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

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

        return explicitSe05Search(m_polyhedralSynchronousProduct);
    }
protected:

    double discretiseRtlFormula() override
    {
        Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
        Timer timer {};
        m_discreteLtlFormula = DiscreteLtlFormula::discretiseInfinite(std::move(m_rtlFormula));
        const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
        Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
        return discretisationExecutionTimeSeconds;
    }

    void constructPolyhedralLtlAutomaton() override
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { m_polyhedralSystem };
        m_ltlAutomaton = buildPolyhedralBuchiLtlAutomaton(
            std::move(m_discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            m_automatonOptimizationFlags
        );
    }
};