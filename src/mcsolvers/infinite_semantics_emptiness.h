#pragma once

#include "PolyhedralSynchronousProductAutomaton.h"
#include <spot/twaalgos/se05.hh>

struct EmptinessCheckDenotationResult
{
    std::vector<spot::twa_run> acceptingRuns {};
    int totalAcceptingRuns { 0 };
    bool isEmpty {};
    PowersetSharedPtr result {};
};

inline EmptinessCheckDenotationResult explicitSe05Search(PolyhedralSynchronousProductAutomatonConstSharedPtr synchronousProductAutomaton)
{
    PowersetSharedPtr result { std::make_shared<Powerset>(synchronousProductAutomaton->spaceDimension(), PPL::EMPTY) };
    spot::emptiness_check_ptr explicitSe05Search { spot::explicit_se05_search(synchronousProductAutomaton->twa()) };

    std::vector<spot::twa_run> acceptingRuns {};
    spot::emptiness_check_result_ptr emptinessCheckResult { explicitSe05Search->check() };
    while (emptinessCheckResult != nullptr)
    {
        spot::twa_run_ptr twaRun { emptinessCheckResult->accepting_run() };
        acceptingRuns.push_back(*twaRun);

        std::list<spot::twa_run::step>::iterator prefixStepIterator { twaRun->prefix.begin() };
        ++prefixStepIterator; // skip dummy initial state

        const spot::state* initialState { prefixStepIterator->s };
        const unsigned initialStateNumber { synchronousProductAutomaton->twa()->state_number(initialState) };
        assert(synchronousProductAutomaton->isInitialState(initialStateNumber));

        PowersetConstSharedPtr initialStatePoints { synchronousProductAutomaton->points(initialStateNumber) };
        PPLUtils::fusion(*result, *initialStatePoints);

        emptinessCheckResult = explicitSe05Search->check();
    }

    return { acceptingRuns, static_cast<int>(acceptingRuns.size()), acceptingRuns.size() == 0, result };
}