#include "OmnidirectionalPolyhedralAbstraction.h"

#include "TileExtractorFast.h"

OmnidirectionalPolyhedralAbstraction::OmnidirectionalPolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem)
    : PolyhedralAbstraction { polyhedralSystem }
{
    if (!polyhedralSystem->isOmnidirectionalFlow())
        throw std::invalid_argument("To construct an OmnidirectionalPolyhedralAbstraction, the flow must be omnidirectional!");

    PolyhedralAbstraction::initializeGraph();

    std::vector tiles { extractTilesFromPolyhedralSystem(polyhedralSystem) };
    buildAbstraction(std::move(tiles));
}

std::vector<Tile> OmnidirectionalPolyhedralAbstraction::extractTilesFromPolyhedralSystem(PolyhedralSystemConstSharedPtr polyhedralSystem)
{
    std::vector observables { polyhedralSystem->generateObservables() };
    TileExtractorFast tileExtractor {};
    return tileExtractor.extractTiles(observables);
}

void OmnidirectionalPolyhedralAbstraction::buildAbstraction(std::vector<Tile>&& tiles)
{
    const int totalTiles {static_cast<int>(tiles.size())};
    std::unordered_map<Tile, unsigned> stateByTile {};
    stateByTile.reserve(totalTiles * 3 / 2);

    for (int i { 0 }; i < totalTiles; ++i)
        for (int j { 0 }; j < totalTiles; ++j)
            for (int k { 0 }; k < totalTiles; ++k)
                processTriple(tiles[i], tiles[j], tiles[k], stateByTile);
}

void OmnidirectionalPolyhedralAbstraction::processTriple(
    const Tile& tile1,
    const Tile& tile2,
    const Tile& tile3,
    std::unordered_map<Tile, unsigned>& stateByTile
)
{
    TripleTileNode qTripleTileNode { tile1, tile2, tile3 };
    if (qTripleTileNode.isEmpty()) return;

    unsigned qState { m_graph->new_state() };
    m_tileNodes.insert(std::make_pair(qState, qTripleTileNode));

    unsigned pState { getStateByTileOrCreate(tile1, stateByTile) };
    unsigned rState { getStateByTileOrCreate(tile3, stateByTile) };
    TileNode pTileNode { tile1 };
    TileNode rTileNode { tile3 };
    m_tileNodes.insert(std::make_pair(pState, pTileNode));
    m_tileNodes.insert(std::make_pair(rState, rTileNode));

    m_graph->new_acc_edge(pState, qState, observableAsBdd(qTripleTileNode.observable()));
    m_graph->new_acc_edge(qState, rState, observableAsBdd(rTileNode.observable()));
}

unsigned OmnidirectionalPolyhedralAbstraction::getStateByTileOrCreate(
    const Tile& tile,
    std::unordered_map<Tile, unsigned>& stateByTile
)
{
    if (stateByTile.count(tile))
        return stateByTile[tile];

    unsigned s { m_graph->new_state() };
    stateByTile[tile] = s;
    return s;
}

PowersetConstSharedPtr OmnidirectionalPolyhedralAbstraction::points(const unsigned state) const
{
    PolyhedralAbstraction::assertThatStateIsInRange(state);

    const std::variant<TileNode, TripleTileNode>& variant { m_tileNodes.at(state) };

    if (std::holds_alternative<TileNode>(variant))
        return std::get<TileNode>(variant).points();

    return std::get<TripleTileNode>(variant).points();
}

const Observable& OmnidirectionalPolyhedralAbstraction::observable(const unsigned state) const
{
    PolyhedralAbstraction::assertThatStateIsInRange(state);

    const std::variant<TileNode, TripleTileNode>& variant { m_tileNodes.at(state) };

    if (std::holds_alternative<TileNode>(variant))
        return std::get<TileNode>(variant).observable();

    return std::get<TripleTileNode>(variant).observable();
}