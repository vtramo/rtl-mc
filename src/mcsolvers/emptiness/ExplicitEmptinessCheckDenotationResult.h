#pragma once

#include <spot/twaalgos/emptiness.hh>
#include "ppl_utils.h"

struct ExplicitEmptinessCheckDenotationResult
{
    std::vector<spot::twa_run> acceptingRuns {};
    std::set<unsigned> initialStates {};
    int totalAcceptingRuns {};
    bool isEmpty {};
    PowersetSharedPtr result {};
    double elapsedTimeInSeconds {};
};