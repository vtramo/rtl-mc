#include "DenotOnTheFly.h"

#include <pairwise_reduce.h>


#include "DenotPathNode.h"
#include "logger.h"
#include "reach.h"
#include "Timer.h"
#include "ppl_utils.h"

PowersetUniquePtr DenotOnTheFly::run()
{
    m_iterations = 0;

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const unsigned acceptingState: m_backwardNfa->acceptingStates())
    {
        Log::log(Verbosity::trace, "\n--- Starting from accepting state: {} ---", acceptingState);

        const StateDenotation & acceptingStateDenotation { m_backwardNfa->stateDenotation(acceptingState) };

        Log::log(Verbosity::trace, "Accepting state: {}\n{}", acceptingState, acceptingStateDenotation.toString(m_polyhedralSystem->symbolTable()));

        PowersetConstSharedPtr denotationAcceptingState { acceptingStateDenotation.denotation() };
        PowersetUniquePtr acceptingStateResult { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
        int patchIndex { 1 };
        for (Powerset::const_iterator patchesIt { denotationAcceptingState->begin() }; patchesIt != denotationAcceptingState->end(); ++patchesIt)
        {
            auto P { patchesIt->pointset() };

            Log::log(Verbosity::trace, "\n------ Accepting state {}, processing patch {}: {} ------\n",
                 acceptingState,
                 patchIndex,
                 PPLOutput::toString(P, m_polyhedralSystem->symbolTable())
            );

            std::vector V(m_backwardNfa->totalStates(), Powerset { m_polyhedralSystem->spaceDimension(), PPL::EMPTY });
            PowersetUniquePtr acceptingStatePatchResult {
                denot(
                    acceptingState,
                    P,
                    P,
                    V,
                    0,
                    acceptingStateDenotation.isSingular()
                )
            };

            Log::log(Verbosity::trace, "Accepting state {}, patch {} result: {}",
                acceptingState,
                patchIndex,
                PPLOutput::toString(*acceptingStatePatchResult, m_polyhedralSystem->symbolTable())
            );

            PPLUtils::fusion(*acceptingStateResult, *acceptingStatePatchResult);

            Log::log(Verbosity::trace, "------ Accepting state {}, result updated: {} ------",
                acceptingState,
                PPLOutput::toString(*acceptingStateResult, m_polyhedralSystem->symbolTable())
            );

            ++patchIndex;
        }

        PPLUtils::fusion(*result, *acceptingStateResult);

        Log::log(Verbosity::trace, "Update global result: {}", PPLOutput::toString(*result, m_polyhedralSystem->symbolTable()));
        Log::log(Verbosity::trace, "--- Accepting state {} completed ---\n", acceptingState);
    }

    m_result = std::make_shared<Powerset>(*result);
    return result;
}

PowersetUniquePtr DenotOnTheFly::denot(
    int state,
    const Poly & P,
    const Poly & X,
    std::vector<Powerset> V,
    const int recursionDepth,
    bool isSing
)
{
    const StateDenotation& stateDenotation { m_backwardNfa->stateDenotation(state) };
    assert(isSing == stateDenotation.isSingular() && "Sing invariant violated.");

    if (m_collectPaths)
    {
        pushPathNode(P, X, stateDenotation, state);
        assert(!containsDuplicatePairs(V));
    }

    if (m_maxIterationReached)
    {
        if (m_collectPaths) popPathNode();

        return std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY);
    }

    assert(recursionDepth <= m_maxRecursionDepth && "Recursion depth exceeded!!!");

    m_iterations++;
    if (m_iterations > m_maxIterations)
    {
        Log::log(Verbosity::debug, "[Denot] Max iterations reached! {}.", m_maxIterations);
        m_maxIterationReached = true;

        if (m_collectPaths) popPathNode();

        return std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY);
    }

    Log::log(Verbosity::trace, "--------- Denot iteration {} ---------", m_iterations);
    Log::log(Verbosity::trace, "Recursion Depth: {}", recursionDepth);

    assert(P.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(P.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
    assert(X.space_dimension() == m_polyhedralSystem->spaceDimension());
    assert(X.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());

    Log::log(Verbosity::trace, "State: {}. Denotation size {}.", state, stateDenotation.denotation()->size());
    Log::log(Verbosity::trace, "State: {}. Denotation {}.", state, stateDenotation.toString(m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "State: {}. Is initial {}", state, m_backwardNfa->isInitialState(state));
    Log::log(Verbosity::trace, "State: {}. Is accepting {}", state, m_backwardNfa->isAcceptingState(state));

    const Powerset& visitedPowerset { getVisitedPowerset(V, state) };
    Log::log(Verbosity::trace, "Visited region. Size: {}. V[{}]: {}.",
        visitedPowerset.size(),
        state,
        PPLOutput::toString(visitedPowerset, m_polyhedralSystem->symbolTable())
    );
    Log::log(Verbosity::trace, "P: {}", PPLOutput::toString(P, m_polyhedralSystem->symbolTable()));
    Log::log(Verbosity::trace, "X: {}\n", PPLOutput::toString(X, m_polyhedralSystem->symbolTable()));

    if (m_backwardNfa->isInitialState(state))
    {
        if (m_collectPaths) addCurrentPath();

        if (stateDenotation.isSingular())
        {
            Log::log(Verbosity::trace, "State {} is initial and singular, returning X: {}",
                state,
                PPLOutput::toString(X, m_polyhedralSystem->symbolTable())
            );

            if (m_collectPaths) popPathNode();

            return std::make_unique<Powerset>(X);
        }

        PowersetUniquePtr result {
            reachPlus(
                Powerset { X },
                Powerset { m_polyhedralSystem->spaceDimension(), PPL::UNIVERSE },
                m_polyhedralSystem->preFlow()
            )
        };
        m_totalReachCalls++;

        Log::log(Verbosity::trace, "State {} is initial and open. Result size: {}. Returning: {}",
            state,
            result->size(),
            PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
        );

        if (m_collectPaths) popPathNode();

        return result;
    }

    if (!stateDenotation.isSingular())
    {
        addDisjunct(V, state, P);
    }

    PowersetUniquePtr result { std::make_unique<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (const auto & edgePredecessor: m_backwardNfa->successors(state))
    {
        unsigned predecessor { edgePredecessor.dst };
        assert(predecessor != state);

        const StateDenotation & predecessorStateDenotation { m_backwardNfa->stateDenotation(predecessor) };

        Log::log(Verbosity::trace, "\n>>> State: {} -> Processing Predecessor: {}\n{}",
            state,
            predecessor,
            predecessorStateDenotation.toString(m_polyhedralSystem->symbolTable())
        );

        const Powerset & predecessorVisitedPowerset { getVisitedPowerset(V, predecessor) };
        assert(predecessorVisitedPowerset.space_dimension() == m_polyhedralSystem->spaceDimension());

        Log::log(Verbosity::trace, "Visited region predecessor V[{}]: {}. Size: {}",
            predecessor,
            PPLOutput::toString(predecessorVisitedPowerset, m_polyhedralSystem->symbolTable()),
            predecessorVisitedPowerset.size()
        );

        Powerset A { *predecessorStateDenotation.denotation() };
        for (Powerset::const_iterator it2 { predecessorVisitedPowerset.begin() }; it2 != predecessorVisitedPowerset.end(); ++it2)
        {
            for (Powerset::iterator it1 { A.begin() }; it1 != A.end(); ++it1)
            {
                if (it1->pointset() == it2->pointset())
                {
                    A.drop_disjunct(it1);
                    break;
                }
            }
        }

        if (A.is_empty())
        {
            continue;
        }

        assert(A.space_dimension() == m_polyhedralSystem->spaceDimension());
        assert(A.space_dimension() == m_polyhedralSystem->preFlow().space_dimension());
        Log::log(Verbosity::trace, "Predecessor denotation minus visited region (A): {}, Size: {}",
            PPLOutput::toString(A, m_polyhedralSystem->symbolTable()), A.size());

        Timer timer {};
        Log::log(Verbosity::trace, "Calling reach operator (isSing = {}).", isSing);
        ReachPairs reachPairs {
            predecessorStateDenotation.isSingular()
                ? reach0(A, X, m_polyhedralSystem->preFlow())
                : reachPlus(A, X, m_polyhedralSystem->preFlow())
        };
        Log::log(Verbosity::trace, "Reach pairs computed (size: {}). Elapsed time: {} s.", reachPairs.size(), timer.elapsedInSeconds());
        m_totalReachCalls++;

        int reachPairIndex { 1 };
        for (const auto & [Q, Y]: reachPairs)
        {
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

        Log::log(Verbosity::trace, "<<< State: {} -> Predecessor Completed: {}\n", state, predecessor);
    }

    Log::log(Verbosity::trace, "Returning result. Result size: {}. Result: {}",
        result->size(),
        PPLOutput::toString(*result, m_polyhedralSystem->symbolTable())
    );

    if (m_collectPaths) popPathNode();

    return result;
}

bool DenotOnTheFly::containsDuplicatePairs(const std::vector<Powerset>& V) const
{
    std::unordered_set<std::string> stateOpenPatchStrings {};
    unsigned totSingularStates { 0 };
    for (const auto& denotNode: m_currentPath)
    {
        int state { denotNode.state() };
        const StateDenotation& stateDenotation { m_backwardNfa->stateDenotation(state) };
        if (stateDenotation.isSingular())
        {
            totSingularStates++;
            continue;
        }

        std::string openPatchString { denotNode.patchString() };
        stateOpenPatchStrings.insert(std::to_string(state) + " " + openPatchString);
    }

    const bool duplicateFound { stateOpenPatchStrings.size() != (m_currentPath.size() - totSingularStates) };
    if (duplicateFound)
    {
        std::cerr << m_currentPath << std::endl;
        for (const auto& denotNode: m_currentPath)
        {
            int state { denotNode.state() };
            const Powerset& visitedPowerset { V[state] };
            std::cerr << "V[" << state << "]: " << PPLOutput::toString(visitedPowerset, m_polyhedralSystem->symbolTable()) << std::endl;
        }
    }

    return duplicateFound;
}

void DenotOnTheFly::addDisjunct(std::vector<Powerset> & V, const int state, const Poly & P) const
{
    Log::log(Verbosity::trace, "State {} is not singular, adding P: {}",
        state,
        PPLOutput::toString(P, m_polyhedralSystem->symbolTable())
    );

    V[state].add_disjunct(P);

    Log::log(Verbosity::trace, "V[{}]: {}",
        state,
        PPLOutput::toString(V[state], m_polyhedralSystem->symbolTable())
    );
}

const Powerset & DenotOnTheFly::getVisitedPowerset(std::vector<Powerset> & V, const int state)
{
    return V[state];
}

void DenotOnTheFly::pushPathNode(const Poly& P, const Poly& X, const StateDenotation& stateDenotation, const int state)
{
    m_currentPath.push_back(DenotPathNode { state, P, X, stateDenotation.formula(), m_polyhedralSystem->symbolTable() });
}

DenotPathNode DenotOnTheFly::popPathNode()
{
    if (m_currentPath.empty())
    {
        throw std::invalid_argument("Cannot pop from empty path.");
    }

    auto denotPathNode { m_currentPath.back() };
    m_currentPath.pop_back();
    return denotPathNode;
}

void DenotOnTheFly::addCurrentPath()
{
    const DenotPathNode& lastNode { m_currentPath.back() };
    const auto& [_, inserted] { m_allContributionsAsStrings.insert(std::string { lastNode.reachPointsString() }) };
    if (!inserted) m_totalRedundantPaths++;
    m_paths.push_back(m_currentPath);
}

