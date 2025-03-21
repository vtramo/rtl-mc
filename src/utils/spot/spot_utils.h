#pragma once

#include <spot/tl/apcollect.hh>
#include <spot/twaalgos/postproc.hh>

/*!
 *  \brief A namespace providing utility functions for working with Spot (a library for LTL, ω-automata manipulation and model checking).
 */
namespace SpotUtils
{
    /*!
     * \brief Converts a Spot optimization level to a human-readable string.
     * \param optimizationLevel The optimization level to convert.
     * \return A string representation of the optimization level.
     */
    std::string toOptimizationLevelString(spot::postprocessor::optimization_level optimizationLevel);


    /*!
     * \brief Extracts all minterms from an edge guard in a Spot automaton.
     * \param twaGraph The Spot automaton (as a `spot::twa_graph_ptr`) from which the guard is extracted.
     * \param guard The BDD representing the edge guard.
     * \return A `std::vector<spot::formula>` containing all minterms.
     */
    std::vector<spot::formula> extractMintermsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard);

    /*!
     * \brief Transposes a Spot automaton.
     * \param twaGraph The Spot automaton (as a `spot::twa_graph_ptr`) to transpose.
     * \return A `spot::twa_graph_ptr` representing the transposed automaton.
     */
    spot::twa_graph_ptr transpose(const spot::twa_graph_ptr& twaGraph);

    /*!
     * \brief Collects all accepting states from a Spot automaton.
     * \param twa The Spot automaton (as a `spot::const_twa_graph_ptr`) from which to collect accepting states.
     * \return An `std::unordered_set<unsigned>` containing all accepting states.
     */
    std::unordered_set<unsigned> collectAcceptingStates(spot::const_twa_graph_ptr twa);
}
