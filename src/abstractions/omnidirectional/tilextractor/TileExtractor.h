#pragma once

#include <unordered_set>

#include "Tile.h"

class TileExtractor
{
public:
    TileExtractor();

    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables);
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable);
private:
    const Observable* m_currentObservable;
    std::unordered_set<int> m_patchesIdInFirstTile {};

    Tile findFirstTile();
    std::list<std::reference_wrapper<const Poly>> collectPatchesNotInFirstTile();
    std::vector<Tile> findRemainingTiles();
};