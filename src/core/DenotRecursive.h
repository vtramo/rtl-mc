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
        m_maxRecursionDepth = backwardNfa.maxRecursiveDepth();
    }
    ~DenotRecursive() override = default;
    PowersetUniquePtr run() override;

    [[nodiscard]] int totalIterations() const override { return m_iterations; }

private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};
    int m_maxRecursionDepth {};

    std::vector<Powerset> initializeUnvisitedMap() const;
    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::vector<Powerset> V,
        int recursionDepth,
        bool isSing
    );
};