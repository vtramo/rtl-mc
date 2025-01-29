#pragma once

#include <future>

#include "PolyhedralSystem.h"
#include "BackwardNFA.h"
#include "ppl_utils.h"

inline namespace V1Recursive
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {}

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return m_iterations; }

    private:
        int m_iterations { };
        PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
        const BackwardNFA& m_backwardNfa {};

        PowersetUniquePtr denot(
            int state,
            const Poly& P,
            const Poly& X,
            std::unordered_map<int, Powerset>& V,
            bool isSing
        );

        void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
        Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state) const;
    };
}


namespace V2Iterative
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
        , m_backwardNfa { backwardNfa }
        {}

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return m_iterations; }

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
}

namespace V1Concurrent
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {
            const std::unordered_set<int>& finalStates { backwardNfa.finalStates() };
            m_denotationFinalStates.reserve(finalStates.size());
            for (const auto& finalState: finalStates)
                m_denotationFinalStates.push_back({ finalState, m_backwardNfa.stateDenotation(finalState).denotation() });
        }

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return 0; /* race conditions, maybe atomic int */ }

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
}

namespace V2Concurrent
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {}

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return m_iterations; }

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

        std::future<std::pair<int, PPLUtils::ReachPairs>> computeReachPairsAsync(
            int predecessor,
            std::unordered_map<int, Powerset>& V,
            const Poly& X
        );

        std::pair<int, PPLUtils::ReachPairs> computeReachPairs(
          int predecessor,
          std::unordered_map<int, Powerset>& V,
          const Poly& X
        );

        void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
        Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state) const;
    };
}

namespace V3Concurrent
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
        , m_backwardNfa { backwardNfa }
        {}

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return m_iterations; }

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

        std::future<std::pair<int, PPLUtils::ReachPairs>> computeReachPairsAsync(
            int predecessor,
            const Powerset& predecessorVisitedRegion,
            const Poly& X
        );

        std::pair<int, PPLUtils::ReachPairs> computeReachPairs(
          int predecessor,
          const Powerset& predecessorVisitedRegion,
          const Poly& X
        ) const;

        void addDisjunct(std::unordered_map<int, Powerset>& V, int state, const Poly& P) const;
        void insertEmptyPredecessorVisitedPowersetsIfNotExist(std::unordered_map<int, Powerset>& V, int state) const;
        Powerset& getVisitedPowerset(std::unordered_map<int, Powerset>& V, int state);
    };
}

namespace V4Concurrent
{
    class Denot
    {
    public:
        Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, const BackwardNFA& backwardNfa)
          : m_polyhedralSystem { polyhedralSystem }
          , m_backwardNfa { backwardNfa }
        {
            m_spaceDimension = m_polyhedralSystem->getSpaceDimension();
        }

        PowersetUniquePtr operator() () { return run(); }
        PowersetUniquePtr run();

        [[nodiscard]] int totalIterations() const { return m_iterations; }

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
}