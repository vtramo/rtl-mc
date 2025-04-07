#include "OmnidirectionalPolyhedralAbstraction.h"
#include "TileExtractorGraph.h"

OmnidirectionalPolyhedralAbstraction::OmnidirectionalPolyhedralAbstraction(
    PolyhedralSystemSharedPtr polyhedralSystem,
    const std::string_view name
)
    : PolyhedralAbstraction(polyhedralSystem, name)
{
    if (!polyhedralSystem->hasOmnidirectionalFlow())
        throw std::invalid_argument("To construct an OmnidirectionalPolyhedralAbstraction, the flow must be omnidirectional!");

    std::vector tiles { extractTilesFromPolyhedralSystem(polyhedralSystem) };
    buildAbstraction(std::move(tiles));
}

std::vector<Tile> OmnidirectionalPolyhedralAbstraction::extractTilesFromPolyhedralSystem(PolyhedralSystemSharedPtr polyhedralSystem)
{
    Log::log(Verbosity::veryVerbose, "[{}] Generating polyhedral system observables...", m_name);
    const std::vector<Observable>& observables { polyhedralSystem->getOrGenerateObservables() };
    Log::log(Verbosity::verbose, "[{}] Polyhedral system total observables: {}.", m_name, observables.size());

    Log::log(Verbosity::veryVerbose, "[{}] Extracting tiles...", m_name);
    TileExtractorGraph tileExtractor {};
    std::vector<Tile> tiles { tileExtractor.extractTiles(observables) };
    Log::log(Verbosity::verbose, "[{}] Total tiles: {}.", m_name, tiles.size());

    return tiles;
}

/**
 * This method constructs the abstraction by:
 * 1. Processing all possible triple combinations of \c Tile s
 * 2. Creating non-empty nodes and valid edges between nodes (\ref processTriple)
 * 3. Logging construction statistics
 */
void OmnidirectionalPolyhedralAbstraction::buildAbstraction(std::vector<Tile>&& tiles)
{
    const int totalTiles {static_cast<int>(tiles.size())};
    std::unordered_map<Tile, unsigned> stateByTile {};
    stateByTile.reserve(totalTiles * 3 / 2);

    for (int i { 0 }; i < totalTiles; ++i)
    {
        const auto& _ = getStateByTileOrCreate(tiles[i], stateByTile);

        for (int j { 0 }; j < totalTiles; ++j)
        {
            for (int k { 0 }; k < totalTiles; ++k)
            {
                processTriple(tiles[i], tiles[j], tiles[k], stateByTile);
            }
        }
    }

    Log::log(m_automaton, fmt::format("{}-construction-completed", m_name));
    Log::log(Verbosity::verbose, "[{}] Construction completed.", m_name);
    Log::log(Verbosity::verbose, "[{}] Total states: {}.", m_name, totalStates());
    Log::log(Verbosity::verbose, "[{}] Total edges: {}.", m_name, totalEdges());
    Log::log(Verbosity::verbose, "[{}] Total initial states: {}.", m_name, totalInitialStates());
    Log::log(Verbosity::verbose, "[{}] Total accepting states: {}.", m_name, totalAcceptingStates());
    Log::log(Verbosity::verbose, "[{}] Initial state: {}.", m_name, initialState());
}

/**
 * For a triple \f$(P,Q,R)\f$, this method:
 * 1. Creates a new \c TripleTileNode \f$(P,Q,R)\f$ if \f$Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\f$ is non-empty
 * 2. Creates corresponding \c TileNode s for \f$P\f$ and \f$R\f$ if they don't exist
 * 3. Adds edges:
 *    - \f$(P) \rightarrow (P,Q,R)\f$ labeled with \f$\text{observableAsBdd}(\mathit{obs}(Q) \cup \{\mathit{sing}\})\f$
 *    - \f$(P,Q,R) \rightarrow (R)\f$ labeled with \f$\text{observableAsBdd}(\mathit{obs}(R))\f$
 *
 * \see observableAsBdd
 */
void OmnidirectionalPolyhedralAbstraction::processTriple(
    const Tile& tile1,
    const Tile& tile2,
    const Tile& tile3,
    std::unordered_map<Tile, unsigned>& stateByTile
)
{
    TripleTileNode qTripleTileNode { tile1, tile2, tile3 };
    if (qTripleTileNode.isEmpty()) return;

    unsigned qState { m_automaton->new_state() };
    m_tileNodes.insert(std::make_pair(qState, qTripleTileNode));

    unsigned pState { getStateByTileOrCreate(tile1, stateByTile) };
    unsigned rState { getStateByTileOrCreate(tile3, stateByTile) };
    TileNode pTileNode { tile1 };
    TileNode rTileNode { tile3 };
    m_tileNodes.insert(std::make_pair(pState, pTileNode));
    m_tileNodes.insert(std::make_pair(rState, rTileNode));

    m_automaton->new_acc_edge(pState, qState, observableAsBdd(qTripleTileNode.observable()));
    m_automaton->new_acc_edge(qState, rState, observableAsBdd(rTileNode.observable()));
}

unsigned OmnidirectionalPolyhedralAbstraction::getStateByTileOrCreate(
    const Tile& tile,
    std::unordered_map<Tile, unsigned>& stateByTile
)
{
    if (stateByTile.count(tile))
        return stateByTile[tile];

    unsigned s { m_automaton->new_state() };
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