/**
* \file TileExtractorDoublyLinkedList.h
 * \brief Doubly linked list based implementation of \c Tile extraction from \c Observable s
 */

#pragma once

#include "Tile.h"
#include "Observable.h"
#include "TileExtractor.h"

/**
 * \class TileExtractorDoublyLinkedList
 * \brief An implementation of the \c TileExtractor class that internally uses a doubly linked list.
 * \see TileExtractor
 * \see Tile
 * \see Observable
 * \see adjacency.h
 */
class TileExtractorDoublyLinkedList: public TileExtractor
{
public:
    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;
protected:
    struct PowersetLinkedList
    {
        struct Node;
        using NodePtr = std::shared_ptr<Node>;
        struct Node
        {
            PowersetSharedPtr powerset {};
            NodePtr next { nullptr };
            NodePtr prev { nullptr };
        };

        NodePtr head { nullptr };
    };

    PowersetLinkedList m_powersetLinkedList {};

    /**
     * \brief Constructs the initial doubly linked list from the \c Observable 's patches.
     *        Each node is a \c Powerset of size 1.
     * \param observable The input observable containing the patches
     */
    void buildPowersetLinkedList(const Observable& observable);

    /**
     * \brief Merges the patches from node `i` into node `j` iff they are geometrically adjacent
     * \param i First node
     * \param j Second node
     * \return `true` if patches were adjacent and merge succeeded, `false` otherwise
     * \see adjacency.h
     */
    static bool merge(const PowersetLinkedList::NodePtr& i, const PowersetLinkedList::NodePtr& j);
};



