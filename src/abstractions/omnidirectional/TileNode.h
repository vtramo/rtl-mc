/**
 * \file TileNode.h
 * \brief Representation of a single \c Tile node in a \c OmnidirectionalPolyhedralAbstraction
 */

#pragma once

#include "Tile.h"

/**
 * \class TileNode
 * \brief Represents a node in an \c OmnidirectionalPolyhedralAbstraction consisting of a single \c Tile
 *
 * This class represents a node \f$P\f$ in an \c OmnidirectionalPolyhedralAbstraction that corresponds to exactly one \c Tile.
 *
* The node includes:
 * - The \c Observable containing the \c Tile (\ref TileNode::observable())
 * - The set of patches that comprise the \c Tile (\ref TileNode::points())
 *
 * \see Observable
 * \see Tile
 * \see OmnidirectionalPolyhedralAbstraction
 */
class TileNode
{
public:
    /**
     * \brief Constructs a node from a \c Tile
     * \param tile The \c Tile representing this node
     */
    explicit TileNode(const Tile& tile)
        : m_tile { tile }
    {}

    /**
     * \brief The \c Observable containing the \c Tile
     * \return Const reference to the \c Observable containing the \c Tile
     */
    [[nodiscard]] const Observable& observable() const { return m_tile.observable(); }

    /**
    * \brief Gets the set of patches that make up the underlying \c Tile
     * \return Shared pointer to the set of patches that make up the underlying \c Tile  (as a \c PowersetConstSharedPtr)
     */
    [[nodiscard]] PowersetConstSharedPtr points() const { return m_tile.patches(); }

    /**
     * \brief Checks if the set of points associated to this node covers the entire state space
     * \return `true` if the set of points associated to this node is the universe
     */
    [[nodiscard]] bool isUniverse() const { return points()->is_universe(); }

    /**
     * \brief Checks if the set of points associated to this node is empty
     * \return `true` if the underlying \c Tile form an empty set
     */
    [[nodiscard]] bool isEmpty() const { return points()->is_empty(); }

    /**
     * \brief Gets the space dimension of the state space in which this node is located
     * \return The dimension of the \c Tile
     */
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
