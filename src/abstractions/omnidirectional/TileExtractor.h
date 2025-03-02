#pragma once

#include <unordered_set>

#include "Tile.h"

class TileExtractor
{
public:
    TileExtractor();

    std::vector<Tile> extractTiles(const std::vector<Observable>& observables);
    std::vector<Tile> extractTiles(const Observable& observable);
private:
    const Observable* m_currentObservable;
    std::unordered_set<int> m_patchesIndexesInFirstTile {};

    Tile findFirstTile();
    std::list<std::reference_wrapper<const Poly>> collectPatchesNotInFirstTile();
    std::vector<Tile> findRemainingTiles();

    static PowersetUniquePtr border(const Poly& p, const Poly& q);
    static bool areAdjacent(const Poly& p, const Poly& q);
};