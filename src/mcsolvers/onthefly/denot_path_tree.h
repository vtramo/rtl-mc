#pragma once

#include <spot/twa/formula2bdd.hh>

#include "DenotPathNode.h"
#include "BackwardNFA.h"
#include <spot/twa/twagraph.hh>

/**
 * \brief Builds a path tree automaton from a set of denot paths
 * \param backwardNfa The \ref BackwardNFA used as a basis for the path tree construction
 * \param paths Vector of vectors containing the denot paths to be integrated into the tree
 * \return spot::twa_graph_ptr The constructed path tree automaton with states and transitions from the paths
 */
inline spot::twa_graph_ptr buildDenotPathTree(
    const BackwardNFA& backwardNfa,
    const std::vector<std::vector<DenotPathNode>>& paths
)
{
    spot::const_twa_graph_ptr backwardNfaTwa{backwardNfa.twa()};
    spot::bdd_dict_ptr dict{backwardNfaTwa->get_dict()};
    spot::twa_graph_ptr pathTree{spot::make_twa_graph(dict)};
    pathTree->copy_ap_of(backwardNfaTwa);
    pathTree->new_states(backwardNfaTwa->num_states());

    std::unordered_set<std::string> edges{};
    for (const std::vector<DenotPathNode>& path : paths)
    {
        assert(path.size() > 0);

        if (path.size() == 0)
        {
            continue;
        }

        if (path.size() == 1)
        {
            const DenotPathNode& node{path[0]};
            std::string edgeStr{std::to_string(node.state()) + ", " + std::to_string(node.state())};
            if (edges.count(edgeStr)) continue;
            pathTree->new_acc_edge(node.state(), node.state(), spot::formula_to_bdd(node.observable(), dict, pathTree), true);
            edges.insert(edgeStr);
            continue;
        }

        const DenotPathNode* prevNode{&path[0]};
        for (unsigned i{1}; i < path.size(); ++i)
        {
            const DenotPathNode& node{path[i]};
            std::string edgeStr{std::to_string(prevNode->state()) + ", " + std::to_string(node.state())};
            if (!edges.count(edgeStr))
            {
                pathTree->new_acc_edge(prevNode->state(), node.state(), spot::formula_to_bdd(node.observable(), dict, pathTree), i == 1);
                edges.insert(edgeStr);
            }
            prevNode = &node;
        }
    }

    unsigned initialState{backwardNfaTwa->get_init_state_number()};
    pathTree->set_init_state(initialState);
    for (unsigned acceptingState : backwardNfa.acceptingStates())
    {
        const StateDenotation& acceptingStateDenotation{backwardNfa.stateDenotation(acceptingState)};
        pathTree->new_acc_edge(initialState, acceptingState, spot::formula_to_bdd(acceptingStateDenotation.formula(), dict, pathTree), true);
    }

    return pathTree;
}
