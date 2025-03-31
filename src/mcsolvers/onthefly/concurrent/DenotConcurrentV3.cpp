#include "DenotConcurrentV3.h"

PowersetUniquePtr DenotConcurrentV3::run()
{
    m_iterations = 0;

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

    for (const int finalState: m_backwardNfa->acceptingStates())
    {
        const StateDenotation& finalStateDenotation { m_backwardNfa->stateDenotation(finalState) };

        PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
        PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

        for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
        {
            std::unordered_map<int, Powerset> V {};
            PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), V, true) };
            PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);
        }

        PPLUtils::fusion(*result, *finalStateResult);
    }

    return result;
}

PowersetUniquePtr DenotConcurrentV3::denot(
    const int state,
    const Poly& P,
    const Poly& X,
    std::unordered_map<int, Powerset> V,
    const bool isSing
)
{
    m_iterations++;

    assert(P.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    const StateDenotation& stateDenotation { m_backwardNfa->stateDenotation(state) };
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated.");

    if (m_backwardNfa->isInitialState(state))
        return std::make_unique<Powerset>(X);

    if (!stateDenotation.isSingular())
        addDisjunct(V, state, P);

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    if (!m_backwardNfa->hasSuccessors(state))
        return result;

    insertEmptyPredecessorVisitedPowersetsIfNotExist(V, state);

    const int totalPredecessors { m_backwardNfa->countSuccessors(state) };
    BackwardNFA::EdgeIterator edgeIterator { m_backwardNfa->successors(state) };
    const int myPredecessor { static_cast<int>(edgeIterator.begin()->dst) };
    std::vector<std::future<std::pair<int, ReachPairs>>> reachPairFutures {};
    if (totalPredecessors > 1)
    {
        reachPairFutures.reserve(totalPredecessors - 1);
        std::for_each(++edgeIterator.begin(), edgeIterator.end(),
        [&](const auto& edgePredecessor)
        {
            const int predecessor { static_cast<int>(edgePredecessor.dst) };
            reachPairFutures.push_back(
                computeReachPairsAsync(
                    predecessor,
                    getVisitedPowerset(V, predecessor),
                    X
                )
            );
        });
    }

    std::vector<std::pair<int, ReachPair>> predecessorReachPairs {};
    predecessorReachPairs.reserve(totalPredecessors);
    ReachPairs myReachPairs { computeReachPairs(myPredecessor, getVisitedPowerset(V, myPredecessor), X).second };
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
            const auto& task {
                PPL::make_threadable(
                    std::bind(
                        &DenotConcurrentV3::denot, this,
                        predecessor,
                        std::ref(Q),
                        std::ref(Y),
                        std::ref(V),
                        !isSing)
                    )
            };
            denotFutures.push_back(std::async(task));
        }
    }

    const int predecessor { predecessorReachPairs[0].first };
    const auto& [Q, Y] { predecessorReachPairs[0].second };
    PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

    for (auto& future: denotFutures)
        PPLUtils::fusion(*result, *future.get());

    return result;
}

std::future<std::pair<int, ReachPairs>> DenotConcurrentV3::computeReachPairsAsync(
    int predecessor,
    const Powerset& predecessorVisitedRegion,
    const Poly& X
)
{
    const auto& task {
        PPL::make_threadable(
            std::bind(
                &DenotConcurrentV3::computeReachPairs, this,
                predecessor, std::ref(predecessorVisitedRegion), std::ref(X)
            )
        )
    };

    return std::async(task);
}

std::pair<int, ReachPairs> DenotConcurrentV3::computeReachPairs(
    int predecessor,
    const Powerset& predecessorVisitedRegion,
    const Poly& X
) const
{
    const StateDenotation& predecessorStateDenotation { m_backwardNfa->stateDenotation(predecessor) };

    assert(predecessorVisitedRegion.space_dimension() == m_polyhedralSystem->spaceDimension());

    PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), predecessorVisitedRegion) };
    assert(A->space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(A->space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    ReachPairs reachPairs {
        predecessorStateDenotation.isSingular()
            ? reach0(*A, X, m_polyhedralSystem->preFlow())
            : reachPlus(*A, X, m_polyhedralSystem->preFlow())
    };

    return { predecessor, std::move(reachPairs) };
}

void DenotConcurrentV3::insertEmptyPredecessorVisitedPowersetsIfNotExist(std::unordered_map<int, Powerset>& V, const int state) const
{
    for (auto edgePredecessor: m_backwardNfa->successors(state))
    {
        const int predecessor { static_cast<int>(edgePredecessor.dst) };
        V.try_emplace(predecessor, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY });
    }
}

void DenotConcurrentV3::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
}

Powerset& DenotConcurrentV3::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state)
{
    return V.at(state);
}