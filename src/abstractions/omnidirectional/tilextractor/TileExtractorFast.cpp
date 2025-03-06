#include "TileExtractorFast.h"

std::vector<Tile> TileExtractorFast::extractTiles(const std::vector<Observable>& observables)
{
    return TileExtractor::extractTiles(observables);
}

std::vector<Tile> TileExtractorFast::extractTiles(const Observable& observable)
{
    if (observable.isEmpty()) return {};

    buildPowersetLinkedList(observable);
    assert(m_powersetLinkedList.head != nullptr);

    std::vector<Tile> tiles {};
    PowersetLinkedList::NodePtr i { m_powersetLinkedList.head };
    while (i != nullptr)
    {
        PowersetLinkedList::NodePtr j { i->next };
        PowersetLinkedList::NodePtr next { j };

        while (j != nullptr)
        {
            if (merge(i, j))
            {
                if (i == m_powersetLinkedList.head)
                {
                    m_powersetLinkedList.head = next;
                }
                else
                {
                    i->prev->next = i->next;
                    i->next->prev = i->prev;
                }

                if (j == next)
                {
                    next = j->next;
                }

                i->prev = nullptr;
                i->next = nullptr;
                i = j;
            }

            j = j->next;
        }

        i = next;
    }

    PowersetLinkedList::NodePtr cursor { m_powersetLinkedList.head };
    while (cursor != nullptr)
    {
        tiles.emplace_back(observable, cursor->powerset);
        cursor->prev = nullptr;
        cursor = cursor->next;
    }

    return tiles;
}

void TileExtractorFast::buildPowersetLinkedList(const Observable& observable)
{
    if (observable.isEmpty()) return;

    Powerset::const_iterator patchIterator { observable.begin() };
    m_powersetLinkedList.head = std::make_shared<PowersetLinkedList::Node>();
    m_powersetLinkedList.head->powerset = std::make_shared<Powerset>(patchIterator->pointset());
    PowersetLinkedList::NodePtr prev {};
    PowersetLinkedList::NodePtr curr { m_powersetLinkedList.head };
    ++patchIterator;
    for (; patchIterator != observable.end(); ++patchIterator)
    {
        curr->next = std::make_shared<PowersetLinkedList::Node>();
        curr->prev = prev;
        prev = curr;
        curr = curr->next;
        curr->powerset = std::make_shared<Powerset>(patchIterator->pointset());
    }
}

bool TileExtractorFast::merge(const PowersetLinkedList::NodePtr& i, const PowersetLinkedList::NodePtr& j)
{
    assert(i != nullptr && j != nullptr);

    if (PPLUtils::areAdjacent(*i->powerset, *j->powerset))
    {
        PPLUtils::fusion(*j->powerset, *i->powerset);
        return true;
    }

    return false;
}
