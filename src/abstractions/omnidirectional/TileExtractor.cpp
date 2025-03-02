#include "TileExtractor.h"

#include <stack>
#include <unordered_set>

TileExtractor::TileExtractor(): m_currentObservable {}
{
}

std::vector<Tile> TileExtractor::extractTiles(const std::vector<Observable>& observables)
{
    std::vector<Tile> tiles {};

    for (const Observable& observable: observables)
    {
        std::vector observableTiles { extractTiles(observable) };

        if (!observableTiles.empty())
        {
            tiles.resize(tiles.size() + observableTiles.size());
            std::move_backward(observableTiles.begin(), observableTiles.end(), tiles.end());
        }
    }

    return tiles;
}

std::vector<Tile> TileExtractor::extractTiles(const Observable& observable)
{
    m_currentObservable = &observable;
    if (m_currentObservable->isEmpty()) return {};

    PowersetConstSharedPtr observableInterpretation { m_currentObservable->interpretation() };
    const Poly& firstPatch { observableInterpretation->begin()->pointset() };

    if (m_currentObservable->totalPatches() == 1)
    {
        PowersetSharedPtr powerset { std::make_shared<Powerset>(firstPatch) };
        Tile tile { *m_currentObservable, powerset };
        return { std::move(tile) };
    }

    std::vector<Tile> observableTiles {};
    Tile firstTile { findFirstTile() };
    observableTiles.push_back(firstTile);

    std::vector remainingTails { findRemainingTiles() };
    observableTiles.resize(observableTiles.size() + remainingTails.size());
    std::move_backward(remainingTails.begin(), remainingTails.end(), observableTiles.end());

    m_patchesIndexesInFirstTile.clear();
    return observableTiles;
}

Tile TileExtractor::findFirstTile()
{
    PowersetConstSharedPtr observableInterpretation { m_currentObservable->interpretation() };
    const Poly& firstPatch { observableInterpretation->begin()->pointset() };

    PowersetSharedPtr tilePatches { std::make_shared<Powerset>(firstPatch) };

    m_patchesIndexesInFirstTile.insert(1);

    std::stack<std::reference_wrapper<const Poly>> todoPatches {};
    todoPatches.push(firstPatch);

    do
    {
        const Poly& currentPatch { todoPatches.top() };
        todoPatches.pop();

        int patchIndex {};
        for (const auto& otherPatchWrapper: *m_currentObservable->interpretation())
        {
            ++patchIndex;
            const Poly& otherPatch { otherPatchWrapper.pointset() };
            if (otherPatch == currentPatch) continue;
            if (m_patchesIndexesInFirstTile.count(patchIndex)) continue;

            if (areAdjacent(otherPatch, currentPatch))
            {
                tilePatches->add_disjunct(otherPatch);
                todoPatches.push(otherPatch);
                m_patchesIndexesInFirstTile.insert(patchIndex);
            }
        }
    } while (!todoPatches.empty());

    return Tile { *m_currentObservable, tilePatches };
}

std::list<std::reference_wrapper<const Poly>> TileExtractor::collectPatchesNotInFirstTile()
{
    int patchIndex {};
    std::list<std::reference_wrapper<const Poly>> patchesNotInFirstTile {};

    for (const auto& patchWrapper: *m_currentObservable->interpretation())
    {
        ++patchIndex;
        const Poly& patch { patchWrapper.pointset() };
        if (!m_patchesIndexesInFirstTile.count(patchIndex))
            patchesNotInFirstTile.push_back(patch);
    }

    return patchesNotInFirstTile;
}

std::vector<Tile> TileExtractor::findRemainingTiles()
{
    std::vector<Tile> tiles {};
    std::list globalTodoPatches { collectPatchesNotInFirstTile() };
    std::stack<std::reference_wrapper<const Poly>> todoPatches {};

    while (!globalTodoPatches.empty())
    {
        const Poly& currentGlobalPatch { globalTodoPatches.back() };
        globalTodoPatches.pop_back();
        todoPatches.push(currentGlobalPatch);
        PowersetSharedPtr tilePatches { std::make_shared<Powerset>(currentGlobalPatch) };

        do
        {
            const Poly& currentTodoPatch { todoPatches.top() };
            todoPatches.pop();
            for (auto otherPatchIterator { globalTodoPatches.begin() }; otherPatchIterator != globalTodoPatches.end();)
            {
                if (areAdjacent(*otherPatchIterator, currentTodoPatch))
                {
                    tilePatches->add_disjunct(*otherPatchIterator);
                    todoPatches.push(*otherPatchIterator);
                    otherPatchIterator = globalTodoPatches.erase(otherPatchIterator);
                }
                ++otherPatchIterator;
            }
        } while (!todoPatches.empty());

        tiles.emplace_back(*m_currentObservable, std::move(tilePatches));
    }

    return tiles;
}

PowersetUniquePtr TileExtractor::border(const Poly& p, const Poly& q)
{
    Poly closureP { p };
    closureP.topological_closure_assign();

    Poly closureQ { q };
    closureQ.topological_closure_assign();

    PolyUniquePtr pIntersectClosureQ { PPLUtils::intersect(p, closureQ) };
    PolyUniquePtr qIntersectClosureP { PPLUtils::intersect(q, std::move(closureP)) };
    return PPLUtils::fusion(*pIntersectClosureQ, *qIntersectClosureP);
}

bool TileExtractor::areAdjacent(const Poly& p, const Poly& q)
{
    PowersetUniquePtr borderPQ { border(p, q) };
    return !borderPQ->is_empty();
}
