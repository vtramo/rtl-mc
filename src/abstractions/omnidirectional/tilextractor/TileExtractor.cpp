#include "TileExtractor.h"

std::vector<Tile> TileExtractor::extractTiles(const std::vector<Observable>& observables)
{
    std::vector<Tile> tiles {};

    for (const Observable& observable: observables)
    {
        std::vector observableTiles { extractTiles(observable) };

        if (!observableTiles.empty())
        {
            tiles.resize(tiles.size() + observableTiles.size());
            std::move_backward(observableTiles.begin(), observableTiles.end(), tiles.end());
        }
    }

    return tiles;
}
