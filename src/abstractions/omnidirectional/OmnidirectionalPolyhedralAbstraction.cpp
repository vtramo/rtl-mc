#include "OmnidirectionalPolyhedralAbstraction.h"

#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/dot.hh>

#include "TileExtractor.h"

OmnidirectionalPolyhedralAbstraction::OmnidirectionalPolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem)
{
    if (!polyhedralSystem->isOmnidirectionalFlow())
        throw std::invalid_argument("To construct an OmnidirectionalPolyhedralAbstraction, the flow must be omnidirectional!");

    std::vector observables { polyhedralSystem->generateObservables() };

    TileExtractor tileExtractor {};
    std::vector tiles { tileExtractor.extractTiles(observables) };

    initializeGraph(polyhedralSystem->bddDict());
    buildAbstraction(std::move(tiles));
}

void OmnidirectionalPolyhedralAbstraction::buildAbstraction(std::vector<Tile>&& tiles)
{
    const int totalTiles { static_cast<int>(tiles.size()) };
    for (int i { 0 }; i < totalTiles; ++i)
        for (int j { 0 }; j < totalTiles; ++j)
            for (int k { 0 }; k < totalTiles; ++k)
                processTriple(tiles[i], tiles[j], tiles[k]);
}

void OmnidirectionalPolyhedralAbstraction::initializeGraph(spot::bdd_dict_ptr bddDict)
{
    m_graph = std::make_shared<spot::twa_graph>(bddDict);
    m_graph->prop_state_acc(spot::trival { true });
    m_graph->set_acceptance(spot::acc_cond::acc_code::t());
}

void OmnidirectionalPolyhedralAbstraction::processTriple(const Tile& tile1, const Tile& tile2, const Tile& tile3)
{
    TripleTileNode qTripleTileNode { tile1, tile2, tile3 };
    if (qTripleTileNode.isEmpty()) return;

    unsigned qState { m_graph->new_state() };
    m_tileNodes.insert(std::make_pair(qState, qTripleTileNode));

    unsigned pState { getStateByTileOrCreate(tile1) };
    unsigned rState { getStateByTileOrCreate(tile3) };
    TileNode pTileNode { tile1 };
    TileNode rTileNode { tile3 };
    m_tileNodes.insert(std::make_pair(pState, pTileNode));
    m_tileNodes.insert(std::make_pair(rState, rTileNode));

    m_graph->new_acc_edge(pState, qState, observableAsBdd(qTripleTileNode.observable()));
    m_graph->new_acc_edge(qState, rState, observableAsBdd(rTileNode.observable()));
}

unsigned OmnidirectionalPolyhedralAbstraction::getStateByTileOrCreate(const Tile& tile)
{
    if (m_stateByTile.count(tile))
    {
        return m_stateByTile[tile];
    }

    unsigned s { m_graph->new_state() };
    m_stateByTile[tile] = s;
    return s;
}

bdd OmnidirectionalPolyhedralAbstraction::observableAsBdd(const Observable& observable)
{
    const spot::atomic_prop_set& observableAtoms { observable.atoms() };
    return spot::formula_to_bdd(SpotUtils::andAtoms(observableAtoms), m_graph->get_dict(), m_graph);
}

PowersetConstSharedPtr OmnidirectionalPolyhedralAbstraction::points(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    const std::variant<TileNode, TripleTileNode>& variant { m_tileNodes.at(state) };

    if (std::holds_alternative<TileNode>(variant))
        return std::get<TileNode>(variant).points();

    return std::get<TripleTileNode>(variant).points();
}

const Observable& OmnidirectionalPolyhedralAbstraction::observable(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    const std::variant<TileNode, TripleTileNode>& variant { m_tileNodes.at(state) };

    if (std::holds_alternative<TileNode>(variant))
        return std::get<TileNode>(variant).observable();

    return std::get<TripleTileNode>(variant).observable();
}

int OmnidirectionalPolyhedralAbstraction::totalStates() const
{
    return static_cast<int>(m_graph->num_states());
}

int OmnidirectionalPolyhedralAbstraction::totalEdges() const
{
    return static_cast<int>(m_graph->num_edges());
}

OmnidirectionalPolyhedralAbstraction::EdgeIterator OmnidirectionalPolyhedralAbstraction::successors(const unsigned state)
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    return m_graph->out(state);
}

bool OmnidirectionalPolyhedralAbstraction::hasSuccessors(const unsigned state)
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    auto edgeStorages { m_graph->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

int OmnidirectionalPolyhedralAbstraction::countSuccessors(const unsigned state)
{
    const EdgeIterator& edgeIterator { m_graph->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

spot::const_twa_graph_ptr OmnidirectionalPolyhedralAbstraction::twa() const
{
    return m_graph;
}

int OmnidirectionalPolyhedralAbstraction::initialState() const
{
    return static_cast<int>(m_graph->get_init_state_number());
}

void OmnidirectionalPolyhedralAbstraction::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_graph);
}