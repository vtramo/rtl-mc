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
    [[nodiscard]] int totalAcceptingTraces() const override { return m_acceptingTraces.size(); }
    [[nodiscard]] const std::vector<std::vector<std::string>>& acceptingTraces() const { return m_acceptingTraces; }

private:
    int m_iterations { };
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    const BackwardNFA& m_backwardNfa {};
    int m_maxRecursionDepth {};
    std::stack<std::string> m_backwardTrace {};
    std::vector<std::vector<std::string>> m_acceptingTraces {};

    PowersetUniquePtr denot(
        int state,
        const Poly& P,
        const Poly& X,
        std::vector<Powerset> V,
        int recursionDepth,
        bool isSing
    );

    void saveAcceptingTrace();
    static void addDisjunct(std::vector<Powerset>& V, int state, const Poly& P);
    static const Powerset& getVisitedPowerset(std::vector<Powerset>& V, int state);
};