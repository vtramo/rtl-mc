#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class DenotConcurrentV1: public Denot
{
public:
    DenotConcurrentV1(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {
        const std::unordered_set<int>& finalStates { backwardNfa.finalStates() };
        m_denotationFinalStates.reserve(finalStates.size());
        for (const auto& finalState: finalStates)
            m_denotationFinalStates.push_back({ finalState, m_backwardNfa.stateDenotation(finalState).denotation() });
    }
    ~DenotConcurrentV1() override = default;

    PowersetUniquePtr run() override;
    [[nodiscard]] int totalIterations() const override { return 0; /* race conditions, maybe atomic int */ }
private:
    using FinalStateConstIterator = std::unordered_set<int>::const_iterator;

    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};
    std::vector<std::pair<int, PowersetConstSharedPtr>> m_denotationFinalStates {};

    void processFinalStates(int start, int end, Powerset& result);

    Powerset denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::unordered_map<int, Powerset> V,
        bool isSing
    );

    void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
    Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state) const;
};
