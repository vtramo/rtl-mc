#pragma once

#include "SolverStats.h"
#include "DenotOnTheFlyStats.h"

class FiniteOnTheFlySolverStats: public SolverStats
{
public:
    void addDenotOnTheFlyStats(const DenotOnTheFlyStats& denotOnTheFlyStats)
    {
        m_denotOnTheFlyStats.push_back(denotOnTheFlyStats);
        const std::size_t totalDenotOnTheFlyStats { m_denotOnTheFlyStats.size() };
        const std::size_t index { totalDenotOnTheFlyStats - 1 };
        addIndexedPlaceholderValues(denotOnTheFlyStats.getValueByPlaceholderMap(), index);
    }

    void merge(const FiniteOnTheFlySolverStats& other)
    {
        this->SolverStats::merge(other);
        for (const DenotOnTheFlyStats& denotOnTheFlyStats: other.m_denotOnTheFlyStats)
            addDenotOnTheFlyStats(denotOnTheFlyStats);
    }

    FiniteOnTheFlySolverStats merge(const FiniteOnTheFlySolverStats& other) const
    {
        FiniteOnTheFlySolverStats finiteOnTheFlyStats { *this };
        finiteOnTheFlyStats.FiniteOnTheFlySolverStats::merge(other);
        return finiteOnTheFlyStats;
    }
protected:
    std::vector<DenotOnTheFlyStats> m_denotOnTheFlyStats {};
};