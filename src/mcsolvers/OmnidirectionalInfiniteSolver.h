#pragma once

#include "OmnidirectionalSolver.h"
#include "Timer.h"
#include "automata_builder.h"
#include "infinite_semantics_emptiness.h"

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

    PowersetSharedPtr run() override
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

        return runEmptinessCheckDenotationSearch();
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
        EmptinessCheckDenotationResult emptinessCheckDenotationResult { emptinessCheckDenotationSearch(m_polyhedralSynchronousProduct) };
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Completed. Elapsed time: {} s.", emptinessCheckDenotationResult.elapsedTimeInSeconds);
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Total accepting runs: {}.", emptinessCheckDenotationResult.totalAcceptingRuns);
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Total collected initial states: {}.", emptinessCheckDenotationResult.initialStates.size());
        Log::log(Verbosity::verbose, "[Emptiness check denotation search] Collected initial states: {}.", fmt::join(emptinessCheckDenotationResult.initialStates, ", "));
        Log::log(Verbosity::debug, "[Emptiness check denotation search] Accepting runs\n{}.", fmt::join(emptinessCheckDenotationResult.acceptingRuns, "\n\n"));
        return emptinessCheckDenotationResult.result;
    }
};