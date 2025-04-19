#pragma once

#include "OmnidirectionalSolver.h"
#include "Timer.h"
#include "automata_builder.h"
#include "emptiness_algorithms.h"

class OmnidirectionalInfiniteSolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalInfiniteSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {}

    ~OmnidirectionalInfiniteSolver() override = default;

    SolverResult run() override
    {
        preprocessPolyhedralSystem();
        logPolyhedralSystemAndCollectStats();

        preprocessRtlFormula();
        logRtlFormulaAndCollectStats();

        const double discretisationExecutionTimeSeconds { discretiseRtlFormula() };
        logAndCollectDiscretisationStats(discretisationExecutionTimeSeconds);

        constructPolyhedralLtlAutomaton();
        constructPolyhedralAbstraction();
        constructSynchronousProductAutomaton();

        return SolverResult { false, runEmptinessCheckDenotationSearch() };
    }
protected:
    double discretiseRtlFormula() override
    {
        Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
        Timer timer {};
        m_discreteLtlFormula = DiscreteLtlFormula::discretiseRtl(std::move(m_rtlFormula));
        const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
        Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
        return discretisationExecutionTimeSeconds;
    }

    void constructPolyhedralLtlAutomaton() override
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { m_polyhedralSystem };
        m_ltlAutomaton = buildPolyhedralLtlBüchiAutomaton(
            std::move(m_discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            m_automatonOptimizationFlags
        );
        const PolyhedralLtlAutomatonStats& automatonStats { m_ltlAutomaton->stats() };
        m_solverStats->addAutomatonStats(automatonStats);
    }

    PowersetSharedPtr runEmptinessCheckDenotationSearch()
    {
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Started.");
        Timer timer {};

        std::unordered_set initialStatesWithAcceptingRuns { collectInitialStatesWithAcceptingRuns(*m_polyhedralSynchronousProduct) };
        PowersetSharedPtr result { std::make_shared<Powerset>(m_polyhedralSynchronousProduct->spaceDimension(), PPL::EMPTY) };
        for (unsigned initialState: initialStatesWithAcceptingRuns)
        {
            PPLUtils::fusion(*result, *m_polyhedralSynchronousProduct->points(initialState));
        }

        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Completed. Elapsed time: {} s.", timer.elapsedInSeconds());
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Total collected initial states: {}.", initialStatesWithAcceptingRuns.size());
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Collected initial states: {}.", fmt::join(initialStatesWithAcceptingRuns, ", "));
        return result;
    }
};