/**
 * \file TileExtractorGraph.h
 * \brief Graph-based implementation of \c Tile extraction from \c Observable s
 */
#pragma once

#include "TileExtractor.h"

/**
 * \class TileExtractorGraph
 * \brief An implementation of the \c TileExtractor class that internally uses a graph.
 *
 * 1. Creates an undirected graph where:
 *    - Nodes represent individual patches (all \c Poly objects contained in the interpretation of the \c Observable)
 *    - There is an edge between two nodes if and only if the corresponding \c Poly s are adjacent
 * 2. Identifies connected components in the graph
 * 3. Each connected component forms a \c Tile
 *
 * \see TileExtractor
 * \see Tile
 * \see Observable
 * \see adjacency.h
 */
class TileExtractorGraph: public TileExtractor
{
public:
    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;
};