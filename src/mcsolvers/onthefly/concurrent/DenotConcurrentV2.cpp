#include "DenotConcurrentV2.h"

PowersetUniquePtr DenotConcurrentV2::run()
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

PowersetUniquePtr DenotConcurrentV2::denot(
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

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
    {
        int predecessor { static_cast<int>(edgePredecessor.dst) };
        const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };

        Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
        assert(visitedPowerset.space_dimension() == m_polyhedralSystem->spaceDimension());

        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
        assert(A->space_dimension() == m_polyhedralSystem->spaceDimension());
        assert(A->space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

        ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? reach0(*A, X, m_polyhedralSystem->preFlow())
                : reachPlus(*A, X, m_polyhedralSystem->preFlow())
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
            const auto& task {
                PPL::make_threadable(
                    std::bind(
                        &DenotConcurrentV2::denot, this,
                        predecessor,
                        std::ref(Q),
                        std::ref(Y),
                        std::ref(V),
                        !isSing
                    )
                )
            };
            futures.push_back(std::async(task));
        }

        const Poly& Q { reachPairs[0].first };
        const Poly& Y { reachPairs[0].second };
        PPLUtils::fusion(*result, *denot(predecessor, Q, Y, V, !isSing));

        for (auto& future: futures)
            PPLUtils::fusion(*result, *future.get());
    }

    return result;
}

void DenotConcurrentV2::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
}

Powerset& DenotConcurrentV2::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
{
    return V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second;
}