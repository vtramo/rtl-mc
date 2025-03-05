#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class DenotOnTheFly: public Denot
{
public:
    DenotOnTheFly(const PolyhedralSystemConstSharedPtr polyhedralSystem, BackwardNFAConstSharedPtr backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {
        const auto& stats { m_backwardNfa->stats() };
        m_maxRecursionDepth = 1 + stats.nfaConstructionStats.totalNumberPatches * 2;
    }
    ~DenotOnTheFly() override = default;

    PowersetUniquePtr run() override;

    [[nodiscard]] int totalIterations() const override { return m_iterations; }

private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    BackwardNFAConstSharedPtr m_backwardNfa {};
    int m_maxRecursionDepth {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::vector<Powerset> V,
        int recursionDepth,
        bool isSing
    );

    void addDisjunct(std::vector<Powerset>& V, int state, const Poly& P) const;
    static const Powerset& getVisitedPowerset(std::vector<Powerset>& V, int state);
};