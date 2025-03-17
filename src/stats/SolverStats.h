#pragma once

#include "FormattableStats.h"
#include "PolyhedralSystemStats.h"
#include "DiscretisationStats.h"
#include "AutomatonStats.h"
#include "RtlFormulaStats.h"
#include <vector>
#include <fmt/format.h>

class SolverStats: public FormattableStats
{
public:
    static const inline std::string s_executionTimePlaceHolder { "%x" };

    void addExecutionTime(const double executionTimeSeconds)
    {
        m_executionTimesSeconds.push_back(executionTimeSeconds);
        const std::size_t totalExecutionTimes { m_executionTimesSeconds.size() };
        const std::size_t index { totalExecutionTimes - 1 };
        std::string indexedPlaceholder { fmt::format("{}[{}]", s_executionTimePlaceHolder, index) };
        m_valueByPlaceholder[indexedPlaceholder] = std::to_string(executionTimeSeconds);
    }

    void addRtlFormulaStats(const RtlFormulaStats& rtlFormulaStats)
    {
        m_rtlFormulaStats.push_back(rtlFormulaStats);
        const std::size_t totalRtlFormulaStats { m_rtlFormulaStats.size() };
        const std::size_t index { totalRtlFormulaStats - 1 };
        addIndexedPlaceholderValues(rtlFormulaStats.getValueByPlaceholderMap(), index);
    }

    void addPolyhedralSystemStats(const PolyhedralSystemStats& polyhedralSystemStats)
    {
        m_polyhedralSystemStats.push_back(polyhedralSystemStats);
        const std::size_t totalPolyhedralSystemStats { m_polyhedralSystemStats.size() };
        const std::size_t index { totalPolyhedralSystemStats - 1 };
        addIndexedPlaceholderValues(polyhedralSystemStats.getValueByPlaceholderMap(), index);
    }

    void addDiscretisationStats(const DiscretisationStats& discretisationStats)
    {
        m_discretisationStats.push_back(discretisationStats);
        const std::size_t totalDiscretisationStats { m_discretisationStats.size() };
        const std::size_t index { totalDiscretisationStats - 1 };
        addIndexedPlaceholderValues(discretisationStats.getValueByPlaceholderMap(), index);
    }

    void addAutomatonStats(const AutomatonStats& automatonStats)
    {
        m_automataStats.push_back(automatonStats);
        const std::size_t totalAutomataStats { m_automataStats.size() };
        const std::size_t index { totalAutomataStats - 1 };
        addIndexedPlaceholderValues(automatonStats.getValueByPlaceholderMap(), index);
    }

    void merge(const SolverStats& other)
    {
        for (const auto& executionTime: other.m_executionTimesSeconds)
            addExecutionTime(executionTime);
        for (const auto& rtlFormula: other.m_rtlFormulaStats)
            addRtlFormulaStats(rtlFormula);
        for (const auto& discretisation: other.m_discretisationStats)
            addDiscretisationStats(discretisation);
        for (const auto& automatonStats: other.m_automataStats)
            addAutomatonStats(automatonStats);
        for (const auto& polyhedralSystemStats: other.m_polyhedralSystemStats)
            addPolyhedralSystemStats(polyhedralSystemStats);
    }

    SolverStats merge(const SolverStats& other) const
    {
        SolverStats mergedSolverStats { *this };
        mergedSolverStats.merge(other);
        return mergedSolverStats;
    }

protected:
    std::vector<double> m_executionTimesSeconds {};
    std::vector<PolyhedralSystemStats> m_polyhedralSystemStats {};
    std::vector<DiscretisationStats> m_discretisationStats {};
    std::vector<AutomatonStats> m_automataStats {};
    std::vector<RtlFormulaStats> m_rtlFormulaStats {};

    void addIndexedPlaceholderValues(const std::unordered_map<std::string, std::string>& valueByPlaceholder, std::size_t index)
    {
        for (const auto& [placeholder, value]: valueByPlaceholder)
        {
            std::string indexedPlaceholder { fmt::format("{}[{}]", placeholder, index) };
            m_valueByPlaceholder[indexedPlaceholder] = value;
        }
    }
};