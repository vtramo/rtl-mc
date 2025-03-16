#include "TileExtractorList.h"
#include "adjacency.h"

std::vector<Tile> TileExtractorList::extractTiles(const std::vector<Observable>& observables)
{
    return TileExtractor::extractTiles(observables);
}

std::vector<Tile> TileExtractorList::extractTiles(const Observable& observable)
{
    if (observable.isEmpty()) return {};

    PowersetConstSharedPtr observableInterpretation { observable.interpretation() };
    const Poly& firstPatch { observableInterpretation->begin()->pointset() };

    if (observable.totalPatches() == 1)
    {
        PowersetSharedPtr powerset { std::make_shared<Powerset>(firstPatch) };
        Tile tile { observable, powerset };
        return { std::move(tile) };
    }

    std::vector<Tile> tiles {};
    initializePartialTileList(observable);

    auto head { m_partialTileList.begin() };
    auto cursor { ++m_partialTileList.begin() };

    do
    {
        bool advanceHead { true };
        PowersetSharedPtr headPartialTile { *head };

        do
        {
            PowersetSharedPtr cursorPartialTile { *cursor };
            if (areAdjacent(*headPartialTile, *cursorPartialTile))
            {
                cursor = m_partialTileList.erase(cursor);
                PPLUtils::fusion(*headPartialTile, *cursorPartialTile);
                advanceHead = false;
            }
            else
            {
                ++cursor;
            }
        } while (cursor != m_partialTileList.end());

        if (advanceHead)
        {
            tiles.emplace_back(observable, *head);
            ++head;
        }

        cursor = head;
        ++cursor;
        if (cursor == m_partialTileList.end())
        {
            tiles.emplace_back(observable, *head);
        }
    } while (cursor != m_partialTileList.end());

    m_partialTileList.clear();
    return tiles;
}

void TileExtractorList::initializePartialTileList(const Observable& observable)
{
    auto head { m_partialTileList.begin() };
    for (const auto& patchWrapper: observable)
    {
        const Poly& patch { patchWrapper.pointset() };
        m_partialTileList.insert(head++, std::make_shared<Powerset>(patch));
    }
}
