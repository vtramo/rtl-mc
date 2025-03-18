#include "spot_utils.h"
#include "formula.h"
#include <spot/twa/formula2bdd.hh>

namespace SpotUtils
{

    std::string toOptimizationLevelString(const spot::postprocessor::optimization_level optimizationLevel)
    {
        switch (optimizationLevel)
        {
        case spot::postprocessor::Low:
            return "Low";
        case spot::postprocessor::Medium:
            return "Medium";
        case spot::postprocessor::High:
            return "High";
        }

        return "Unknown";
    }

    spot::atomic_prop_set extractMintermsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard)
    {
        spot::formula formula = spot::bdd_to_formula(guard, twaGraph->get_dict());
        minterms_of minterms { guard, twaGraph->ap_vars() };
        spot::atomic_prop_set labels { };

        for (const bdd& minterm: minterms)
        {
            spot::formula mintermFormula { bdd_to_formula(minterm, twaGraph->get_dict()) };
            for (const spot::formula& label: collectPositiveLiterals(std::move(mintermFormula)))
            {
                labels.insert(std::move(label));
            }
        }

        return labels;
    }

    spot::twa_graph_ptr transpose(const spot::twa_graph_ptr& twaGraph)
    {
        unsigned totalStates { twaGraph->num_states() };
        spot::twa_graph_ptr transposedTwaGraph { spot::make_twa_graph(twaGraph->get_dict()) };
        transposedTwaGraph->copy_ap_of(twaGraph);
        transposedTwaGraph->new_states(totalStates);

        for (unsigned state { 0 }; state < totalStates; ++state)
        {
            for (const auto& edge: twaGraph->out(state))
            {
                transposedTwaGraph->new_edge(edge.dst, state, edge.cond, edge.acc);
            }
        }

        return transposedTwaGraph;
    }

    std::unordered_set<unsigned> collectAcceptingStates(spot::const_twa_graph_ptr twa)
    {
        std::unordered_set<unsigned> acceptingStates {};
        for (unsigned nfaState { 0 }; nfaState < twa->num_states(); ++nfaState)
        {
            if (twa->state_is_accepting(nfaState))
            {
                acceptingStates.insert(nfaState);
            }
        }
        return acceptingStates;
    }
}