#pragma once

#include "Denot.h"
#include "BackwardNFA.h"
#include "PolyhedralSystem.h"
#include "ppl_utils.h"

class DenotConcurrentV4: public Denot
{
public:
    DenotConcurrentV4(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {
        m_spaceDimension = m_polyhedralSystem->spaceDimension();
    }
    ~DenotConcurrentV4() override = default;

    PowersetUniquePtr run() override;
    [[nodiscard]] int totalIterations() const override { return m_iterations; }
private:
    int m_iterations { };
    PPL::dimension_type m_spaceDimension { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset> V,
        bool isSing
    );

    std::pair<int, PPLUtils::ReachPairs> computeReachPairs(
        int predecessor,
        const Powerset& predecessorVisitedPatches,
        const Poly& X
    ) const;

    void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
    Powerset& getVisitedPatches(std::unordered_map<int, Powerset>& V, int state) const;
};
