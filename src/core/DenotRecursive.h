#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class DenotRecursive: public Denot
{
public:
    DenotRecursive(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {
        const int maxNumberOfPatches { m_backwardNfa.stats().backwardNfaConstructionStats.maxNumberPatches };
        const int maxNumberOfTotalPatches { m_backwardNfa.totalStates() * maxNumberOfPatches };
        m_maxRecursionDepth = 1 + 2 * maxNumberOfTotalPatches;
    }
    ~DenotRecursive() override = default;

    PowersetUniquePtr run() override;

    [[nodiscard]] int totalIterations() const override { return m_iterations; }

private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};
    int m_maxRecursionDepth {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::vector<Powerset> V,
        int recursionDepth,
        bool isSing
    );

    static void addDisjunct(std::vector<Powerset>& V, int state, const Poly& P);
    static const Powerset& getVisitedPowerset(std::vector<Powerset>& V, int state);
};