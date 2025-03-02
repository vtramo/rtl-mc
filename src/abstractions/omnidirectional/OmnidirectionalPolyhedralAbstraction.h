#pragma once

#include <spot/tl/apcollect.hh>
#include <variant>
#include "TileNode.h"
#include "TripleTileNode.h"
#include "PolyhedralSystem.h"

class OmnidirectionalPolyhedralAbstraction;
using OmnidirectionalPolyhedralAbstractionConstSharedPtr = std::shared_ptr<OmnidirectionalPolyhedralAbstraction>;

class OmnidirectionalPolyhedralAbstraction
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    explicit OmnidirectionalPolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem);

    [[nodiscard]] const Observable& observable(unsigned state) const;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const;
    [[nodiscard]] int initialState() const;
    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] EdgeIterator successors(unsigned state);
    [[nodiscard]] bool hasSuccessors(unsigned state);
    [[nodiscard]] int countSuccessors(unsigned state);

    void printDotFormat(std::ostream& os) const;

private:
    spot::twa_graph_ptr m_graph {};
    std::unordered_map<unsigned, std::variant<TileNode, TripleTileNode>> m_tileNodes {};
    std::unordered_map<Tile, unsigned> m_stateByTile {};

    void buildAbstraction(std::vector<Tile>&& tiles);
    void initializeGraph(spot::bdd_dict_ptr bddDict);
    unsigned getStateByTileOrCreate(const Tile& tile);
    bdd observableAsBdd(const Observable& observable);
    void processTriple(const Tile& tile1, const Tile& tile2, const Tile& tile3);
    spot::const_twa_graph_ptr twa() const;

    friend class FiniteSynchronousProduct;
};