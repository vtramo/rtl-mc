#pragma once

#include <spot/tl/apcollect.hh>
#include <spot/twaalgos/postproc.hh>

namespace SpotUtils
{
    std::string toOptimizationLevelString(spot::postprocessor::optimization_level optimizationLevel);
    spot::atomic_prop_set extractMintermsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard);
    spot::twa_graph_ptr transpose(const spot::twa_graph_ptr& twaGraph);
    std::unordered_set<unsigned> collectAcceptingStates(spot::const_twa_graph_ptr twa);
}
