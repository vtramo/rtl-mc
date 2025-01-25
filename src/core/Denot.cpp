#include <spdlog/spdlog.h>
#include <reach.h>
#include "Denot.h"
#include "Timer.h"
#include "ppl_utils.h"

inline namespace V1
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            spdlog::trace("\n--- Starting from final state: {} ---", finalState);

            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

            spdlog::trace("Final state: {}\n{}", finalState, finalStateDenotation.toString(m_polyhedralSystem->getSymbolTable()));

            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
            int patchIndex { 1 };
            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                spdlog::trace("\n------ Final state {}, processing patch {}: {} ------\n",
                    finalState,
                    patchIndex,
                    PPLOutput::toString(patchesIt->pointset(), m_polyhedralSystem->getSymbolTable())
                );

                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true) };

                spdlog::trace("Final state {}, patch {} result: {}",
                    finalState,
                    patchIndex,
                    PPLOutput::toString(*finalStatePatchResult, m_polyhedralSystem->getSymbolTable())
                );

                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);

                spdlog::trace("------ Final state {}, result updated: {} ------",
                    finalState,
                    PPLOutput::toString(*finalStateResult, m_polyhedralSystem->getSymbolTable())
                );
            }

            PPLUtils::fusion(*result, *finalStateResult);

            spdlog::trace("Update global result: {}", PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable()));
            spdlog::trace("--- Final state {} completed ---\n", finalState);
        }

        return result;
    }

    PowersetUniquePtr Denot::denot(
        const int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset> V,
        const bool isSing
    )
    {
        m_iterations++;
        spdlog::trace("--------- Denot iteration {} ---------", m_iterations);

        assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

        spdlog::trace("State: {}\n{}", state, stateDenotation.toString(m_polyhedralSystem->getSymbolTable()));
        spdlog::trace("Visited region V[{}]: {}",
            state,
            PPLOutput::toString(getVisitedPowerset(V, state), m_polyhedralSystem->getSymbolTable())
        );
        spdlog::trace("P: {}", PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable()));
        spdlog::trace("X: {}", PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()));

        if (m_backwardNfa.isInitialState(state))
        {
            spdlog::trace("State {} is initial, returning X: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
            );

            return std::make_unique<Powerset>(X);
        }

        if (!stateDenotation.isSingular())
        {
            spdlog::trace("State {} is not singular, adding P: {}",
                state,
                PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable())
            );

            addDisjunct(V, state, P);

            spdlog::trace("State {} updated visited region: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
            );
        }

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
        for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
        {
            int predecessor { static_cast<int>(edgePredecessor.dst) };
            const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
            spdlog::trace("\n>>> State: {} -> Processing Predecessor: {}\n{}",
                state,
                predecessor,
                predecessorStateDenotation.toString(m_polyhedralSystem->getSymbolTable())
            );

            Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
            assert(visitedPowerset.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            spdlog::trace("Visited region V[{}]: {}",
                predecessor,
                PPLOutput::toString(visitedPowerset, m_polyhedralSystem->getSymbolTable())
            );

            PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
            assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
            spdlog::trace("Predecessor denotation minus visited region (A): {}",
                PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()));

            Timer timer {};
            spdlog::trace("Calling reach operator (isSing = {}).", isSing);
            PPLUtils::ReachPairs reachPairs {
                predecessorStateDenotation.isSingular()
                    ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                    : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
            };
            spdlog::trace("Reach pairs computed (size: {}). Elapsed time: {}.", reachPairs.size(), timer.elapsed());

            int reachPairIndex { 1 };
            for (const auto& [Q, Y]: reachPairs)
            {
                spdlog::trace("\nReach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                spdlog::trace("Q: {}\nY: {}",
                    PPLOutput::toString(Q, m_polyhedralSystem->getSymbolTable()),
                    PPLOutput::toString(Y, m_polyhedralSystem->getSymbolTable())
                );

                assert(Q.space_dimension() == m_polyhedralSystem->getSpaceDimension());
                assert(Y.space_dimension() == m_polyhedralSystem->getSpaceDimension());

                PowersetUniquePtr denotResult { denot(predecessor, Q, Y, V, !isSing) };

                spdlog::trace("Reach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                spdlog::trace("Result: {}", PPLOutput::toString(*denotResult, m_polyhedralSystem->getSymbolTable()));

                PPLUtils::fusion(*result, *denotResult);

                spdlog::trace("Update result denot iteration {}: {}",
                    m_iterations,
                    PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable())
                );

                reachPairIndex++;
            }

            spdlog::trace("<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
        }

        return result;
    }

    void Denot::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
    {
        V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
    }

    Powerset& Denot::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
    {
        return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
    }


}

namespace V1Concurrent
{
    Powerset Denot::run()
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

    void Denot::processFinalStates(const int start, const int end, Powerset& result)
    {
        for (int i = start; i < end; ++i)
        {
            assert(i >= 0 && i < static_cast<int>(m_denotationFinalStates.size()) && "Final state index is out of range");
            const int finalState { m_denotationFinalStates[i].first };
            PowersetConstSharedPtr denotationFinalState { m_denotationFinalStates[i].second };
            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
                PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true));
        }
    }

    Powerset Denot::denot(
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

            std::cout << "A: " << PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()) << " | X: " << PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()) << std::endl;
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

    void Denot::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
    {
        V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
    }

    Powerset& Denot::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
    {
        return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
    }
}
