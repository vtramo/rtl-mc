#pragma once

#include "Tile.h"
#include "Observable.h"
#include "TileExtractor.h"

class TileExtractorDoublyLinkedList: public TileExtractor
{
public:
    [[nodiscard]] std::vector<Tile> extractTiles(const std::vector<Observable>& observables) override;
    [[nodiscard]] std::vector<Tile> extractTiles(const Observable& observable) override;
private:
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

    void buildPowersetLinkedList(const Observable& observable);
    static bool merge(const PowersetLinkedList::NodePtr& i, const PowersetLinkedList::NodePtr& j);
};



