#include "DenotRecursive.h"
#include "logger.h"
#include "reach.h"
#include "Timer.h"
#include "ppl_utils.h"

PowersetUniquePtr DenotRecursive::run()
{
    m_iterations = 0;

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };

    for (const int finalState: m_backwardNfa.finalStates())
    {
        Log::log(Verbosity::trace, "\n--- Starting from final state: {} ---", finalState);

        const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

        Log::log(Verbosity::trace, "Final state: {}\n{}", finalState, finalStateDenotation.toString(m_polyhedralSystem->getSymbolTable()));

        PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
        PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
        int patchIndex { 1 };
        for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
        {
            Log::log(Verbosity::trace, "\n------ Final state {}, processing patch {}: {} ------\n",
                finalState,
                patchIndex,
                PPLOutput::toString(patchesIt->pointset(), m_polyhedralSystem->getSymbolTable())
            );

            std::unordered_map<int, Powerset> V {};
            PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), V, 0, true) };

            Log::log(Verbosity::trace, "Final state {}, patch {} result: {}",
                finalState,
                patchIndex,
                PPLOutput::toString(*finalStatePatchResult, m_polyhedralSystem->getSymbolTable())
            );

            PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);

            Log::log(Verbosity::trace, "------ Final state {}, result updated: {} ------",
                finalState,
                PPLOutput::toString(*finalStateResult, m_polyhedralSystem->getSymbolTable())
            );
        }

        PPLUtils::fusion(*result, *finalStateResult);

        Log::log(Verbosity::trace, "Update global result: {}", PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable()));
        Log::log(Verbosity::trace, "--- Final state {} completed ---\n", finalState);
    }

    return result;
}

PowersetUniquePtr DenotRecursive::denot(
    int state,
    const Poly& P,
    const Poly& X,
    std::unordered_map<int, Powerset> V,
    const int recursionDepth,
    bool isSing
)
{
    assert(recursionDepth <= m_maxRecursionDepth && "Recursion depth exceeded!!!");

    m_iterations++;
    Log::log(Verbosity::trace, "--------- Denot iteration {} ---------", m_iterations);

    assert(P.space_dimension() == m_polyhedralSystem->getSpaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->getSpaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());

    const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

    Log::log(Verbosity::trace, "State: {}\n{}", state, stateDenotation.toString(m_polyhedralSystem->getSymbolTable()));
    Log::log(Verbosity::trace, "Is initial {}", m_backwardNfa.isInitialState(state));
    Log::log(Verbosity::trace, "Is final {}", m_backwardNfa.isFinalState(state));
    Log::log(Verbosity::trace, "Visited region V[{}]: {}",
        state,
        PPLOutput::toString(getVisitedPowerset(V, state), m_polyhedralSystem->getSymbolTable())
    );
    Log::log(Verbosity::trace, "P: {}", PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable()));
    Log::log(Verbosity::trace, "X: {}\n", PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable()));

    if (m_backwardNfa.isInitialState(state))
    {
        Log::log(Verbosity::trace, "State {} is initial, returning X: {}",
            state,
            PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
        );

        return std::make_unique<Powerset>(X);
    }

    if (!stateDenotation.isSingular())
    {
        Log::log(Verbosity::trace, "State {} is not singular, adding P: {}",
            state,
            PPLOutput::toString(P, m_polyhedralSystem->getSymbolTable())
        );

        addDisjunct(V, state, P);

        Log::log(Verbosity::trace, "State {} updated visited region: {}",
            state,
            PPLOutput::toString(X, m_polyhedralSystem->getSymbolTable())
        );
    }

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY) };
    for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
    {
        int predecessor { static_cast<int>(edgePredecessor.dst) };
        const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };
        Log::log(Verbosity::trace, "\n>>> State: {} -> Processing Predecessor: {}\n{}",
            state,
            predecessor,
            predecessorStateDenotation.toString(m_polyhedralSystem->getSymbolTable())
        );

        Powerset& visitedPowerset { getVisitedPowerset(V, predecessor) };
        assert(visitedPowerset.space_dimension() == m_polyhedralSystem->getSpaceDimension());
        Log::log(Verbosity::trace, "Visited region V[{}]: {}",
            predecessor,
            PPLOutput::toString(visitedPowerset, m_polyhedralSystem->getSymbolTable())
        );

        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), visitedPowerset) };
        assert(A->space_dimension() == m_polyhedralSystem->getSpaceDimension());
        assert(A->space_dimension() == m_polyhedralSystem->getPreFlow().space_dimension());
        Log::log(Verbosity::trace, "Predecessor denotation minus visited region (A): {}",
            PPLOutput::toString(*A, m_polyhedralSystem->getSymbolTable()));

        Timer timer {};
        Log::log(Verbosity::trace, "Calling reach operator (isSing = {}).", isSing);
        PPLUtils::ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? PPLUtils::reach0(*A, X, m_polyhedralSystem->getPreFlow())
                : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->getPreFlow())
        };
        Log::log(Verbosity::trace, "Reach pairs computed (size: {}). Elapsed time: {} s.", reachPairs.size(), timer.elapsedInSeconds());

        int reachPairIndex { 1 };
        for (const auto& [Q, Y]: reachPairs)
        {
            Log::log(Verbosity::trace, "\nReach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
            Log::log(Verbosity::trace, "Q: {}\nY: {}",
                PPLOutput::toString(Q, m_polyhedralSystem->getSymbolTable()),
                PPLOutput::toString(Y, m_polyhedralSystem->getSymbolTable())
            );

            assert(Q.space_dimension() == m_polyhedralSystem->getSpaceDimension());
            assert(Y.space_dimension() == m_polyhedralSystem->getSpaceDimension());

            PowersetUniquePtr denotResult { denot(predecessor, Q, Y, V, recursionDepth + 1, !isSing) };

            Log::log(Verbosity::trace, "Reach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
            Log::log(Verbosity::trace, "Result: {}", PPLOutput::toString(*denotResult, m_polyhedralSystem->getSymbolTable()));

            PPLUtils::fusion(*result, *denotResult);

            Log::log(Verbosity::trace, "Update result denot iteration {}: {}",
                m_iterations,
                PPLOutput::toString(*result, m_polyhedralSystem->getSymbolTable())
            );

            reachPairIndex++;
        }

        Log::log(Verbosity::trace, "<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
    }

    return result;
}

void DenotRecursive::addDisjunct(std::unordered_map<int, Powerset>& V, const int state, const Poly& P) const
{
    V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second.add_disjunct(P);
}

Powerset& DenotRecursive::getVisitedPowerset(std::unordered_map<int, Powerset>& V, const int state) const
{
    return V.try_emplace(state, Powerset { m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY }).first->second;
}