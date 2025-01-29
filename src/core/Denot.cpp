#include "logger.h"
#include "reach.h"
#include "Denot.h"
#include <spdlog/spdlog.h>
#include <future>
#include <stack>
#include "reach.h"
#include "Timer.h"
#include "Denot.h"
#include "BackwardNFA.h"
#include "ppl_utils.h"
#include "ppl_output.h"
#include "StateDenotation.h"

inline namespace V1Recursive
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            Logger::log(Verbosity::trace, "\n--- Starting from final state: {} ---", finalState);

            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

            Logger::log(Verbosity::trace, "Final state: {}\n{}", finalState, finalStateDenotation.toString(m_polyhedralSystem->getSymbolTable()));

            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
            int patchIndex { 1 };
            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                Logger::log(Verbosity::trace, "\n------ Final state {}, processing patch {}: {} ------\n",
                    finalState,
                    patchIndex,
                    PPLOutput::toString(patchesIt->pointset(), m_polyhedralSystem->getSymbolTable())
                );

                std::unordered_map<int, Powerset> V {};
                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), V, true) };

                Logger::log(Verbosity::trace, "Final state {}, patch {} result: {}",
                    finalState,
                    patchIndex,
                    PPLOutput::toString(*finalStatePatchResult, m_polyhedralSystem->getSymbolTable())
                );

                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);

                Logger::log(Verbosity::trace, "------ Final state {}, result updated: {} ------",
                    finalState,
                    PPLOutput::toString(*finalStateResult, m_polyhedralSystem->getSymbolTable())
                );
            }

            PPLUtils::fusion(*result, *finalStateResult);

            Logger::log(Verbosity::trace, "Update global result: {}", PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable()));
            Logger::log(Verbosity::trace, "--- Final state {} completed ---\n", finalState);
        }

        return result;
    }

    PowersetUniquePtr Denot::denot(
        const int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset>& V,
        const bool isSing
    )
    {
        m_iterations++;
        Logger::log(Verbosity::trace, "--------- Denot iteration {} ---------", m_iterations);

        assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

        Logger::log(Verbosity::trace, "State: {}\n{}", state, stateDenotation.toString(m_polyhedralSystem->getSymbolTable()));
        Logger::log(Verbosity::trace, "Is initial {}", m_backwardNfa.isInitialState(state));
        Logger::log(Verbosity::trace, "Is final {}", m_backwardNfa.isFinalState(state));
        Logger::log(Verbosity::trace, "Visited region V[{}]: {}",
            state,
            PPLOutput::toString(getVisitedPowerset(V, state), m_polyhedralSystem->getSymbolTable())
        );
        Logger::log(Verbosity::trace, "P: {}", PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable()));
        Logger::log(Verbosity::trace, "X: {}\n", PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()));

        if (m_backwardNfa.isInitialState(state))
        {
            Logger::log(Verbosity::trace, "State {} is initial, returning X: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
            );

            return std::make_unique<Powerset>(X);
        }

        if (!stateDenotation.isSingular())
        {
            Logger::log(Verbosity::trace, "State {} is not singular, adding P: {}",
                state,
                PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable())
            );

            addDisjunct(V, state, P);

            Logger::log(Verbosity::trace, "State {} updated visited region: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
            );
        }

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
        for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
        {
            int predecessor { static_cast<int>(edgePredecessor.dst) };
            const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
            Logger::log(Verbosity::trace, "\n>>> State: {} -> Processing Predecessor: {}\n{}",
                state,
                predecessor,
                predecessorStateDenotation.toString(m_polyhedralSystem->getSymbolTable())
            );

            Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
            assert(visitedPowerset.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            Logger::log(Verbosity::trace, "Visited region V[{}]: {}",
                predecessor,
                PPLOutput::toString(visitedPowerset, m_polyhedralSystem->getSymbolTable())
            );

            PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
            assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
            Logger::log(Verbosity::trace, "Predecessor denotation minus visited region (A): {}",
                PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()));

            Timer timer {};
            Logger::log(Verbosity::trace, "Calling reach operator (isSing = {}).", isSing);
            PPLUtils::ReachPairs reachPairs {
                predecessorStateDenotation.isSingular()
                    ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                    : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
            };
            Logger::log(Verbosity::trace, "Reach pairs computed (size: {}). Elapsed time: {} s.", reachPairs.size(), timer.elapsedInSeconds());

            int reachPairIndex { 1 };
            for (const auto& [Q, Y]: reachPairs)
            {
                Logger::log(Verbosity::trace, "\nReach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                Logger::log(Verbosity::trace, "Q: {}\nY: {}",
                    PPLOutput::toString(Q, m_polyhedralSystem->getSymbolTable()),
                    PPLOutput::toString(Y, m_polyhedralSystem->getSymbolTable())
                );

                assert(Q.space_dimension() == m_polyhedralSystem->getSpaceDimension());
                assert(Y.space_dimension() == m_polyhedralSystem->getSpaceDimension());

                PowersetUniquePtr denotResult { denot(predecessor, Q, Y, V, !isSing) };

                Logger::log(Verbosity::trace, "Reach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                Logger::log(Verbosity::trace, "Result: {}", PPLOutput::toString(*denotResult, m_polyhedralSystem->getSymbolTable()));

                PPLUtils::fusion(*result, *denotResult);

                Logger::log(Verbosity::trace, "Update result denot iteration {}: {}",
                    m_iterations,
                    PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable())
                );

                reachPairIndex++;
            }

            Logger::log(Verbosity::trace, "<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
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

namespace V2Iterative
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };
            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true) };
                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);
            }
            PPLUtils::fusion(*result, *finalStateResult);
        }

        return result;
    }

    struct DenotFrame
    {
        int state {};
        Poly P {};
        Poly X {};
        std::unordered_map<int, Powerset> V {};
        const bool isSing {};
    };

    PowersetUniquePtr Denot::denot(
        const int _state,
        const Poly& _P,
        const Poly& _X,
        std::unordered_map<int, Powerset> _V,
        const bool _isSing
    )
    {
        std::stack<DenotFrame> stack {};
        stack.push({ _state, _P, _X, _V, _isSing });
        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        while (!stack.empty())
        {
            DenotFrame& denotFrame { stack.top() };

            m_iterations++;

            const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(denotFrame.state) };

            if (m_backwardNfa.isInitialState(denotFrame.state))
            {
                result->add_disjunct(denotFrame.X);
                stack.pop();
                continue;
            }

            if (!stateDenotation.isSingular())
            {
                addDisjunct(denotFrame.V, denotFrame.state, denotFrame.P);
            }

            for (const auto& edgePredecessor: m_backwardNfa.predecessors(denotFrame.state))
            {
                int predecessor { static_cast<int>(edgePredecessor.dst) };
                const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };

                Powerset& visitedPowerset { getVisitedPowerset(denotFrame.V, predecessor) };

                PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };

                PPLUtils::ReachPairs reachPairs {
                    predecessorStateDenotation.isSingular()
                        ? PPLUtils::reach0(*A, denotFrame.X, m_polyhedralSystem->getPreFlow())
                        : PPLUtils::reachPlus(*A, denotFrame.X, m_polyhedralSystem->getPreFlow())
                };

                for (const auto& [Q, Y]: reachPairs)
                {
                    stack.push({ predecessor, Q, Y, denotFrame.V, !_isSing });
                }
            }

            stack.pop();
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
    PowersetUniquePtr Denot::run()
    {
        const int totalFinalStates { m_backwardNfa.totalFinalStates() };
        constexpr int minPerThread { 1 };
        const int maxThreads { (totalFinalStates + minPerThread - 1) / minPerThread };
        const int hardwareThreads { static_cast<int>(std::thread::hardware_concurrency()) };
        const int numThreads { std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads) };
        const int finalStatesPerThread { totalFinalStates / numThreads };

        std::cout << "num threads: " << numThreads << std::endl;
        std::cout << "finalStatesPerThread: " << finalStatesPerThread << std::endl;
        std::cout << "totalFinalStates: " << totalFinalStates << std::endl;

        std::vector<Powerset> results(numThreads, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY });
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

        PowersetUniquePtr finalResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
        for (const auto& result: results)
            PPLUtils::fusion(*finalResult, result);
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

namespace V2Concurrent
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true) };
                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);
            }

            PPLUtils::fusion(*result, *finalStateResult);
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

        assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

        if (m_backwardNfa.isInitialState(state))
        {
            return std::make_unique<Powerset>(X);
        }

        if (!stateDenotation.isSingular())
        {
            addDisjunct(V, state, P);
        }

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
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

            if (reachPairs.empty()) continue;
            if (reachPairs.size() == 1)
            {
                PPLUtils::fusion(*result, *denot(predecessor, reachPairs[0].first, reachPairs[0].second, V, !isSing));
                continue;
            }

            std::vector<std::future<PowersetUniquePtr>> futures {};
            futures.reserve(reachPairs.size() - 1);
            for (int i = 1; i < static_cast<int>(reachPairs.size()); i++)
            {
                const Poly& Q { reachPairs[i].first };
                const Poly& Y { reachPairs[i].second };
                futures.push_back(std::async(&Denot::denot, this, predecessor, Q, Y, V, !isSing));
            }

            const Poly& Q { reachPairs[0].first };
            const Poly& Y { reachPairs[0].second };
            PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

            for (auto& future: futures)
                PPLUtils::fusion(*result, *future.get());
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

namespace V3Concurrent
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true) };
                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);
            }

            PPLUtils::fusion(*result, *finalStateResult);
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

        assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

        if (m_backwardNfa.isInitialState(state))
        {
            return std::make_unique<Powerset>(X);
        }

        if (!stateDenotation.isSingular())
        {
            addDisjunctV3(V, state, P);
        }

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
        if (!m_backwardNfa.hasPredecessors(state))
            return result;

        insertEmptyPredecessorVisitedPowersetsIfNotExist(V, state);

        const int totalPredecessors { m_backwardNfa.countPredecessors(state) };
        BackwardNFA::EdgeIterator edgeIterator { m_backwardNfa.predecessors(state) };
        const int myPredecessor { static_cast<int>(edgeIterator.begin()->dst) };
        std::vector<std::future<std::pair<int, PPLUtils::ReachPairs>>> reachPairFutures {};
        if (totalPredecessors > 1)
        {
            reachPairFutures.reserve(totalPredecessors - 1);
            std::for_each(++edgeIterator.begin(), edgeIterator.end(),
            [&](const auto& edgePredecessor)
            {
                const int predecessor { static_cast<int>(edgePredecessor.dst) };
                reachPairFutures.push_back(computeReachPairsAsync(predecessor, getVisitedPowersetV3(V, predecessor), X));
            });
        }

        std::vector<std::pair<int, PPLUtils::ReachPair>> predecessorReachPairs {};
        predecessorReachPairs.reserve(totalPredecessors);
        PPLUtils::ReachPairs myReachPairs { computeReachPairs(myPredecessor, getVisitedPowersetV3(V, myPredecessor), X).second };
        for (auto&& myReachPair: myReachPairs)
            predecessorReachPairs.emplace_back(myPredecessor, std::move(myReachPair));

        for (auto& future: reachPairFutures)
        {
            auto&& [predecessor, reachPairs] { future.get() };
            for (auto&& reachPair: reachPairs)
                predecessorReachPairs.emplace_back(predecessor, std::move(reachPair));
        }

        if (predecessorReachPairs.empty()) return result;

        std::vector<std::future<PowersetUniquePtr>> denotFutures {};
        if (predecessorReachPairs.size() > 1)
        {
            denotFutures.reserve(predecessorReachPairs.size() - 1);
            for (int i { 1 }; i < static_cast<int>(predecessorReachPairs.size()); i++)
            {
                const int predecessor { predecessorReachPairs[i].first };
                const auto& [Q, Y] { predecessorReachPairs[i].second };
                denotFutures.push_back(std::async(&Denot::denot, this, predecessor, Q, Y, V, !isSing));
            }
        }

        const int predecessor { predecessorReachPairs[0].first };
        const auto& [Q, Y] { predecessorReachPairs[0].second };
        PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

        for (auto& future: denotFutures)
            PPLUtils::fusion(*result, *future.get());

        return result;
    }

    std::future<std::pair<int, PPLUtils::ReachPairs>> Denot::computeReachPairsAsync(
        int predecessor,
        const Powerset& predecessorVisitedRegion,
        const Poly& X
    )
    {
        return std::async(&Denot::computeReachPairs, this, predecessor, std::ref(predecessorVisitedRegion), std::ref(X));
    }

    std::pair<int, PPLUtils::ReachPairs> Denot::computeReachPairs(
        int predecessor,
        const Powerset& predecessorVisitedRegion,
        const Poly& X
    ) const
    {
        const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };

        assert(predecessorVisitedRegion.space_dimension() == m_polyhedralSystem->getSpaceDimension());

        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), predecessorVisitedRegion) };
        assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

        PPLUtils::ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
        };

        return { predecessor, std::move(reachPairs) };
    }

    void Denot::insertEmptyPredecessorVisitedPowersetsIfNotExist(std::unordered_map<int, Powerset>& V, const int state) const
    {
        for (auto edgePredecessor: m_backwardNfa.predecessors(state))
        {
            const int predecessor { static_cast<int>(edgePredecessor.dst) };
            V.try_emplace(predecessor, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY });
        }
    }

    void Denot::addDisjunctV3(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
    {
        V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
    }

    Powerset& Denot::getVisitedPowersetV3(std::unordered_map<int, Powerset>& V, const int state)
    {
        return V.at(state);
    }
}

namespace V4Concurrent
{
    PowersetUniquePtr Denot::run()
    {
        m_iterations = 0;

        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

        for (const int finalState: m_backwardNfa.finalStates())
        {
            const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

            PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
            PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

            for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
            {
                PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), {}, true) };
                PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);
            }

            PPLUtils::fusion(*result, *finalStateResult);
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
        if (m_backwardNfa.isInitialState(state))
            return std::make_unique<Powerset>(X);

        std::unique_lock lockGuard { m_mutex };
        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        if (!stateDenotation.isSingular())
            addDisjunct(V, state, P);
        lockGuard.unlock();

        PowersetUniquePtr result { std::make_unique<Powerset>(m_spaceDimension, PPL::EMPTY) };
        if (!m_backwardNfa.hasPredecessors(state))
            return result;

        const int totalPredecessors { m_backwardNfa.countPredecessors(state) };
        std::vector<std::pair<int, PPLUtils::ReachPair>> predecessorReachPairs {};
        predecessorReachPairs.reserve(totalPredecessors * 2);
        for (auto edgePredecessor: m_backwardNfa.predecessors(state))
        {
            const int predecessor { static_cast<int>(edgePredecessor.dst) };
            std::cout << ">>> predecessor " << predecessor << " | Thread: " << std::this_thread::get_id() << std::endl;
            lockGuard.lock();
            const auto& predecessorVisitedPatches { getVisitedPatches(V, predecessor) };
            PPLUtils::ReachPairs reachPairs { computeReachPairs(predecessor, predecessorVisitedPatches, X).second };
            for (auto&& reachPair: reachPairs)
                predecessorReachPairs.emplace_back(predecessor, std::move(reachPair));
            lockGuard.unlock();
            std::cout << "<<< predecessor " << predecessor << " | Thread: " << std::this_thread::get_id() << std::endl;
        }

        std::vector<std::future<PowersetUniquePtr>> denotFutures {};
        if (predecessorReachPairs.size() > 1)
        {
            denotFutures.reserve(predecessorReachPairs.size() - 1);
            for (int i { 1 }; i < static_cast<int>(predecessorReachPairs.size()); i++)
            {
                const int predecessor { predecessorReachPairs[i].first };
                const auto& [Q, Y] { predecessorReachPairs[i].second };
                denotFutures.push_back(std::async(std::launch::async, &Denot::denot, this, predecessor, Q, Y, V, !isSing));
            }
        }

        const int predecessor { predecessorReachPairs[0].first };
        const auto& [Q, Y] { predecessorReachPairs[0].second };
        PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

        for (auto& future: denotFutures)
            PPLUtils::fusion(*result, *future.get());

        return result;
    }

    std::pair<int, PPLUtils::ReachPairs> Denot::computeReachPairs(
        int predecessor,
        const Powerset& predecessorVisitedPatches,
        const Poly& X
    ) const
    {
        const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), predecessorVisitedPatches) };

        PPLUtils::ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
        };

        return { predecessor, std::move(reachPairs) };
    }

    Powerset& Denot::getVisitedPatches(std::unordered_map<int, Powerset>& V, const int state) const
    {
        return V.try_emplace(state, Powerset { m_spaceDimension, PPL::EMPTY }).first->second;
    }

    void Denot::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
    {
        V.try_emplace(state, Powerset { m_spaceDimension, PPL::EMPTY }).first->second.add_disjunct(P);
    }
}