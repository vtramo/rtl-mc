#pragma once

#include <vector>

#include "Tile.h"
#include "Observable.h"

class TileExtractor
{
public:
    virtual ~TileExtractor() = default;

    [[nodiscard]] virtual std::vector<Tile> extractTiles(const std::vector<Observable>& observables);
    [[nodiscard]] virtual std::vector<Tile> extractTiles(const Observable& observable) = 0;
};

