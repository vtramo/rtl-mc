#pragma once

#include "PolyhedralLtlAutomatonStats.h"

class PolyhedralLtlFiniteAutomatonStats: public PolyhedralLtlAutomatonStats
{
public:
    static const inline std::string s_nfaExecutionTimeSecondsPlaceHolder { "%ANx" };
    static const inline std::string s_nfaTotalStatesPlaceHolder { "%ANs" };
    static const inline std::string s_nfaTotalInitialStatesPlaceHolder { "%ANis" };
    static const inline std::string s_nfaTotalAcceptingStatesPlaceHolder { "%ANas" };
    static const inline std::string s_nfaTotalEdgesPlaceHolder { "%ANe" };
    static const inline std::string s_nfaTotalSccInfoPlaceHolder { "%ANscc" };
    static const inline std::string s_maxRecursiveDepthOnTheFly { "%ANr" };

    [[nodiscard]] int getNfaTotalStates() const { return m_nfaTotalStates; }
    [[nodiscard]] int getNfaTotalInitialStates() const { return m_nfaTotalInitialStates; }
    [[nodiscard]] int getNfaTotalAcceptingStates() const { return m_nfaTotalAcceptingStates; }
    [[nodiscard]] int getNfaTotalEdges() const { return m_nfaTotalEdges; }
    [[nodiscard]] std::optional<spot::scc_info> getNfaSccInfo() const { return m_nfaSccInfo; }
    [[nodiscard]] double getNfaExecutionTimeSeconds() const { return m_nfaExecutionTimeSeconds; }
    [[nodiscard]] int getNfaMaxRecursiveDepth() const { return m_nfaMaxRecursiveDepth; }

    void setNfaTotalStates(const int nfaTotalStates)
    {
        m_nfaTotalStates = nfaTotalStates;
        m_valueByPlaceholder[s_nfaTotalStatesPlaceHolder] = std::to_string(nfaTotalStates);
    }

    void setNfaTotalInitialStates(const int nfaTotalInitialStates)
    {
        m_nfaTotalInitialStates = nfaTotalInitialStates;
        m_valueByPlaceholder[s_nfaTotalInitialStatesPlaceHolder] = std::to_string(nfaTotalInitialStates);
    }

    void setNfaTotalAcceptingStates(const int nfaTotalAcceptingStates)
    {
        m_nfaTotalAcceptingStates = nfaTotalAcceptingStates;
        m_valueByPlaceholder[s_nfaTotalAcceptingStatesPlaceHolder] = std::to_string(nfaTotalAcceptingStates);
    }

    void setNfaTotalEdges(const int nfaTotalEdges)
    {
        m_nfaTotalEdges = nfaTotalEdges;
        m_valueByPlaceholder[s_nfaTotalEdgesPlaceHolder] = std::to_string(nfaTotalEdges);
    }

    void setNfaSccInfo(const spot::scc_info& nfaSccInfo)
    {
        m_nfaSccInfo = nfaSccInfo;
        m_valueByPlaceholder[s_nfaTotalSccInfoPlaceHolder] = std::to_string(nfaSccInfo.scc_count());
    }

    void setNfaExecutionTimeSeconds(const double nfaExecutionTimeSeconds)
    {
        m_nfaExecutionTimeSeconds = nfaExecutionTimeSeconds;
        m_valueByPlaceholder[s_nfaExecutionTimeSecondsPlaceHolder] = std::to_string(nfaExecutionTimeSeconds);
    }

    void setNfaMaxRecursiveDepth(const int nfaMaxRecursiveDepth)
    {
        m_nfaMaxRecursiveDepth = nfaMaxRecursiveDepth;
        m_valueByPlaceholder[s_maxRecursiveDepthOnTheFly] = std::to_string(nfaMaxRecursiveDepth);
    }

protected:
    int m_nfaTotalStates {};
    int m_nfaTotalInitialStates {};
    int m_nfaTotalAcceptingStates {};
    int m_nfaTotalEdges {};
    std::optional<spot::scc_info> m_nfaSccInfo {};
    double m_nfaExecutionTimeSeconds {};
    int m_nfaMaxRecursiveDepth {};
};