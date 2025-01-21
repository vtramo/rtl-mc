#pragma once

#include "ppl_utils.h"
#include "reach.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
inline namespace V1
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {}

        Powerset operator() () { return run(); }
        Powerset run()
        {
            m_iterations = 0;

            Powerset result { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };
            std::unordered_map<int, Powerset> V {};

            for (const int finalState: m_backwardNfa.finalStates())
            {
                const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };
                PowersetConstSharedPtr powersetFinalState { finalStateDenotation.denotation() };

                for (Powerset::const_iterator patchesIt { powersetFinalState->begin() }; patchesIt != powersetFinalState->end(); ++patchesIt)
                    PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), std::move(V), true));
            }

            return result;
        }

        [[nodiscard]] int totalIterations() const { return m_iterations; }

    private:
        int m_iterations { };
        PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
        const BackwardNFA& m_backwardNfa {};

        Powerset denot(
            const int state,
            const Poly& P,
            const Poly& X,
            std::unordered_map<int, Powerset>&& V,
            const bool isSing
        )
        {
            m_iterations++;
            assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
            assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

            const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
            assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

            if (m_backwardNfa.isInitialState(state))
                return Powerset { X };

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

                    const Powerset& powerset { denot(predecessor, Q, Y, std::move(V), !isSing) };
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

}

namespace V2
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {}

        Powerset operator() () { return run(); }
        Powerset run()
        {
            m_iterations = 0;

            Powerset result { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };

            for (const int finalState: m_backwardNfa.finalStates())
            {
                const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };
                PowersetConstSharedPtr powersetFinalState { finalStateDenotation.denotation() };
                for (Powerset::const_iterator patchesIt { powersetFinalState->begin() }; patchesIt != powersetFinalState->end(); ++patchesIt)
                    PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true));
            }

            return result;
        }

        [[nodiscard]] int totalIterations() const { return m_iterations; }

    private:
        int m_iterations { };
        PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
        const BackwardNFA& m_backwardNfa {};

        Powerset denot(
            const int state,
            const Poly& P,
            const Poly& X,
            std::unordered_map<int, PowersetSharedPtr> V,
            const bool isSing
        )
        {
            m_iterations++;
            assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
            assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

            const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
            assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

            if (m_backwardNfa.isInitialState(state))
                return Powerset { X };

            if (!stateDenotation.isSingular())
                addDisjunct(V, state, P);

            Powerset result { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };
            for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
            {
                int predecessor { static_cast<int>(edgePredecessor.dst) };
                const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
                PowersetSharedPtr alreadyVisitedAreaPredecessor { getDisjunct(V, predecessor) };
                assert(alreadyVisitedAreaPredecessor->space_dimension() == m_polyhedralSystem->getSpaceDimension());

                PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), *alreadyVisitedAreaPredecessor) };
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

                    const Powerset& powerset { denot(predecessor, Q, Y, V, !isSing) };
                    PPLUtils::fusion(result, powerset);
                }
            }

            return result;
        }

        void addDisjunct(std::unordered_map<int, PowersetSharedPtr>& V, const int state, const Poly& P) const
        {
            V.try_emplace(state, std::make_shared<Powerset>(Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY })).first->second->add_disjunct(P);
        }

        PowersetSharedPtr getDisjunct(std::unordered_map<int, PowersetSharedPtr>& V, const int state) const
        {
            return V.try_emplace(state, std::make_shared<Powerset>(Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY })).first->second;
        }
    };
}