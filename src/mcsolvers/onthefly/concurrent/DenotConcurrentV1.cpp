#include "DenotConcurrentV1.h"
#include "ppl_utils.h"
#include "reach.h"

PowersetUniquePtr DenotConcurrentV1::run()
{
    const int totalAcceptingStates { static_cast<int>(m_backwardNfa->totalAcceptingStates()) };
    constexpr int minPerThread { 1 };
    const int maxThreads { (totalAcceptingStates + minPerThread - 1) / minPerThread };
    const int hardwareThreads { static_cast<int>(std::thread::hardware_concurrency()) };
    const int numThreads { std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads) };
    const int finalStatesPerThread { totalAcceptingStates / numThreads };

    std::cout << "num threads: " << numThreads << std::endl;
    std::cout << "finalStatesPerThread: " << finalStatesPerThread << std::endl;
    std::cout << "totalAcceptingStates: " << totalAcceptingStates << std::endl;

    std::vector results(numThreads, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY });
    std::vector<std::thread> threads {};
    threads.reserve(numThreads - 1);
    int start = 0;
    for (int i = 0; i < numThreads - 1; ++i)
    {
        const int end { start + finalStatesPerThread };
        auto task {
            PPL::make_threadable(
                std::bind(
                    &DenotConcurrentV1::processAcceptingStates, this,
                    start, end, std::ref(results[i]))
                )
        };
        threads.push_back(std::thread(task));
        start = end;
    }

    std::cout << "final thread\n" << std::endl;
    processAcceptingStates(start, totalAcceptingStates - 1, std::ref(results[numThreads - 1]));

    for (auto& thread: threads)
        thread.join();

    PowersetUniquePtr finalResult { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const auto& result: results)
        PPLUtils::fusion(*finalResult, result);
    return finalResult;
}

void DenotConcurrentV1::processAcceptingStates(const int start, const int end, Powerset& result)
{
    for (int i = start; i < end; ++i)
    {
        assert(i >= 0 && i < static_cast<int>(m_denotationAcceptingStates.size()) && "Accepting state index is out of range");
        const int finalState { m_denotationAcceptingStates[i].first };
        PowersetConstSharedPtr denotationFinalState { m_denotationAcceptingStates[i].second };
        for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
        {
            std::unordered_map<int, Powerset> V {};
            PPLUtils::fusion(result, denot(finalState, patchesIt->pointset(), patchesIt->pointset(), V, true));
        }
    }
}

Powerset DenotConcurrentV1::denot(
    const int state,
    const Poly& P,
    const Poly& X,
    std::unordered_map<int, Powerset> V,
    const bool isSing
)
{
    // std::cerr << "Thread " << std::this_thread::get_id() << " State: " << state << std::endl;
    assert(P.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    const StateDenotation& stateDenotation { m_backwardNfa->stateDenotation(state) };
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated.");

    if (m_backwardNfa->isInitialState(state))
        return Powerset { X };

    if (!stateDenotation.isSingular())
        addDisjunct(V, state, P);

    Powerset result { m_polyhedralSystem->spaceDimension(), PPL::EMPTY };
    for (const auto& edgePredecessor: m_backwardNfa->successors(state))
    {
        int predecessor { static_cast<int>(edgePredecessor.dst) };
        const StateDenotation& predecessorStateDenotation { m_backwardNfa->stateDenotation(predecessor) };
        Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
        assert(visitedPowerset.space_dimension() == m_polyhedralSystem->spaceDimension());

        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
        assert(A->space_dimension() == m_polyhedralSystem->spaceDimension());
        assert(A->space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

        // std::cout << "A: " << PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()) << " | X: " << PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()) << std::endl;
        ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? reach0(*A, X, m_polyhedralSystem->preFlow())
                : reachPlus(*A, X, m_polyhedralSystem->preFlow())
        };

        for (const auto& [Q, Y]: reachPairs)
        {
            assert(Q.space_dimension() == m_polyhedralSystem->spaceDimension());
            assert(Y.space_dimension() == m_polyhedralSystem->spaceDimension());

            const Powerset& powerset { denot(predecessor, Q, Y, V, !isSing) };
            PPLUtils::fusion(result, powerset);
        }
    }

    return result;
}

void DenotConcurrentV1::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
}

Powerset& DenotConcurrentV1::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
{
    return V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second;
}