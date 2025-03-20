#pragma once

#include "TileExtractor.h"

class TileExtractorGraph: public TileExtractor
{
public:
    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;

private:

};