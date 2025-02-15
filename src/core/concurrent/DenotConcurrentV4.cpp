#include "DenotConcurrentV4.h"
#include "reach.h"
#include <future>


PowersetUniquePtr DenotConcurrentV4::run()
{
    m_iterations = 0;

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

    for (const int finalState: m_backwardNfa.finalStates())
    {
        const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

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

PowersetUniquePtr DenotConcurrentV4::denot(
    const int state,
    const Poly& P,
    const Poly& X,
    std::unordered_map<int, Powerset> V,
    const bool isSing
)
{
    if (m_backwardNfa.isInitialState(state))
        return std::make_unique<Powerset>(X);

    const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
    if (!stateDenotation.isSingular())
        addDisjunct(V, state, P);

    PowersetUniquePtr result { std::make_unique<Powerset>(m_spaceDimension, PPL::EMPTY) };
    if (!m_backwardNfa.hasPredecessors(state))
        return result;

    const int totalPredecessors { m_backwardNfa.countPredecessors(state) };
    std::vector<std::pair<int, PPLUtils::ReachPair>> predecessorReachPairs {};
    predecessorReachPairs.reserve(totalPredecessors * 2);
    for (auto edgePredecessor: m_backwardNfa.predecessors(state))
    {
        const int predecessor { static_cast<int>(edgePredecessor.dst) };
        const auto& predecessorVisitedPatches { getVisitedPatches(V, predecessor) };
        PPLUtils::ReachPairs reachPairs { computeReachPairs(predecessor, predecessorVisitedPatches, X).second };
        for (auto&& reachPair: reachPairs)
            predecessorReachPairs.emplace_back(predecessor, std::move(reachPair));
    }

    if (predecessorReachPairs.empty()) return result;

    std::vector<std::future<PowersetUniquePtr>> denotFutures {};
    if (predecessorReachPairs.size() > 1)
    {
        denotFutures.reserve(predecessorReachPairs.size());
        for (int i { 0 }; i < static_cast<int>(predecessorReachPairs.size() - 1); i++)
        {
            assert(i < static_cast<int>(predecessorReachPairs.size()));
            const int predecessor { predecessorReachPairs[i].first };
            const auto& [Q, Y] { predecessorReachPairs[i].second };
            const auto& task {
                PPL::make_threadable(
                    std::bind(
                        &DenotConcurrentV4::denot, this,
                        predecessor,
                        std::ref(Q),
                        std::ref(Y),
                        std::ref(V),
                        !isSing
                    )
                )
            };
            denotFutures.push_back(std::async(task));
        }
    }

    const int myReachPairIndex { static_cast<int>(predecessorReachPairs.size() - 1) };
    const int predecessor { predecessorReachPairs[myReachPairIndex].first };
    const auto& [Q, Y] { predecessorReachPairs[myReachPairIndex].second };
    PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

    for (auto& future: denotFutures)
        PPLUtils::fusion(*result, *future.get());

    return result;
}

std::pair<int, PPLUtils::ReachPairs> DenotConcurrentV4::computeReachPairs(
    int predecessor,
    const Powerset& predecessorVisitedPatches,
    const Poly& X
) const
{
    const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
    PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), predecessorVisitedPatches) };

    PPLUtils::ReachPairs reachPairs {
        predecessorStateDenotation.isSingular()
            ? PPLUtils::reach0(*A, X, m_polyhedralSystem->preFlow())
            : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->preFlow())
    };

    return { predecessor, std::move(reachPairs) };
}

Powerset& DenotConcurrentV4::getVisitedPatches(std::unordered_map<int, Powerset>& V, const int state) const
{
    return V.try_emplace(state, Powerset { m_spaceDimension, PPL::EMPTY }).first->second;
}

void DenotConcurrentV4::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_spaceDimension, PPL::EMPTY }).first->second.add_disjunct(P);
}

