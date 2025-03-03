#pragma once

#include "Tile.h"

class TileNode
{
public:
    explicit TileNode(const Tile& tile)
        : m_tile { tile }
    {}

    [[nodiscard]] const Observable& observable() const { return m_tile.observable(); }
    [[nodiscard]] PowersetConstSharedPtr points() const { return m_tile.patches(); }
    [[nodiscard]] bool isUniverse() const { return points()->is_universe(); }
    [[nodiscard]] bool isEmpty() const { return points()->is_empty(); }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_tile.spaceDimension(); }

    friend bool operator!= (const TileNode& tileNode1, const TileNode& tileNode2)
    {
        return tileNode1.m_tile != tileNode2.m_tile;
    }

    friend bool operator== (const TileNode& tileNode1, const TileNode& tileNode2)
    {
        return tileNode1.m_tile == tileNode2.m_tile;
    }

private:
    Tile m_tile {};
};
