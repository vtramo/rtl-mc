#pragma once

#include "FormattableStats.h"

class DenotOnTheFlyStats : public FormattableStats
{
public:
    static const inline std::string s_totalIterationsPlaceHolder{"%Di"};
    static const inline std::string s_executionTimeSecondsPlaceHolder{"%Dx"};
    static const inline std::string s_totalPaths{"%Dp"};
    static const inline std::string s_totalRedundantPaths{"%Dpr"};
    static const inline std::string s_totalReachCalls{"%Drc"};
    static const inline std::string s_result{"%Dr"};
    static const inline std::string s_resultIsIncomplete{"%Dd"};
    static const inline std::string s_maxIterations{"%Dmi"};

    [[nodiscard]] int getTotalIterations() const { return m_totalIterations; }
    [[nodiscard]] double getExecutionTimeSeconds() const { return m_executionTimeSeconds; }
    [[nodiscard]] int getTotalPaths() const { return m_totalPaths; }
    [[nodiscard]] int getTotalRedundantPaths() const { return m_totalRedundantPaths; }
    [[nodiscard]] int getTotalReachCalls() const { return m_totalReachCalls; }
    [[nodiscard]] PowersetConstSharedPtr getResult() const { return m_result; }
    [[nodiscard]] bool getIsIncompleteResult() const { return m_isIncompleteResult; }
    [[nodiscard]] int getMaxNumberIterations() const { return m_maxNumberIterations; }

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

    void setTotalPaths(const int totalPaths)
    {
        m_totalPaths = totalPaths;
        m_valueByPlaceholder[s_totalPaths] = std::to_string(m_totalPaths);
    }

    void setTotalRedundantPaths(const int totalRedundantPaths)
    {
        m_totalRedundantPaths = totalRedundantPaths;
        m_valueByPlaceholder[s_totalRedundantPaths] = std::to_string(totalRedundantPaths);
    }

    void setTotalReachCalls(const int totalReachCalls)
    {
        m_totalReachCalls = totalReachCalls;
        m_valueByPlaceholder[s_totalReachCalls] = std::to_string(m_totalReachCalls);
    }

    void setResult(PowersetConstSharedPtr result, const PolyhedralSystemSymbolTable& symbolTable)
    {
        m_result = result;
        m_valueByPlaceholder[s_result] = PPLOutput::toString(*result, symbolTable);
    }

    void setIsIncompleteResult(const bool isIncomplete)
    {
        m_isIncompleteResult = isIncomplete;
        m_valueByPlaceholder[s_resultIsIncomplete] = std::to_string(m_isIncompleteResult);
    }

    void setMaxNumberIterations(const int maxIterations)
    {
        m_maxNumberIterations = maxIterations;
        m_valueByPlaceholder[s_maxIterations] = std::to_string(m_maxNumberIterations);
    }

protected:
    int m_totalIterations{};
    double m_executionTimeSeconds{};
    int m_totalPaths{};
    int m_totalRedundantPaths{};
    int m_totalReachCalls{};
    int m_maxNumberIterations{};
    bool m_isIncompleteResult{};
    PowersetConstSharedPtr m_result{};
};
