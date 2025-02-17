#include "DenotRecursive.h"
#include "logger.h"
#include "reach.h"
#include "Timer.h"
#include "ppl_utils.h"

PowersetUniquePtr DenotRecursive::run()
{
    m_iterations = 0;

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

    for (const int finalState: m_backwardNfa.finalStates())
    {
        Log::log(Verbosity::trace, "\n--- Starting from final state: {} ---", finalState);

        const StateDenotation& finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

        Log::log(Verbosity::trace, "Final state: {}\n{}", finalState, finalStateDenotation.toString(m_polyhedralSystem->symbolTable()));

        PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
        PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
        int patchIndex { 1 };
        for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt)
        {
            Log::log(Verbosity::trace, "\n------ Final state {}, processing patch {}: {} ------\n",
                finalState,
                patchIndex,
                PPLOutput::toString(patchesIt->pointset(), m_polyhedralSystem->symbolTable())
            );

            std::vector V(m_backwardNfa.totalStates(), Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY });
            PowersetUniquePtr finalStatePatchResult { denot(finalState, patchesIt->pointset(), patchesIt->pointset(), V, 0, true) };

            Log::log(Verbosity::trace, "Final state {}, patch {} result: {}",
                finalState,
                patchIndex,
                PPLOutput::toString(*finalStatePatchResult, m_polyhedralSystem->symbolTable())
            );

            PPLUtils::fusion(*finalStateResult, *finalStatePatchResult);

            Log::log(Verbosity::trace, "------ Final state {}, result updated: {} ------",
                finalState,
                PPLOutput::toString(*finalStateResult, m_polyhedralSystem->symbolTable())
            );
        }

        PPLUtils::fusion(*result, *finalStateResult);

        Log::log(Verbosity::trace, "Update global result: {}", PPLOutput::toString(*result, m_polyhedralSystem->symbolTable()));
        Log::log(Verbosity::trace, "--- Final state {} completed ---\n", finalState);
    }

    return result;
}

PowersetUniquePtr DenotRecursive::denot(
    int state,
    const Poly& P,
    const Poly& X,
    std::vector<Powerset> V,
    const int recursionDepth,
    bool isSing
)
{
    m_iterations++;
    Log::log(Verbosity::trace, "--------- Denot iteration {} ---------", m_iterations);
    Log::log(Verbosity::trace, "Recursion Depth: {}", recursionDepth);

    assert(P.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
    assert(recursionDepth <= m_maxRecursionDepth && "Recursion depth exceeded!!!");
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

    m_backwardTrace.push(fmt::format("{}", stateDenotation.observables()));

    Log::log(Verbosity::trace, "State: {}. Denotation {}", state, stateDenotation.toString(m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "State: {}. Is initial {}", state, m_backwardNfa.isInitialState(state));
    Log::log(Verbosity::trace, "State: {}. Is final {}", state, m_backwardNfa.isFinalState(state));

    const Powerset& visitedPowerset { getVisitedPowerset(V, state) };
    Log::log(Verbosity::trace, "Visited region V[{}]: {}. Size: {}",
        state,
        PPLOutput::toString(visitedPowerset, m_polyhedralSystem->symbolTable()),
        visitedPowerset.size()
    );
    Log::log(Verbosity::trace, "P: {}", PPLOutput::toString(P, m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "X: {}\n", PPLOutput::toString(X, m_polyhedralSystem->symbolTable()));

    if (m_backwardNfa.isInitialState(state))
    {
        Log::log(Verbosity::trace, "State {} is initial, returning X: {}",
            state,
            PPLOutput::toString(X, m_polyhedralSystem->symbolTable())
        );

        saveAcceptingTrace();
        m_backwardTrace.pop();

        return std::make_unique<Powerset>(X);
    }

    if (!stateDenotation.isSingular())
    {
        Log::log(Verbosity::trace, "State {} is not singular, adding P: {}",
            state,
            PPLOutput::toString(P, m_polyhedralSystem->symbolTable())
        );

        addDisjunct(V, state, P);

        Log::log(Verbosity::trace, "State {} updated visited region: {}",
            state,
            PPLOutput::toString(X, m_polyhedralSystem->symbolTable())
        );
    }

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const auto& edgePredecessor: m_backwardNfa.predecessors(state))
    {
        int predecessor { static_cast<int>(edgePredecessor.dst) };
        const StateDenotation& predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };

        Log::log(Verbosity::trace, "\n>>> State: {} -> Processing Predecessor: {}\n{}",
            state,
            predecessor,
            predecessorStateDenotation.toString(m_polyhedralSystem->symbolTable())
        );

        const Powerset& predecessorVisitedPowerset { getVisitedPowerset(V, predecessor) };
        assert(predecessorVisitedPowerset.space_dimension() == m_polyhedralSystem->spaceDimension());

        Log::log(Verbosity::trace, "Visited region predecessor V[{}]: {}. Size: {}",
            predecessor,
            PPLOutput::toString(predecessorVisitedPowerset, m_polyhedralSystem->symbolTable()),
            predecessorVisitedPowerset.size()
        );

        PowersetUniquePtr A { PPLUtils::minus(*predecessorStateDenotation.denotation(), predecessorVisitedPowerset) };
        if (A->is_empty()) continue;

        assert(A->space_dimension() == m_polyhedralSystem->spaceDimension());
        assert(A->space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
        Log::log(Verbosity::trace, "Predecessor denotation minus visited region (A): {}, Size: {}",
            PPLOutput::toString(*A, m_polyhedralSystem->symbolTable()), A->size());

        Timer timer {};
        Log::log(Verbosity::trace, "Calling reach operator (isSing = {}).", isSing);
        PPLUtils::ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? PPLUtils::reach0(*A, X, m_polyhedralSystem->preFlow())
                : PPLUtils::reachPlus(*A, X, m_polyhedralSystem->preFlow())
        };
        Log::log(Verbosity::trace, "Reach pairs computed (size: {}). Elapsed time: {} s.", reachPairs.size(), timer.elapsedInSeconds());

        int reachPairIndex { 1 };
        for (const auto& [Q, Y]: reachPairs)
        {
            Log::log(Verbosity::trace, "\nReach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
            Log::log(Verbosity::trace, "Q: {}, Y: {}",
                PPLOutput::toString(Q, m_polyhedralSystem->symbolTable()),
                PPLOutput::toString(Y, m_polyhedralSystem->symbolTable())
            );

            assert(Q.space_dimension() == m_polyhedralSystem->spaceDimension());
            assert(Y.space_dimension() == m_polyhedralSystem->spaceDimension());

            PowersetUniquePtr denotResult { denot(predecessor, Q, Y, V, recursionDepth + 1, !isSing) };

            Log::log(Verbosity::trace, "Reach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
            Log::log(Verbosity::trace, "Result: {}", PPLOutput::toString(*denotResult, m_polyhedralSystem->symbolTable()));

            PPLUtils::fusion(*result, *denotResult);

            Log::log(Verbosity::trace, "Update result denot iteration {}: {}",
                m_iterations,
                PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
            );

            reachPairIndex++;
        }

        Log::log(Verbosity::trace, "<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
    }

    Log::log(Verbosity::trace, "Returning result: {}. Size: {}",
        PPLOutput::toString(*result, m_polyhedralSystem->symbolTable()),
        result->size()
    );

    m_backwardTrace.pop();
    return result;
}

void DenotRecursive::addDisjunct(std::vector<Powerset>& V, const int state, const Poly& P)
{
    V[state].add_disjunct(P);
}

const Powerset& DenotRecursive::getVisitedPowerset(std::vector<Powerset>& V, const int state)
{
    return V[state];
}

void DenotRecursive::saveAcceptingTrace()
{
    std::stack backwardTrace { m_backwardTrace };
    std::vector<std::string> acceptingTrace {};
    acceptingTrace.reserve(backwardTrace.size());
    while (!backwardTrace.empty())
    {
        acceptingTrace.push_back(backwardTrace.top());
        backwardTrace.pop();
    }
    m_acceptingTraces.push_back(acceptingTrace);
}
