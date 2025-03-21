#include "spot_utils.h"
#include "formula.h"
#include <spot/twa/formula2bdd.hh>

namespace SpotUtils
{

    /*!
     * This function takes a `spot::postprocessor::optimization_level` and returns a string representation of the level.
     * The optimization level is used in Spot's post-processing algorithms to control the trade-off between performance
     * and the quality of the result.
     */
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

    /*!
     * This function extracts all minterms from a BDD (Binary Decision Diagram) representing the guard
     * of an edge in a Spot automaton.
     */
    std::vector<spot::formula> extractMintermsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard)
    {
        spot::formula formula = spot::bdd_to_formula(guard, twaGraph->get_dict());
        minterms_of minterms { guard, twaGraph->ap_vars() };
        std::vector<spot::formula> mintermsFormula {};

        for (const bdd& minterm: minterms)
        {
            spot::formula mintermFormula { bdd_to_formula(minterm, twaGraph->get_dict()) };
            mintermsFormula.push_back(mintermFormula);
        }

        return mintermsFormula;
    }

    /*!
     * This function transposes a given Spot automaton, effectively reversing all its edges. The resulting automaton
     * represents the reverse of the original automaton's transition relation.
     */
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

    /*!
     * This function iterates over the states of a Spot automaton and collects the indices of all states that are marked
     * as accepting.
     */
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