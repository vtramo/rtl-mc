#pragma once

#include <FiniteOnTheFlySolver.h>

#include "GeneralSolver.h"

class GeneralInfiniteSolver: public GeneralSolver
{
public:
    GeneralInfiniteSolver(
        PolyhedralSystemSharedPtr polyhedralSystem,
        const spot::formula& rtlFormula,
        const AutomatonOptimizationFlags automatonOptimizationFlags,
        const bool universalDenotation = false
    )
      : GeneralSolver(polyhedralSystem, rtlFormula, automatonOptimizationFlags, universalDenotation)
    {
        bool isClosedFlow { m_polyhedralSystem->isClosedFlow() };
        bool isNonRecurrentRtl { SpotUtils::isNonRecurrent(m_rtlFormula) };
        if (!isClosedFlow || !isNonRecurrentRtl)
        {
            std::string errorMessage = "Error initializing GeneralInfiniteSolver: ";

            if (!isClosedFlow)
            {
                errorMessage += "The flow of the polyhedral system is not closed. ";
            }

            if (!isNonRecurrentRtl)
            {
                errorMessage += "The provided RTL formula is recurrent.";
            }

            throw std::runtime_error(errorMessage);
        }
    }

    ~GeneralInfiniteSolver() override = default;

    PowersetSharedPtr run() override
    {
        preprocessPolyhedralSystem();
        preprocessRtlFormula();

        SolverUniquePtr solver {};
        if (m_polyhedralSystem->isOmnidirectionalFlow())
        {
            solver = std::make_unique<OmnidirectionalFiniteSolver>(
                m_polyhedralSystem,
                m_rtlFormula,
                m_automatonOptimizationFlags,
                m_universalDenotation
            );
        }
        else
        {
            solver = std::make_unique<FiniteOnTheFlySolver>(
                m_polyhedralSystem,
                m_rtlFormula,
                m_automatonOptimizationFlags,
                m_universalDenotation
            );
        }

        return solver->run();
    }
protected:
    void preprocessPolyhedralSystem() override
    {
        addStayAtomInPolyhedralSystem();
    }

    void addStayAtomInPolyhedralSystem()
    {
        PowersetUniquePtr stayInterpretation { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

        if (!m_polyhedralSystem->isOmnidirectionalFlow())
        {
            std::vector observables { m_polyhedralSystem->generateObservables() };

            for (Observable observable: observables)
            {
                PowersetConstSharedPtr observableInterpretation { observable.interpretation() };
                for (auto patch { observableInterpretation->begin() }; patch != observableInterpretation->end(); ++patch)
                {
                    PolyUniquePtr patchCone { PPLUtils::cone(patch->pointset()) };
                    PolyUniquePtr patchConeIntersectFlow { PPLUtils::intersect(*patchCone, m_polyhedralSystem->flow()) };
                    if (!patchConeIntersectFlow->is_empty())
                    {
                        stayInterpretation->add_disjunct(patch->pointset());
                    }
                }
            }
        }

        m_polyhedralSystem->addAtomInterpretation(ap("stay"), *stayInterpretation);
    }

    void preprocessRtlFormula() override
    {
        m_rtlFormula = And({ m_rtlFormula, F(And({ ap("stay"), ap("last") }))});
    }

    double discretiseRtlFormula() override { return 0; }
    void constructPolyhedralLtlAutomaton() override {}
};