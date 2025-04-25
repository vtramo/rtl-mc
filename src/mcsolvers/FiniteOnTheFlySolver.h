#pragma once

#include "Solver.h"
#include "automata_builder.h"
#include "DenotConcurrentV1.h"
#include "DenotOnTheFly.h"
#include "Timer.h"
#include "FiniteOnTheFlySolverStats.h"

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
        const bool discretiseRtlfDirectToLtl = false,
        const bool collectPaths = false
    ) : Solver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
        const std::string_view solverName = "FiniteOnTheFlySolver"
      , m_finiteOnTheFlySolverStats { std::make_shared<FiniteOnTheFlySolverStats>() }
      , m_concurrent { concurrent }
      , m_discretiseRtlfDirectToLtl { discretiseRtlfDirectToLtl }
      , m_collectPaths { collectPaths }
    {
       m_solverStats = m_finiteOnTheFlySolverStats;
    }

    ~FiniteOnTheFlySolver() override = default;

    [[nodiscard]] const FiniteOnTheFlySolverStats& stats() const override { return *m_finiteOnTheFlySolverStats; }

    SolverResult run() override
    {
        Timer timer {};
        preprocessPolyhedralSystem();
        logPolyhedralSystemAndCollectStats();

        preprocessRtlFormula();
        logRtlFormulaAndCollectStats();

        const double discretisationExecutionTimeSeconds { discretiseRtlFormula() };
        logAndCollectDiscretisationStats(discretisationExecutionTimeSeconds);

        constructBackwardFiniteLtlAutomaton();

        PowersetSharedPtr result { startDenotAlgorithm() };
        m_finiteOnTheFlySolverStats->addExecutionTime(timer.elapsedInSeconds());

        return SolverResult { m_isIncompleteResult, result };
    }
protected:
    BackwardNFAConstSharedPtr m_backwardNfa {};
    std::shared_ptr<FiniteOnTheFlySolverStats> m_finiteOnTheFlySolverStats {};
    bool m_concurrent {};
    bool m_discretiseRtlfDirectToLtl {};
    bool m_isIncompleteResult {};
    bool m_collectPaths {};

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
        const PolyhedralLtlFiniteAutomatonStats& backwardNfaStats { m_backwardNfa->stats() };
        m_finiteOnTheFlySolverStats->addAutomatonStats(backwardNfaStats);

        Log::log(Verbosity::verbose, "<<< BackwardNFA automaton construction completed. Elapsed time: {} s.\n", timer.elapsedInSeconds());
    }

    virtual PowersetUniquePtr startDenotAlgorithm()
    {
        Log::log(Verbosity::verbose, ">>> Denot algorithm started.");
        Timer timer {};

        std::unique_ptr denotUniquePtr { createDenotAlgorithm() };
        Denot& denot { *denotUniquePtr };

        PowersetUniquePtr result { denot() };
        if (m_universalDenotation)
        {
            Log::log(Verbosity::verbose, "[Negated result] Total patches: {}", result->size());
            Log::log(Verbosity::verbose, "[Negated result]: {}", PPLOutput::toString(*result, m_polyhedralSystem->symbolTable()));
            result = PPLUtils::minus(m_polyhedralSystem->invariant(), *result);
        }

        const double denotExecutionTimeSeconds { timer.elapsedInSeconds() };
        DenotOnTheFlyStats denotStats { collectDenotStats(denot, denotExecutionTimeSeconds) };
        m_isIncompleteResult = denotStats.getIsIncompleteResult();
        m_finiteOnTheFlySolverStats->addDenotOnTheFlyStats(denotStats);
        Log::log(Verbosity::verbose, "<<< Denot algorithm terminated. Elapsed time: {} s.", denotStats.getExecutionTimeSeconds());
        Log::log(Verbosity::verbose, "[Denot algorithm] Total iterations: {}.\n", denotStats.getTotalIterations());

        if (m_collectPaths && !m_concurrent)
        {
            const auto& denotPaths { denot.paths() };
            Log::log(Verbosity::debug, "[Denot algorithm] Total paths: {}", denotPaths.size());
            Log::log(Verbosity::debug, "[Denot algorithm] Paths:\n{}", fmt::join(denotPaths, "\n\n\n"));
        }

        return result;
    }

    std::unique_ptr<Denot> createDenotAlgorithm()
    {
        int maxIterations { getMaxIterationsFromEnvVar() };

        if (m_concurrent)
        {
            return std::make_unique<DenotConcurrentV1>(m_polyhedralSystem, m_backwardNfa);
        }

        return std::make_unique<DenotOnTheFly>(m_polyhedralSystem, m_backwardNfa, m_collectPaths, maxIterations);
    }

    static int getMaxIterationsFromEnvVar()
    {
        int maxIterations { 100000 };

        if (const char* envMaxIter { std::getenv("DENOT_MAX") })
        {
            try
            {
                maxIterations = std::stoi(envMaxIter);
                if (maxIterations <= 0)
                {
                    Log::log(Verbosity::verbose, "DENOT_MAX must be positive, using default value 100000");
                    maxIterations = 100000;
                }
            }
            catch ([[maybe_unused]] const std::exception& e)
            {
                Log::log(Verbosity::verbose, "Invalid DENOT_MAX value '{}', using default value 100000", envMaxIter);
                maxIterations = 100000;
            }
        }

        return maxIterations;
    }
};
