#include "DenotRecursive.h"
#include "logger.h"
#include "reach.h"
#include "Timer.h"
#include "ppl_utils.h"

std::vector<Powerset> DenotRecursive::initializeUnvisitedMap() const
{
    std::vector<Powerset> V {};
    V.resize(m_backwardNfa.totalStates());
    for (int state = 0; state < m_backwardNfa.totalStates(); state++)
        V[state] = *m_backwardNfa.stateDenotation(state).denotation();
    return V;
}

PowersetUniquePtr DenotRecursive::run() {
    m_iterations = 0;

    std::vector V { initializeUnvisitedMap() };

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };

    for (const int finalState : m_backwardNfa.finalStates()) {
        Log::log(Verbosity::trace, "\n--- Starting from final state: {} ---", finalState);

        const StateDenotation & finalStateDenotation { m_backwardNfa.stateDenotation(finalState) };

        Log::log(Verbosity::trace, "Final state: {}\n{}", finalState, finalStateDenotation.toString(m_polyhedralSystem->symbolTable()));

        PowersetConstSharedPtr denotationFinalState { finalStateDenotation.denotation() };
        PowersetUniquePtr finalStateResult { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
        int patchIndex { 1 };
        for (Powerset::const_iterator patchesIt { denotationFinalState->begin() }; patchesIt != denotationFinalState->end(); ++patchesIt) {
            Log::log(Verbosity::trace, "\n------ Final state {}, processing patch {}: {} ------\n",
                finalState,
                patchIndex,
                PPLOutput::toString(patchesIt->pointset(), m_polyhedralSystem->symbolTable())
            );

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
    const Poly & P,
    const Poly & X,
    std::vector<Powerset> V,
    const int recursionDepth,
    bool isSing
) {
    assert(recursionDepth <= m_maxRecursionDepth && "Recursion depth exceeded!!!");

    m_iterations++;
    Log::log(Verbosity::trace, "--------- Denot iteration {} ---------", m_iterations);
    Log::log(Verbosity::trace, "Recursion Depth: {}", recursionDepth);

    assert(P.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    const StateDenotation & stateDenotation { m_backwardNfa.stateDenotation(state) };
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated, state: " + state);

    Log::log(Verbosity::trace, "State: {}. Denotation size {}.", state, stateDenotation.denotation()->size());
    Log::log(Verbosity::trace, "State: {}. Denotation {}.", state, stateDenotation.toString(m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "State: {}. Is initial {}", state, m_backwardNfa.isInitialState(state));
    Log::log(Verbosity::trace, "State: {}. Is final {}", state, m_backwardNfa.isFinalState(state));

    Log::log(Verbosity::trace, "P: {}", PPLOutput::toString(P, m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "X: {}\n", PPLOutput::toString(X, m_polyhedralSystem->symbolTable()));

    if (m_backwardNfa.isInitialState(state)) {
        if (stateDenotation.isSingular()) {
            Log::log(Verbosity::trace, "State {} is initial and singular, returning X: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->symbolTable())
            );

            return std::make_unique<Powerset>(X);
        }

        PPLUtils::ReachPairs reachPairs {
            PPLUtils::reachPlus(
                Powerset { X },
                Poly { m_polyhedralSystem->spaceDimension(), PPL::UNIVERSE },
                m_polyhedralSystem->preFlow()
            )
        };
        PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
        for (const auto & [Q, Y] : reachPairs)
            result->add_disjunct(Q);

        Log::log(Verbosity::trace, "State {} is initial and open. Result size: {}. Returning: {}",
            state,
            result->size(),
            PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
        );

        return result;
    }

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const auto & edgePredecessor: m_backwardNfa.predecessors(state))
    {
        int predecessor { static_cast<int>(edgePredecessor.dst) };
        const StateDenotation & predecessorStateDenotation { m_backwardNfa.stateDenotation(predecessor) };

        Log::log(Verbosity::trace, "\n>>> State: {} -> Processing Predecessor: {}\n{}",
            state,
            predecessor,
            predecessorStateDenotation.toString(m_polyhedralSystem->symbolTable())
        );

        for (
            Powerset::iterator predecessorUnvisitedPatchIt { V[predecessor].begin() };
            predecessorUnvisitedPatchIt != V[predecessor].end();
            ++predecessorUnvisitedPatchIt
        )
        {
            Powerset A { Powerset { predecessorUnvisitedPatchIt->pointset() } };
            if (!predecessorStateDenotation.isSingular())
                predecessorUnvisitedPatchIt = V[predecessor].drop_disjunct(predecessorUnvisitedPatchIt);

            Timer timer {};
            Log::log(Verbosity::trace, "Calling reach operator (isSing = {}).", isSing);
            PPLUtils::ReachPairs reachPairs {
                predecessorStateDenotation.isSingular()
                    ? PPLUtils::reach0(A, X, m_polyhedralSystem->preFlow())
                    : PPLUtils::reachPlus(A, X, m_polyhedralSystem->preFlow())
            };
            Log::log(Verbosity::trace, "Reach pairs computed (size: {}). Elapsed time: {} s.", reachPairs.size(), timer.elapsedInSeconds());

            int reachPairIndex { 1 };
            for (const auto & [Q, Y]: reachPairs) {
                Log::log(Verbosity::trace, "\nReach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                Log::log(Verbosity::trace, "Q: {}, Y: {}",
                    PPLOutput::toString(Q, m_polyhedralSystem->symbolTable()),
                    PPLOutput::toString(Y, m_polyhedralSystem->symbolTable())
                );

                assert(Q.space_dimension() == m_polyhedralSystem->spaceDimension());
                assert(Y.space_dimension() == m_polyhedralSystem->spaceDimension());
                assert(PPLUtils::containsDisjunct(*predecessorStateDenotation.denotation(), Q));

                PowersetUniquePtr denotResult { denot(predecessor, Q, Y, V, recursionDepth + 1, !isSing) };

                Log::log(Verbosity::trace, "Reach pair {} (State: {}, Predecessor {})", reachPairIndex, state, predecessor);
                Log::log(Verbosity::trace, "(State: {}, Predecessor {}) Result size: {}. Result: {}",
                    state,
                    predecessor,
                    denotResult->size(),
                    PPLOutput::toString(*denotResult, m_polyhedralSystem->symbolTable()));

                PPLUtils::fusion(*result, *denotResult);

                Log::log(Verbosity::trace, "Update result. Denot iteration {}. Result size: {}. Result: {}",
                    m_iterations,
                    result->size(),
                    PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
                );

                reachPairIndex++;
            }
        }

        Log::log(Verbosity::trace, "<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
    }

    Log::log(Verbosity::trace, "Returning result. Result size: {}. Result: {}",
        result->size(),
        PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
    );

    return result;
}
