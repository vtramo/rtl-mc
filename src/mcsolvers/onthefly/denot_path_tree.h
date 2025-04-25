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
    pathTree->prop_state_acc(spot::trival { true });
    pathTree->set_acceptance(spot::acc_cond { 1, spot::acc_cond::acc_code::buchi() });

    std::unordered_map<DenotPathNode, std::string> nameByNode{};
    auto namedPathTree { pathTree->create_namer<std::string>() };
    std::unordered_set<const DenotPathNode*> acceptingNodes{};
    std::map<unsigned, unsigned>* highlightStates { new std::map<unsigned, unsigned>() };
    constexpr unsigned RED { 5 };
    for (const auto& path: paths)
    {
        unsigned nodeCounter { 1 };
        for (const auto& denotNode: path)
        {
            const int state { denotNode.state() };
            std::string stateName { std::to_string(state) + "\n" };
            stateName += ("P = " + std::string{denotNode.patchString()} + "\n");
            stateName += ("X = " + std::string{denotNode.reachPointsString()});
            const unsigned namedState { namedPathTree->new_state(stateName) };
            nameByNode[denotNode] = stateName;

            const bool isFirst { nodeCounter == 1 };
            const bool isLast { nodeCounter == path.size() };
            if (isFirst)
            {
                acceptingNodes.insert(&denotNode);
            }

            if (isLast)
            {
                highlightStates->insert({ namedState, RED });
            }

            ++nodeCounter;
        }
    }

    std::unordered_set<std::string> edges {};
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
            auto nodeName { nameByNode.at(node) };
            std::string edgeName { nodeName + " -> " + nodeName };
            if (!edges.count(edgeName))
            {
                namedPathTree->new_edge(nodeName, nodeName, spot::formula_to_bdd(node.observable(), dict, pathTree), spot::acc_cond::mark_t { 0 });
                edges.insert(edgeName);
            }
            continue;
        }

        auto prevNodeName { nameByNode.at(path[0]) };
        for (unsigned i{1}; i < path.size(); ++i)
        {
            const DenotPathNode& node{path[i]};
            auto nodeName { nameByNode.at(node) };
            std::string edgeName { prevNodeName + " -> " + nodeName };

            if (!edges.count(edgeName))
            {
                namedPathTree->new_edge(
                    prevNodeName,
                    nodeName,
                    spot::formula_to_bdd(node.observable(), dict, pathTree),
                    i == 1 ? spot::acc_cond::mark_t { 0 } : spot::acc_cond::mark_t {}
                );
                edges.insert(edgeName);
            }

            prevNodeName = nodeName;
        }
    }

    std::string initialStateName { "Dummy Initial State" };
    unsigned initialState { namedPathTree->new_state(initialStateName) };
    for (const auto& acceptingNode: acceptingNodes)
    {
        std::string acceptingNodeName { nameByNode.at(*acceptingNode) };
        std::string edgeName { initialStateName + " -> " + acceptingNodeName };
        if (!edges.count(edgeName))
        {
            namedPathTree->new_edge(
                initialStateName,
                acceptingNodeName,
                spot::formula_to_bdd(acceptingNode->observable(), dict, pathTree),
                spot::acc_cond::mark_t {}
            );
            edges.insert(edgeName);
        }
    }
    std::vector<std::string>* stateToName{new std::vector<std::string>(namedPathTree->state_to_name)};
    pathTree->set_named_prop("state-names", stateToName);
    pathTree->set_named_prop("highlight-states", highlightStates);
    pathTree->set_init_state(initialState);
    return pathTree;
}
