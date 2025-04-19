#pragma once

#include "PolyhedralSystem.h"
#include "logger.h"
#include "stats_collectors.h"
#include "AutomatonOptimization.h"
#include "DiscreteFiniteLtlFormula.h"
#include "SolverResult.h"
#include "spot_utils.h"
#include "SolverStats.h"

using namespace SpotUtils;

class Solver;
using SolverUniquePtr = std::unique_ptr<Solver>;

class Solver
{
public:
    virtual ~Solver() = default;

    Solver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : m_polyhedralSystem { polyhedralSystem }
      , m_rtlFormula { rtlFormula }
      , m_universalDenotation { universalDenotation }
      , m_automatonOptimizationFlags { automatonOptimizationFlags }
      , m_solverStats { std::make_shared<SolverStats>() }
    {}

    SolverResult operator() ()
    {
        return run();
    }

    virtual SolverResult run() = 0;

    [[nodiscard]] virtual const SolverStats& stats() const { return *m_solverStats; }

protected:
    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    spot::formula m_rtlFormula {};
    bool m_universalDenotation { false };
    AutomatonOptimizationFlags m_automatonOptimizationFlags {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    std::shared_ptr<SolverStats> m_solverStats {};

    virtual void preprocessPolyhedralSystem() {}
    virtual void preprocessRtlFormula()
    {
        m_rtlFormula =
            m_universalDenotation
                ? Not(m_rtlFormula)
                : m_rtlFormula;
    }

    virtual void logPolyhedralSystemAndCollectStats()
    {
        Log::log(Verbosity::verbose, "[Polyhedral System]\n{}", *m_polyhedralSystem);
        m_solverStats->addPolyhedralSystemStats(collectPolyhedralSystemStats(*m_polyhedralSystem));
    }

    virtual void logRtlFormulaAndCollectStats()
    {
        Log::log(Verbosity::verbose, "[RTL Formula] Formula: {}.", m_rtlFormula);
        RtlFormulaStats rtlFormulaStats { collectRtlStats(m_rtlFormula) };
        m_solverStats->addRtlFormulaStats(rtlFormulaStats);
        Log::log(Verbosity::verbose, "[RTL Formula] Total atomic propositions: {}.", rtlFormulaStats.getTotalAtomicPropositions());
        Log::log(Verbosity::verbose, "[RTL Formula] Length: {}.\n", rtlFormulaStats.getLength());
    }

    virtual double discretiseRtlFormula() = 0;
    virtual void logAndCollectDiscretisationStats(const double discretisationExecutionTimeSeconds)
    {
        DiscretisationStats discretisationStats { collectDiscretisationStats(m_discreteLtlFormula, discretisationExecutionTimeSeconds) };
        m_solverStats->addDiscretisationStats(discretisationStats);
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", m_discreteLtlFormula);
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", discretisationStats.getDiscreteLtlFormulaTotalAtomicPropositions());
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", discretisationStats.getDiscreteLtlFormulaLength());
    }
};