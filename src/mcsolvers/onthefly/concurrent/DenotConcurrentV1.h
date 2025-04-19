#pragma once

#include "Denot.h"
#include "PolyhedralSystem.h"
#include "BackwardNFA.h"

class DenotConcurrentV1: public Denot
{
public:
    DenotConcurrentV1(const PolyhedralSystemConstSharedPtr polyhedralSystem, BackwardNFAConstSharedPtr backwardNfa)
        : Denot(polyhedralSystem, backwardNfa)
    {
        const std::unordered_set<unsigned>& acceptingStates { backwardNfa->acceptingStates() };
        m_denotationAcceptingStates.reserve(acceptingStates.size());
        for (const auto& acceptingState: acceptingStates)
            m_denotationAcceptingStates.push_back({ acceptingState, m_backwardNfa->stateDenotation(acceptingState).denotation() });
    }
    ~DenotConcurrentV1() override = default;

    PowersetUniquePtr run() override;
    [[nodiscard]] int totalIterations() const override { return 0; /* race conditions, maybe atomic int */ }
private:
    using FinalStateConstIterator = std::unordered_set<int>::const_iterator;

    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    BackwardNFAConstSharedPtr m_backwardNfa {};
    std::vector<std::pair<int, PowersetConstSharedPtr>> m_denotationAcceptingStates {};

    void processAcceptingStates(int start, int end, Powerset& result);

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
