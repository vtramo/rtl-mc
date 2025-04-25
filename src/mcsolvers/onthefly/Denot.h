#pragma once

#include "BackwardNFA.h"
#include "DenotPathNode.h"
#include "PolyhedralSystem.h"
#include "ppl_aliases.h"

class Denot
{
public:
    Denot(const PolyhedralSystemConstSharedPtr polyhedralSystem, BackwardNFAConstSharedPtr backwardNfa)
      : m_polyhedralSystem { polyhedralSystem }
      , m_backwardNfa { backwardNfa }
    {
    }
    virtual ~Denot() = default;

    PowersetUniquePtr operator() () { return run(); }
    virtual PowersetUniquePtr run() { return PowersetUniquePtr(); }
    virtual int totalIterations() const { return 0; }
    virtual int totalReachCalls() const { return 0; }
    virtual const std::vector<std::vector<DenotPathNode>>& paths() const
    {
        static std::vector<std::vector<DenotPathNode>> empty {};
        return empty;
    }
    virtual int maxIterations() const { return 0; }
    virtual bool isIncompleteResult() const { return false; }
    virtual PowersetConstSharedPtr result() const { return m_result; }
    virtual const PolyhedralSystem& polyhedralSystem() const { return *m_polyhedralSystem; }
    virtual const BackwardNFA& backwardNfa() const { return *m_backwardNfa; }

protected:
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    BackwardNFAConstSharedPtr m_backwardNfa {};
    PowersetConstSharedPtr m_result { std::make_shared<Powerset>(PPL::EMPTY) };
};