#pragma once

#include "FiniteOnTheFlySolver.h"
#include "brink_stay_atoms.h"

class StayFiniteOnTheFlySolver: public FiniteOnTheFlySolver
{
public:
    StayFiniteOnTheFlySolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false,
        const bool concurrent = false,
        const bool discretiseRtlfDirectToLtl = false
    ) : FiniteOnTheFlySolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation, concurrent, discretiseRtlfDirectToLtl)
    {}

    ~StayFiniteOnTheFlySolver() override = default;
protected:

    void preprocessPolyhedralSystem() override
    {
        Log::log(Verbosity::verbose, ">>> Atomic proposition 'stay' calculation started.");
        Timer timer {};
        const auto& [stayAtom, stayInterpretation] { stay(m_polyhedralSystem) };
        m_polyhedralSystem = m_polyhedralSystem->extend(stayAtom, *stayInterpretation);
        Log::log(Verbosity::verbose, "[Stay calculation] Polyhedral system has been extended with the atomic proposition 'stay'.");
        Log::log(Verbosity::verbose, "[Stay calculation] 'stay' interpretation: {}", PPLOutput::toString(*stayInterpretation, m_polyhedralSystem->symbolTable()));
        Log::log(Verbosity::verbose, "<<< Atomic proposition 'stay' calculation completed. Elapsed time: {} s\n", timer.elapsedInSeconds());
    }

    void preprocessRtlFormula() override
    {
        Log::log(Verbosity::verbose, "[Original RTL Formula] {}", m_rtlFormula);

        m_rtlFormula = eventuallyStayAndLast(m_rtlFormula);
        FiniteOnTheFlySolver::preprocessRtlFormula();
    }
};