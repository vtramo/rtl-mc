#pragma once

#include "ppl_utils.h"
#include "reach.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class Denot
{
public:
    Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {}

    Powerset run()
    {
        Powerset result { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };
        std::unordered_map<int, Powerset> V {};

        for (const int finalState: m_backwardNfa.finalStates())
        {
            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };
            PowersetConstSharedPtr powersetFinalState { finalStateDenotation.denotation() };
            for (Powerset::const_iterator patchesIt { powersetFinalState->begin() }; patchesIt != powersetFinalState->end(); ++patchesIt)
                PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), std::move(V)));
        }

        return result;
    }

private:
    int m_iterations { 1 };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};
    
    Powerset denot(
        const int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset>&& V
    )
    {
        std::cerr << "Denot iteration: " << m_iterations++ << '\n';

        assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        if (m_backwardNfa.isInitialState(state))
            return Powerset { X };

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        if (!stateDenotation.isSingular())
            addDisjunct(V, state, P);

        Powerset result { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };
        for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
        {
            int predecessor { static_cast<int>(edgePredecessor.dst) };
            const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
            const Powerset& alreadyVisitedAreaPredecessor { getDisjunct(V, predecessor) };
            assert(alreadyVisitedAreaPredecessor.space_dimension() == m_polyhedralSystem->getSpaceDimension());

            PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), alreadyVisitedAreaPredecessor) };
            assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

            PPLUtils::ReachPairs reachPairs {
                predecessorStateDenotation.isSingular()
                    ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                    : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
            };

            for (const auto& [Q, Y]: reachPairs)
            {
                assert(Q.space_dimension() == m_polyhedralSystem->getSpaceDimension());
                assert(Y.space_dimension() == m_polyhedralSystem->getSpaceDimension());

                const Powerset& powerset { denot(predecessor, Q, Y, std::move(V)) };
                PPLUtils::fusion(result, powerset);
            }
        }

        return result;
    }

    void addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
    {
        V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
    }

    const Powerset& getDisjunct(std::unordered_map<int, Powerset>& V, const int state) const
    {
        return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
    }
};
