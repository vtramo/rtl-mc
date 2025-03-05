#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
#include "reach.h"
#include <future>

class DenotConcurrentV2: public Denot
{
public:
    DenotConcurrentV2(const PolyhedralSystemConstSharedPtr polyhedralSystem, BackwardNFAConstSharedPtr backwardNfa)
        : m_polyhedralSystem { polyhedralSystem }
        , m_backwardNfa { backwardNfa }
    {}
    ~DenotConcurrentV2() override = default;

    PowersetUniquePtr run() override;
    [[nodiscard]] int totalIterations() const override { return m_iterations; }
private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    BackwardNFAConstSharedPtr m_backwardNfa {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset> V,
        bool isSing
    );

    std::future<std::pair<int, ReachPairs>> computeReachPairsAsync(
        int predecessor,
        std::unordered_map<int, Powerset>& V,
        const Poly& X
    );

    std::pair<int, ReachPairs> computeReachPairs(
      int predecessor,
      std::unordered_map<int, Powerset>& V,
      const Poly& X
    );

    void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
    Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state) const;
};