#pragma once

#include <unordered_set>

#include "Tile.h"
#include "TileExtractor.h"

class TileExtractorComplex: public TileExtractor
{
public:
    TileExtractorComplex();

    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;
private:
    const Observable* m_currentObservable;
    std::unordered_set<int> m_patchesIndexesInFirstTile {};

    Tile findFirstTile();
    std::list<std::reference_wrapper<const Poly>> collectPatchesNotInFirstTile();
    std::vector<Tile> findRemainingTiles();
};
