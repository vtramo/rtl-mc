#pragma once

#include "Solver.h"
#include "automata_builder.h"
#include "DenotConcurrentV1.h"
#include "DenotOnTheFly.h"
#include "Timer.h"

using namespace SpotUtils;

class FiniteOnTheFlySolver: public Solver
{
public:
    FiniteOnTheFlySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const bool concurrent = false,
        const bool discretiseRtlfDirectToLtl = false
    ) : Solver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
      , m_concurrent { concurrent }
      , m_discretiseRtlfDirectToLtl { discretiseRtlfDirectToLtl }
    {}

    ~FiniteOnTheFlySolver() override = default;

    [[nodiscard]] DenotStats denotStats() const { return m_denotStats; }

    PowersetSharedPtr run() override
    {
        preprocessPolyhedralSystem();
        logPolyhedralSystemAndCollectStats();

        preprocessRtlFormula();
        logRtlFormulaAndCollectStats();

        const double discretisationExecutionTimeSeconds { discretiseRtlFormula() };
        logAndCollectDiscretisationStats(discretisationExecutionTimeSeconds);

        constructBackwardFiniteLtlAutomaton();

        return startDenotAlgorithm();
    }
protected:
    BackwardNFAConstSharedPtr m_backwardNfa {};
    DenotStats m_denotStats {};
    bool m_concurrent {};
    bool m_discretiseRtlfDirectToLtl {};

    void preprocessPolyhedralSystem() override {}

    double discretiseRtlFormula() override
    {
        Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
        Timer timer {};
        m_discreteLtlFormula = m_discretiseRtlfDirectToLtl
            ? DiscreteLtlFormula::discretiseRtlFinite(std::move(m_rtlFormula))
            : DiscreteFiniteLtlFormula::discretiseRtlFinite(std::move(m_rtlFormula)).toLtl();
        const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
        Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
        return discretisationExecutionTimeSeconds;
    }

    virtual void constructBackwardFiniteLtlAutomaton()
    {
        Log::log(Verbosity::verbose, ">>> BackwardNFA automaton construction started.");

        Timer timer {};
        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { m_polyhedralSystem };
        m_backwardNfa =
            buildBackwardNfa(
                std::move(m_discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                m_automatonOptimizationFlags
            );

        Log::log(Verbosity::verbose, "<<< BackwardNFA automaton construction completed. Elapsed time: {} s.\n", timer.elapsedInSeconds());
        Log::log(Verbosity::debug, "[BackwardNFA]\n{}\n", *m_backwardNfa);
    }

    virtual PowersetUniquePtr startDenotAlgorithm()
    {
        Log::log(Verbosity::verbose, ">>> Denot algorithm started.");
        Timer timer {};

        std::unique_ptr denotUniquePtr { createDenotAlgorithm() };
        Denot& denot { *denotUniquePtr };

        PowersetUniquePtr denotResult {
            m_universalDenotation
                ? PPLUtils::minus(m_polyhedralSystem->invariant(), *denot())
                : denot()
        };

        const double denotExecutionTimeSeconds { timer.elapsedInSeconds() };
        m_denotStats = collectDenotStats(denot, denotExecutionTimeSeconds);
        Log::log(Verbosity::verbose, "<<< Denot algorithm terminated. Elapsed time: {} s.", m_denotStats.executionTimeSeconds);
        Log::log(Verbosity::verbose, "<<< Denot algorithm total iterations: {}.\n", m_denotStats.totalIterations);

        return denotResult;
    }

    std::unique_ptr<Denot> createDenotAlgorithm()
    {
        if (m_concurrent)
        {
            return std::make_unique<DenotConcurrentV1>(m_polyhedralSystem, m_backwardNfa);
        }

        return std::make_unique<DenotOnTheFly>(m_polyhedralSystem, m_backwardNfa);
    }
};