#pragma once

#include "PolyhedralSystem.h"
#include "PolyhedralSystemStats.h"
#include "RtlFormulaStats.h"
#include "DiscretisationStats.h"
#include "logger.h"
#include "stats_collectors.h"
#include "AutomatonOptimizationFlags.h"
#include "DiscreteFiniteLtlFormula.h"
#include "spot_utils.h"

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
    {}

    PowersetSharedPtr operator() ()
    {
        return run();
    }

    virtual PowersetSharedPtr run() = 0;

    [[nodiscard]] PolyhedralSystemStats polyhedralSystemStats() const { return m_polyhedralSystemStats; }
    [[nodiscard]] RtlFormulaStats rtlFormulaStats() const { return m_rtlFormulaStats; }
    [[nodiscard]] DiscretisationStats discretisationStats() const { return m_discretisationStats; }

protected:
    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    spot::formula m_rtlFormula {};
    bool m_universalDenotation { false };
    PolyhedralSystemStats m_polyhedralSystemStats {};
    AutomatonOptimizationFlags m_automatonOptimizationFlags {};
    RtlFormulaStats m_rtlFormulaStats {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    DiscretisationStats m_discretisationStats {};

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
        m_polyhedralSystemStats = collectPolyhedralSystemStats(*m_polyhedralSystem);
    }

    virtual void logRtlFormulaAndCollectStats()
    {
        Log::log(Verbosity::verbose, "[RTL Formula] Formula: {}.", m_rtlFormula);
        m_rtlFormulaStats = collectRtlStats(m_rtlFormula);
        Log::log(Verbosity::verbose, "[RTL Formula] Total atomic propositions: {}.", m_rtlFormulaStats.totalAtomicPropositions);
        Log::log(Verbosity::verbose, "[RTL Formula] Length: {}.\n", m_rtlFormulaStats.length);
    }

    virtual double discretiseRtlFormula() = 0;
    virtual void logAndCollectDiscretisationStats(const double discretisationExecutionTimeSeconds)
    {
        m_discretisationStats = collectDiscretisationStats(m_discreteLtlFormula, discretisationExecutionTimeSeconds);
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", m_discreteLtlFormula);
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", m_discretisationStats.discreteLtlFormulaTotalAtomicPropositions);
        Log::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", m_discretisationStats.discreteLtlFormulaLength);
    }
};