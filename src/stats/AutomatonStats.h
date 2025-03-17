#pragma once

#include <optional>

#include "FormattableStats.h"

class AutomatonStats: public FormattableStats
{
public:
    static const inline std::string s_totalStatesPlaceHolder { "%As" };
    static const inline std::string s_totalInitialStatesPlaceHolder { "%Ais" };
    static const inline std::string s_totalAcceptingStatesPlaceHolder { "%Aas" };
    static const inline std::string s_totalEdgesPlaceHolder { "%Ae" };
    static const inline std::string s_executionTimeSecondsPlaceHolder { "%Ax" };
    static const inline std::string s_totalSccPlaceHolder { "%Ascc" };

    [[nodiscard]] int getTotalStates() const { return m_totalStates; }
    [[nodiscard]] int getTotalEdges() const { return m_totalEdges; }
    [[nodiscard]] int getTotalInitialStates() const { return m_totalInitialStates; }
    [[nodiscard]] int getTotalAcceptingStates() const { return m_totalAcceptingStates; }
    [[nodiscard]] double getExecutionTimeSeconds() const { return m_executionTimeSeconds; }
    [[nodiscard]] const std::optional<spot::scc_info>& getSccInfo() const { return m_sccInfo; }

    void setTotalStates(const int totalStates)
    {
        m_totalStates = totalStates;
        m_valueByPlaceholder[s_totalStatesPlaceHolder] = std::to_string(totalStates);
    }

    void setTotalEdges(const int totalEdges)
    {
        m_totalEdges = totalEdges;
        m_valueByPlaceholder[s_totalEdgesPlaceHolder] = std::to_string(totalEdges);
    }

    void setTotalInitialStates(const int initialStates)
    {
        m_totalInitialStates = initialStates;
        m_valueByPlaceholder[s_totalInitialStatesPlaceHolder] = std::to_string(initialStates);
    }

    void setTotalAcceptingStates(const int totalAcceptingStates)
    {
        m_totalAcceptingStates = totalAcceptingStates;
        m_valueByPlaceholder[s_totalAcceptingStatesPlaceHolder] = std::to_string(totalAcceptingStates);
    }

    void setExecutionTimeSeconds(const double seconds)
    {
        m_executionTimeSeconds = seconds;
        m_valueByPlaceholder[s_executionTimeSecondsPlaceHolder] = std::to_string(seconds);
    }

    void setSccInfo(const spot::scc_info& sccInfo)
    {
        m_sccInfo = sccInfo;
        m_valueByPlaceholder[s_totalSccPlaceHolder] = std::to_string(sccInfo.scc_count());
    }

protected:
    int m_totalStates {};
    int m_totalEdges {};
    int m_totalInitialStates {};
    int m_totalAcceptingStates {};
    std::optional<spot::scc_info> m_sccInfo {};
    double m_executionTimeSeconds {};
};
