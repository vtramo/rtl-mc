#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
#include <vector>

class DenotPathNode;

class DenotOnTheFly: public Denot
{
public:
    DenotOnTheFly(
        const PolyhedralSystemConstSharedPtr polyhedralSystem,
        BackwardNFAConstSharedPtr backwardNfa,
        const bool collectPaths = false,
        const int maxIterations = 100000
    )
      : Denot(polyhedralSystem, backwardNfa)
      , m_maxIterations { maxIterations }
      , m_collectPaths { collectPaths }
      {
        const auto& stats { m_backwardNfa->stats() };
        m_maxRecursionDepth = stats.getNfaMaxRecursiveDepth();
      }

    ~DenotOnTheFly() override = default;

    PowersetUniquePtr run() override;

    [[nodiscard]] int totalIterations() const override { return m_iterations; }
    [[nodiscard]] const std::vector<std::vector<DenotPathNode>>& paths() const override { return m_paths; }
    [[nodiscard]] int totalReachCalls() const override { return m_totalReachCalls; }
    [[nodiscard]] int maxIterations() const override { return m_maxIterations; }
    [[nodiscard]] bool isIncompleteResult() const override { return m_maxIterationReached; }
    [[nodiscard]] PowersetConstSharedPtr result() const override { return m_result; }

private:
    int m_iterations { };
    int m_maxRecursionDepth {};
    int m_maxIterations {};
    bool m_collectPaths { false };
    bool m_maxIterationReached { false };
    int m_totalReachCalls { };
    std::vector<std::vector<DenotPathNode>> m_paths {};
    std::vector<DenotPathNode> m_currentPath {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::vector<Powerset> V,
        int recursionDepth,
        bool isSing
    );

    void addDisjunct(std::vector<Powerset>& V, int state, const Poly& P) const;
    void pushPathNode(const Poly& P, const Poly& X, const StateDenotation& stateDenotation, int state);
    DenotPathNode popPathNode();
    void addCurrentPath();

    static const Powerset& getVisitedPowerset(std::vector<Powerset>& V, int state);
};