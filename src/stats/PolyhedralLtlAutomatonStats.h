#pragma once

#include "AutomatonStats.h"
#include <optional>
#include <spot/twaalgos/sccinfo.hh>

class PolyhedralLtlAutomatonStats : public AutomatonStats
{
public:
    static const inline std::string s_translationOptimizationLevelPlaceHolder { "%ATo" };
    static const inline std::string s_translationExecutionTimeSecondsPlaceHolder { "%ATx" };
    static const inline std::string s_translationTotalStatesPlaceHolder { "%ATs" };
    static const inline std::string s_translationTotalInitialStatesPlaceHolder { "%ATis" };
    static const inline std::string s_translationTotalAcceptingStatesPlaceHolder { "%ATas" };
    static const inline std::string s_translationTotalAcceptingSetsPlaceHolder { "%ATase" };
    static const inline std::string s_translationTotalEdgesPlaceHolder { "%ATe" };
    static const inline std::string s_translationTotalSccInfoPlaceHolder { "%ATscc" };

    static const inline std::string s_optimizedExecutionTimeSecondsPlaceHolder { "%AOx" };
    static const inline std::string s_optimizedTotalStatesPlaceHolder { "%AOs" };
    static const inline std::string s_optimizedTotalInitialStatesPlaceHolder { "%AOis" };
    static const inline std::string s_optimizedTotalAcceptingStatesPlaceHolder { "%AOas" };
    static const inline std::string s_optimizedTotalEdgesPlaceHolder { "%AOe" };
    static const inline std::string s_optimizedTotalSccInfoPlaceHolder { "%AOscc" };

    static const inline std::string s_maxNumberPatchesPlaceHolder { "%Amp" };
    static const inline std::string s_totalNumberPatchesPlaceHolder { "%Atp" };

    [[nodiscard]] const std::string& getTranslationOptimizationLevel() const { return m_translationOptimizationLevel; }
    [[nodiscard]] double getTranslationExecutionTimeSeconds() const { return m_translationExecutionTimeSeconds; }
    [[nodiscard]] int getTranslationTotalAcceptingSets() const { return m_translationTotalAcceptingSets; }
    [[nodiscard]] const std::optional<spot::scc_info>& getTranslationSccInfo() const { return m_translationSccInfo; }
    [[nodiscard]] int getMaxNumberPatches() const { return m_maxNumberPatches; }
    [[nodiscard]] int getTotalNumberPatches() const { return m_totalNumberPatches; }
    [[nodiscard]] double getOptimizedAutomatonExecutionTimeSeconds() const { return m_optimizedExecutionTimeSeconds; }
    [[nodiscard]] int getOptimizedAutomatonTotalStates() const { return m_optimizedTotalStates; }
    [[nodiscard]] int getOptimizedAutomatonTotalInitialStates() const { return m_optimizedTotalInitialStates; }
    [[nodiscard]] int getOptimizedAutomatonTotalAcceptingStates() const { return m_optimizedTotalAcceptingStates; }
    [[nodiscard]] int getOptimizedAutomatonTotalEdges() const { return m_optimizedTotalEdges; }
    [[nodiscard]] const std::optional<spot::scc_info>& getOptimizedAutomatonSccInfo() const { return m_optimizedSccInfo; }

    void setTranslationOptimizationLevel(const std::string_view level)
    {
        m_translationOptimizationLevel = level;
        m_valueByPlaceholder[s_translationOptimizationLevelPlaceHolder] = level;
    }

    void setTranslationExecutionTimeSeconds(const double seconds)
    {
        m_translationExecutionTimeSeconds = seconds;
        m_valueByPlaceholder[s_translationExecutionTimeSecondsPlaceHolder] = std::to_string(seconds);
    }

    void setTranslationTotalStates(const int totalStates)
    {
        m_translationTotalStates = totalStates;
        m_valueByPlaceholder[s_translationTotalStatesPlaceHolder] = std::to_string(totalStates);
    }

    void setTranslationTotalInitialStates(const int totalInitialStates)
    {
        m_translationTotalInitialStates = totalInitialStates;
        m_valueByPlaceholder[s_translationTotalInitialStatesPlaceHolder] = std::to_string(totalInitialStates);
    }

    void setTranslationTotalAcceptingStates(const int totalAcceptingStates)
    {
        m_translationTotalAcceptingStates = totalAcceptingStates;
        m_valueByPlaceholder[s_translationTotalAcceptingStatesPlaceHolder] = std::to_string(totalAcceptingStates);
    }

    void setTranslationTotalEdges(const int totalEdges)
    {
        m_translationTotalEdges = totalEdges;
        m_valueByPlaceholder[s_translationTotalEdgesPlaceHolder] = std::to_string(totalEdges);
    }

    void setTranslationTotalAcceptingSets(const int totalAcceptingSets)
    {
        m_translationTotalAcceptingSets = totalAcceptingSets;
        m_valueByPlaceholder[s_translationTotalAcceptingSetsPlaceHolder] = std::to_string(totalAcceptingSets);
    }

    void setTranslationSccInfo(const spot::scc_info& translationSccInfo)
    {
        m_translationSccInfo = translationSccInfo;
        m_valueByPlaceholder[s_translationTotalSccInfoPlaceHolder] = std::to_string(translationSccInfo.scc_count());
    }

    void setMaxNumberPatches(const int maxPatches)
    {
        m_maxNumberPatches = maxPatches;
        m_valueByPlaceholder[s_maxNumberPatchesPlaceHolder] = std::to_string(maxPatches);
    }

    void setTotalNumberPatches(const int totalPatches)
    {
        m_totalNumberPatches = totalPatches;
        m_valueByPlaceholder[s_totalNumberPatchesPlaceHolder] = std::to_string(totalPatches);
    }


    void setOptimizedAutomatonExecutionTimeSeconds(const double seconds)
    {
        m_optimizedExecutionTimeSeconds = seconds;
        m_valueByPlaceholder[s_optimizedExecutionTimeSecondsPlaceHolder] = std::to_string(seconds);
    }

    void setOptimizedAutomatonTotalStates(const int totalStates)
    {
        m_optimizedTotalStates = totalStates;
        m_valueByPlaceholder[s_optimizedTotalStatesPlaceHolder] = std::to_string(totalStates);
    }

    void setOptimizedAutomatonTotalInitialStates(const int totalInitialStates)
    {
        m_optimizedTotalInitialStates = totalInitialStates;
        m_valueByPlaceholder[s_optimizedTotalInitialStatesPlaceHolder] = std::to_string(totalInitialStates);
    }

    void setOptimizedAutomatonTotalAcceptingStates(const int totalAcceptingStates)
    {
        m_optimizedTotalAcceptingStates = totalAcceptingStates;
        m_valueByPlaceholder[s_optimizedTotalAcceptingStatesPlaceHolder] = std::to_string(totalAcceptingStates);
    }

    void setOptimizedAutomatonTotalEdges(const int totalEdges)
    {
        m_optimizedTotalEdges = totalEdges;
        m_valueByPlaceholder[s_optimizedTotalEdgesPlaceHolder] = std::to_string(totalEdges);
    }

    void setOptimizedAutomatonSccInfo(const spot::scc_info& optimizedSccInfo)
    {
        m_optimizedSccInfo = optimizedSccInfo;
        m_valueByPlaceholder[s_optimizedTotalSccInfoPlaceHolder] = std::to_string(optimizedSccInfo.scc_count());
    }

protected:
    std::string m_translationOptimizationLevel {};
    double m_translationExecutionTimeSeconds {};
    int m_translationTotalStates {};
    int m_translationTotalEdges {};
    int m_translationTotalInitialStates {};
    int m_translationTotalAcceptingStates {};
    int m_translationTotalAcceptingSets {};
    std::optional<spot::scc_info> m_translationSccInfo {};

    double m_optimizedExecutionTimeSeconds {};
    int m_optimizedTotalStates {};
    int m_optimizedTotalEdges {};
    int m_optimizedTotalInitialStates {};
    int m_optimizedTotalAcceptingStates {};
    std::optional<spot::scc_info> m_optimizedSccInfo {};

    int m_maxNumberPatches {};
    int m_totalNumberPatches {};
};