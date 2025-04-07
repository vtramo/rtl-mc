#pragma once

#include "PolyhedralSynchronousProductAutomaton.h"
#include <spot/twaalgos/se05.hh>
#include <spot/twaalgos/tau03.hh>
#include <spot/twaalgos/magic.hh>

enum class EmptinessCheckAlgorithm
{
    se05,
    tau03,
    magic
};

static spot::emptiness_check_ptr emptinessCheckAlgorithm(
    const EmptinessCheckAlgorithm algorithm,
    spot::const_twa_graph_ptr twa
)
{
    switch (algorithm)
    {
    case EmptinessCheckAlgorithm::se05:
        return spot::explicit_se05_search(twa);
    case EmptinessCheckAlgorithm::tau03:
        return spot::explicit_tau03_search(twa);
    case EmptinessCheckAlgorithm::magic:
        return spot::explicit_magic_search(twa);
    }

    throw std::invalid_argument("Invalid EmptinessCheckAlgorithm");
}

struct EmptinessCheckDenotationResult
{
    std::vector<spot::twa_run> acceptingRuns {};
    std::set<unsigned> initialStates {};
    int totalAcceptingRuns { 0 };
    bool isEmpty {};
    PowersetSharedPtr result {};
    double elapsedTimeInSeconds {};
};

inline EmptinessCheckDenotationResult emptinessCheckDenotationSearch(
    PolyhedralSynchronousProductAutomatonConstSharedPtr synchronousProductAutomaton,
    const EmptinessCheckAlgorithm algorithm = EmptinessCheckAlgorithm::magic
)
{
    Timer timer {};
    PowersetSharedPtr result { std::make_shared<Powerset>(synchronousProductAutomaton->spaceDimension(), PPL::EMPTY) };
    spot::emptiness_check_ptr emptinessCheckSearch { emptinessCheckAlgorithm(algorithm, synchronousProductAutomaton->twa()) };

    std::vector<spot::twa_run> acceptingRuns {};
    std::set<unsigned> initialStates {};
    spot::emptiness_check_result_ptr emptinessCheckResult { emptinessCheckSearch->check() };
    while (emptinessCheckResult != nullptr)
    {
        spot::twa_run_ptr twaRun { emptinessCheckResult->accepting_run() };
        acceptingRuns.push_back(*twaRun);

        std::list<spot::twa_run::step>::iterator prefixStepIterator { twaRun->prefix.begin() };
        ++prefixStepIterator; // skip dummy initial state

        const spot::state* initialState { prefixStepIterator->s };
        const unsigned initialStateNumber { synchronousProductAutomaton->twa()->state_number(initialState) };
        initialStates.insert(initialStateNumber);
        assert(synchronousProductAutomaton->isInitialState(initialStateNumber));

        PowersetConstSharedPtr initialStatePoints { synchronousProductAutomaton->points(initialStateNumber) };
        PPLUtils::fusion(*result, *initialStatePoints);

        emptinessCheckResult = emptinessCheckSearch->check();
    }

    return { acceptingRuns, initialStates, static_cast<int>(acceptingRuns.size()), acceptingRuns.size() == 0, result, timer.elapsedInSeconds() };
}