/**
* \file TileExtractor.h
 * \brief Abstract base class for extracting \c Tile s from \c Observable s
 */

#pragma once

#include <vector>

#include "Tile.h"
#include "Observable.h"

/**
 * \class TileExtractor
 * \brief Abstract class to extract \c Tile s from \c Observable s.
 *
 * This class defines the interface for extracting \c Tile objects from:
 * - A single \c Observable
 * - A vector of \c Observable s
 *
 * A \c Tile represents a maximal connected set of \c Poly (a.k.a. patches) in an \c Observable.
 *
 * \note This is an abstract base class - concrete implementations must provide
 *       at least the single-observable extraction logic.
 *
 * \see Tile
 * \see Observable
 * \see OmnidirectionalPolyhedralAbstraction
 * \see adjacency.h
 */
class TileExtractor
{
public:
    virtual ~TileExtractor() = default;

    /**
     * \brief Extracts all \c Tile s from a vector of \c Observable s
     * \param observables Set of \c Observable s to process
     * \return Combined vector of all \c Tile s extracted from the input \c Observable s
     */
    [[nodiscard]] virtual std::vector<Tile> extractTiles(const std::vector<Observable>& observables);

    /**
     * \brief Extracts all \c Tile s from a single \c Observable
     * \param observable The \c Observable to process
     * \return All \c Tile s contained in the \c Observable
     */
    [[nodiscard]] virtual std::vector<Tile> extractTiles(const Observable& observable) = 0;
};

