#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
#include "reach.h"
#include <future>

class DenotConcurrentV3: public Denot
{
public:
    DenotConcurrentV3(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
    , m_backwardNfa { backwardNfa }
    {}
    ~DenotConcurrentV3() override = default;

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

    std::future<std::pair<int, ReachPairs>> computeReachPairsAsync(
        int predecessor,
        const Powerset& predecessorVisitedRegion,
        const Poly& X
    );

    std::pair<int, ReachPairs> computeReachPairs(
      int predecessor,
      const Powerset& predecessorVisitedRegion,
      const Poly& X
    ) const;

    void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
    void insertEmptyPredecessorVisitedPowersetsIfNotExist(std::unordered_map<int, Powerset>& V, int state) const;
    Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state);
};
