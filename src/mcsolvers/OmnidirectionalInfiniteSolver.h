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

        return runEmptinessCheckDenotation();
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

    PowersetSharedPtr runEmptinessCheckDenotation()
    {
        Log::log(Verbosity::verbose, "[Emptiness check denotation] Started.");
        EmptinessCheckDenotationResult emptinessCheckDenotationResult { explicitSe05Search(m_polyhedralSynchronousProduct) };
        Log::log(Verbosity::verbose, "[Emptiness check denotation] Total accepting runs: {}.", emptinessCheckDenotationResult.totalAcceptingRuns);
        Log::log(Verbosity::debug, "[Emptiness check denotation] Accepting runs\n{}.", fmt::join(emptinessCheckDenotationResult.acceptingRuns, "\n\n"));
        return emptinessCheckDenotationResult.result;
    }
};