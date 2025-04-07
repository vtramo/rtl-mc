#pragma once

#include <spot/twaalgos/emptiness.hh>
#include "ppl_utils.h"

struct EmptinessCheckDenotationResult
{
    std::vector<spot::twa_run> acceptingRuns {};
    std::set<unsigned> initialStates {};
    int totalAcceptingRuns {};
    bool isEmpty {};
    PowersetSharedPtr result {};
    double elapsedTimeInSeconds {};
};