#pragma once

#include "Tile.h"
#include "Observable.h"
#include "TileExtractor.h"

class TileExtractorList: public TileExtractor
{
public:
    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;
private:
    std::list<PowersetSharedPtr> m_partialTileList {};

    void initializePartialTileList(const Observable& observable);
};



