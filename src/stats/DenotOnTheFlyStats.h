#pragma once

#include "FormattableStats.h"

class DenotOnTheFlyStats: public FormattableStats
{
public:
    static const inline std::string s_totalIterationsPlaceHolder { "%Di" };
    static const inline std::string s_executionTimeSecondsPlaceHolder { "%Dx" };

    [[nodiscard]] int getTotalIterations() const { return m_totalIterations; }
    [[nodiscard]] double getExecutionTimeSeconds() const { return m_executionTimeSeconds; }

    void setTotalIterations(const int totalIterations)
    {
        m_totalIterations = totalIterations;
        m_valueByPlaceholder[s_totalIterationsPlaceHolder] = std::to_string(m_totalIterations);
    }

    void setExecutionTimeSeconds(const double executionTimeSeconds)
    {
        m_executionTimeSeconds = executionTimeSeconds;
        m_valueByPlaceholder[s_executionTimeSecondsPlaceHolder] = std::to_string(m_executionTimeSeconds);
    }

protected:
    int m_totalIterations {};
    double m_executionTimeSeconds {};
};