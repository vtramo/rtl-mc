#pragma once

#include "OmnidirectionalSolver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalFiniteSolver.h"
#include "brink_stay_atoms.h"

class OmnidirectionalMaySolver: public OmnidirectionalSolver
{
public:
    OmnidirectionalMaySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const std::string_view solverName = "OmnidirectionalMaySolver"
    )
      : OmnidirectionalSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, solverName)
    {}

    ~OmnidirectionalMaySolver() override = default;

    SolverResult run() override
    {
        OmnidirectionalInfiniteSolver infiniteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        SolverResult infiniteSolverResult { infiniteSolver.run() };
        const SolverStats& infiniteSolverStats { infiniteSolver.stats() };

        preprocessPolyhedralSystem();
        preprocessRtlFormula();

        OmnidirectionalFiniteSolver finiteSolver { m_polyhedralSystem, m_rtlFormula, m_automatonOptimizationFlags, m_universalDenotation };
        SolverResult finiteSolverResult { finiteSolver.run() };
        const SolverStats& finiteSolverStats { finiteSolver.stats() };

        m_solverStats = std::make_shared<SolverStats>(infiniteSolverStats.merge(finiteSolverStats));

        PowersetConstSharedPtr infiniteResult { infiniteSolverResult.result() };
        return SolverResult {
            infiniteSolverResult.isIncompleteResult() || finiteSolverResult.isIncompleteResult(),
            PPLUtils::fusion(*infiniteResult, *infiniteSolverResult.result())
        };
    }
protected:
    void preprocessRtlFormula() override
    {
        m_rtlFormula = eventuallyBrinkAndLast(m_rtlFormula);
    }

    double discretiseRtlFormula() override
    {
        return 0;
    }

    void preprocessPolyhedralSystem() override
    {
        const auto& [brinkAtom, brinkInterpretation] { brinkMay(m_polyhedralSystem) };
        m_polyhedralSystem = m_polyhedralSystem->extend(brinkAtom, *brinkInterpretation);
    }

    void constructPolyhedralLtlAutomaton() override {}
};