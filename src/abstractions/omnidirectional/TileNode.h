#pragma once

#include "Tile.h"

class TileNode
{
public:
    explicit TileNode(const Tile& tile)
        : m_tile { tile }
    {}

    const Observable& observable() const { return m_tile.observable(); }
    PowersetConstSharedPtr points() const { return m_tile.patches(); }
    bool isUniverse() const { return points()->is_universe(); }
    bool isEmpty() const { return points()->is_empty(); }

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
