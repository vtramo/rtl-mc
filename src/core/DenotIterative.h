#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class DenotIterative: public Denot
{
public:
    DenotIterative(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
    , m_backwardNfa { backwardNfa }
    {}

    ~DenotIterative() override = default;

    PowersetUniquePtr run() override;
    [[nodiscard]] int totalIterations() const override { return m_iterations; }

private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset> V,
        bool isSing
    );

    void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
    Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state) const;
};