#pragma once

#include "formula.h"

#include "Tile.h"

class TripleTileNode
{
public:
    TripleTileNode(const Tile& tile1, const Tile& tile2, const Tile& tile3)
        : m_triple { std::make_tuple(tile1, tile2, tile3) }
    {
        assert(tile1.spaceDimension() == tile2.spaceDimension());
        assert(tile2.spaceDimension() == tile3.spaceDimension());

        computeObservable();
        computePoints();
    }

    [[nodiscard]] const Observable& observable() const { return m_observable; }
    [[nodiscard]] PowersetConstSharedPtr points() const { return m_points; }
    [[nodiscard]] bool isUniverse() const { return m_points->is_universe(); }
    [[nodiscard]] bool isEmpty() const { return m_points->is_empty(); }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_observable.spaceDimension(); }

    friend bool operator!= (const TripleTileNode& tripleTileNode1, const TripleTileNode& tripleTileNode2)
    {
        return tripleTileNode1.m_triple != tripleTileNode2.m_triple;
    }

    friend bool operator== (const TripleTileNode& tripleTileNode1, const TripleTileNode& tripleTileNode2)
    {
        return tripleTileNode1.m_triple == tripleTileNode2.m_triple;
    }

private:
    std::tuple<Tile, Tile, Tile> m_triple;
    Observable m_observable {};
    PowersetConstSharedPtr m_points {};

    void computeObservable()
    {
        const Tile& tile2 { std::get<1>(m_triple) };
        const Observable& tile2Observable { tile2.observable() };
        spot::atomic_prop_set tile2AtomsWithSing { tile2Observable.atoms() };
        tile2AtomsWithSing.insert(sing());
        m_observable = Observable { tile2AtomsWithSing, *tile2.patches() };
    }

    void computePoints()
    {
        const Tile& tile1 { std::get<0>(m_triple) };
        const Tile& tile2 { std::get<1>(m_triple) };
        const Tile& tile3 { std::get<2>(m_triple) };
        m_points = PPLUtils::intersect({ tile1.closure(), tile2.patches(), tile3.closure() });
    }
};