#pragma once

#include <variant>
#include "TileNode.h"
#include "TripleTileNode.h"
#include "PolyhedralSystem.h"
#include "PolyhedralAbstraction.h"

class OmnidirectionalPolyhedralAbstraction;
using OmnidirectionalPolyhedralAbstractionConstSharedPtr = std::shared_ptr<const OmnidirectionalPolyhedralAbstraction>;
using OmnidirectionalPolyhedralAbstractionSharedPtr = std::shared_ptr<OmnidirectionalPolyhedralAbstraction>;
using OmnidirectionalPolyhedralAbstractionUniquePtr = std::unique_ptr<OmnidirectionalPolyhedralAbstraction>;

class OmnidirectionalPolyhedralAbstraction: public PolyhedralAbstraction
{
public:
    explicit OmnidirectionalPolyhedralAbstraction(
        PolyhedralSystemConstSharedPtr polyhedralSystem,
        std::string_view name = "OmnidirectionalPolyhedralAbstraction"
    );

    [[nodiscard]] const Observable& observable(unsigned state) const override;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const override;

private:
    std::unordered_map<unsigned, std::variant<TileNode, TripleTileNode>> m_tileNodes {};

    void buildAbstraction(std::vector<Tile>&& tiles);
    unsigned getStateByTileOrCreate(const Tile& tile, std::unordered_map<Tile, unsigned>& stateByTile);
    void processTriple(const Tile& tile1, const Tile& tile2, const Tile& tile3, std::unordered_map<Tile, unsigned>& stateByTile);

    static std::vector<Tile> extractTilesFromPolyhedralSystem(PolyhedralSystemConstSharedPtr polyhedralSystem);
};