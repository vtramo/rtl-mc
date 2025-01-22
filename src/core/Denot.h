#pragma once

#include <thread>

#include "ppl_utils.h"
#include "reach.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
#include "ppl_output.h"

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
            std::unordered_map<int, Powerset> V,
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
                Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
                assert(visitedPowerset.space_dimension() == m_polyhedralSystem->getSpaceDimension());

                PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
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

        void addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
        {
            V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
        }

        Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
        {
            return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
        }
    };
}

namespace V1Concurrent
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {
            const std::unordered_set<int>& finalStates { backwardNfa.finalStates() };
            m_finalStates.reserve(finalStates.size());
            for (const auto& state: finalStates)
                m_finalStates.push_back(state);
        }

        Powerset operator() () { return run(); }
        Powerset run()
        {
            const int totalFinalStates { m_backwardNfa.totalFinalStates() };
            constexpr int minPerThread { 5 };
            const int maxThreads { (totalFinalStates + minPerThread - 1) / minPerThread };
            const int hardwareThreads { static_cast<int>(std::thread::hardware_concurrency()) };
            const int numThreads { std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads) };
            const int finalStatesPerThread { totalFinalStates / numThreads };

            std::cout << "num threads: " << numThreads << std::endl;
            std::cout << "finalStatesPerThread: " << finalStatesPerThread << std::endl;
            std::cout << "totalFinalStates: " << totalFinalStates << std::endl;

            std::vector results(numThreads, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY });
            std::vector<std::thread> threads {};
            threads.reserve(numThreads - 1);
            int start = 0;
            for (int i = 0; i < numThreads - 1; ++i)
            {
                const int end { start + finalStatesPerThread };
                threads.push_back(std::thread(
                    &Denot::processFinalStates, this,
                    start, end, std::ref(results[i])
                ));
                start = end;
            }

            std::cout << "final thread\n" << std::endl;
            processFinalStates(start, totalFinalStates - 1, std::ref(results[numThreads - 1]));

            for (auto& thread: threads)
                thread.join();

            Powerset finalResult { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY };
            for (const auto& result: results)
                PPLUtils::fusion(finalResult, result);
            // return std::reduce(results.begin(), results.end(),
            //     Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY },
            //     [](Powerset& lhs, const Powerset& rhs)
            //     {
            //         PPLUtils::fusion(lhs, rhs);
            //         return lhs;
            //     });
            return finalResult;
        }

        [[nodiscard]] int totalIterations() const { return 0; /* race conditions, maybe atomic int */ }

    private:
        using FinalStateConstIterator = std::unordered_set<int>::const_iterator;

        PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
        const BackwardNFA& m_backwardNfa {};
        std::vector<int> m_finalStates {};

        void processFinalStates(const int start, const int end, Powerset& result)
        {
            for (int i = start; i < end; ++i)
            {
                assert(i >= 0 && i < static_cast<int>(m_finalStates.size()) && "Final state index is out of range");
                const int finalState { m_finalStates[i] };
                const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };
                PowersetConstSharedPtr powersetFinalState { finalStateDenotation.denotation() };
                for (Powerset::const_iterator patchesIt { powersetFinalState->begin() }; patchesIt != powersetFinalState->end(); ++patchesIt)
                    PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true));
            }
        }

        Powerset denot(
            const int state,
            const Poly& P,
            const Poly& X,
            std::unordered_map<int, Powerset> V,
            const bool isSing
        )
        {
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
                Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
                assert(visitedPowerset.space_dimension() == m_polyhedralSystem->getSpaceDimension());

                PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
                assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
                assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

                // std::cout << "A: " << PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()) << " | X: " << PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()) << std::endl;
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

        void addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
        {
            V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
        }

        Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
        {
            return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
        }
    };
}