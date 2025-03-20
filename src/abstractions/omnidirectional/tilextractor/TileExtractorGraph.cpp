#include "TileExtractorGraph.h"

#include <adjacency.h>
#include <spot/twa/bdddict.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/sccinfo.hh>

std::vector<Tile> TileExtractorGraph::extractTiles(const std::vector<Observable>& observables)
{
    return TileExtractor::extractTiles(observables);
}

std::vector<Tile> TileExtractorGraph::extractTiles(const Observable& observable)
{
    spot::bdd_dict_ptr bddDict { spot::make_bdd_dict() };
    spot::twa_graph_ptr graph { std::make_shared<spot::twa_graph>(bddDict) };

    graph->new_states(observable.totalPatches());
    std::vector<std::reference_wrapper<const Poly>> patchByState {};
    patchByState.reserve(observable.totalPatches());
    Powerset::const_iterator patchIterator { observable.begin() };
    for (int patchState { 0 }; patchState < observable.totalPatches(); ++patchState)
    {
        const Poly& patch { patchIterator->pointset() };
        patchByState.push_back(patch);
        ++patchIterator;
    }

    for (int patchState1 { 0 }; patchState1 < observable.totalPatches(); ++patchState1)
    {
        for (int patchState2 { 0 }; patchState2 < observable.totalPatches(); ++patchState2)
        {
            if (areAdjacent(patchByState[patchState1], patchByState[patchState2]))
            {
                graph->new_edge(patchState1, patchState2, bdd_true());
                graph->new_edge(patchState2, patchState1, bdd_true());
            }
        }
    }

    spot::scc_info sccInfo { graph, spot::scc_info_options::ALL | spot::scc_info_options::PROCESS_UNREACHABLE_STATES };
    std::vector<Tile> tiles {};
    tiles.reserve(sccInfo.scc_count());
    for (spot::scc_info_node scc: sccInfo)
    {
        PowersetSharedPtr patches { std::make_shared<Powerset>(observable.spaceDimension(), PPL::EMPTY) };

        for (unsigned sccState: scc.states())
        {
            const auto& patch { patchByState.at(sccState) };
            patches->add_disjunct(patch);
        }

        Tile tile { observable, patches };
        tiles.push_back(std::move(tile));
    }
    return tiles;
}
