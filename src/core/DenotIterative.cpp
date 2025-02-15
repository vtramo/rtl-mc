#include "DenotIterative.h"
#include "ppl_utils.h"
#include "reach.h"

PowersetUniquePtr DenotIterative::run()
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

struct DenotFrame
{
    int state {};
    Poly P {};
    Poly X {};
    std::unordered_map<int, Powerset> V {};
    const bool isSing {};
};

PowersetUniquePtr DenotIterative::denot(
    const int _state,
    const Poly& _P,
    const Poly& _X,
    std::unordered_map<int, Powerset> _V,
    const bool _isSing
)
{
    std::stack<DenotFrame> stack {};
    stack.push({ _state, _P, _X, _V, _isSing });
    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

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
                    ? PPLUtils::reach0(*A, denotFrame.X, m_polyhedralSystem->preFlow())
                    : PPLUtils::reachPlus(*A, denotFrame.X, m_polyhedralSystem->preFlow())
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

void DenotIterative::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
}

Powerset& DenotIterative::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
{
    return V.try_emplace(state, Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY }).first->second;
}