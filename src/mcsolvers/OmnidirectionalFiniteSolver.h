#pragma once

#include "OmnidirectionalSolver.h"
#include "automata_builder.h"
#include "emptiness_algorithms.h"
#include "Timer.h"

class OmnidirectionalFiniteSolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalFiniteSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const std::string_view solverName = "OmnidirectionalFiniteSolver"
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, solverName)
    {}

    ~OmnidirectionalFiniteSolver() override = default;

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

        return SolverResult { false, runFiniteEmptinessCheckDenotationSearch() };
    }
protected:

    double discretiseRtlFormula() override
    {
        Log::log(Verbosity::verbose, ">>> {} - RTL formula discretisation started.", name());
        Timer timer {};
        m_discreteLtlFormula = DiscreteLtlFormula::discretiseRtlFinite(std::move(m_rtlFormula));
        const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
        Log::log(Verbosity::verbose, "<<< {} - Discretisation completed. Elapsed time: {} s.", name(), discretisationExecutionTimeSeconds);
        return discretisationExecutionTimeSeconds;
    }

    void constructPolyhedralLtlAutomaton() override
    {
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { m_polyhedralSystem };
        m_ltlAutomaton = buildPolyhedralLtlFiniteAutomaton(
            std::move(m_discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            m_automatonOptimizationFlags
        );
        const PolyhedralLtlAutomatonStats& automatonStats { m_ltlAutomaton->stats() };
        m_solverStats->addAutomatonStats(automatonStats);
    }

    virtual PowersetSharedPtr runFiniteEmptinessCheckDenotationSearch()
    {
        Log::log(Verbosity::verbose, "[{} - Finite emptiness check denotation search] Started.", name());
        Timer timer {};

        std::unordered_set<unsigned> initialStatesWithAcceptingRuns { collectInitialStatesWithAcceptingRuns(*m_polyhedralSynchronousProduct) };
        PowersetSharedPtr result { std::make_shared<Powerset>(m_polyhedralSynchronousProduct->spaceDimension(), PPL::EMPTY) };
        for (unsigned initialState: initialStatesWithAcceptingRuns)
        {
            PPLUtils::fusion(*result, *m_polyhedralSynchronousProduct->points(initialState));
        }

        Log::log(Verbosity::verbose, "[{} - Finite emptiness check denotation search] Completed. Elapsed time: {} s.", name(), timer.elapsedInSeconds());
        Log::log(Verbosity::verbose, "[{} - Finite emptiness check denotation search] Total collected initial states: {}.", name(), initialStatesWithAcceptingRuns.size());
        Log::log(Verbosity::verbose, "[{} - Finite emptiness check denotation search] Collected initial states: {}.", name(), fmt::join(initialStatesWithAcceptingRuns, ", "));

        return result;
    }
};