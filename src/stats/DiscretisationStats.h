#pragma once

#include "FormattableStats.h"

class DiscretisationStats: public FormattableStats
{
public:
    static const inline std::string s_discreteLtlFormulaTotalAtomicPropositionsPlaceHolder { "%da" };
    static const inline std::string s_discreteLtlFormulaLengthPlaceHolder { "%dl" };
    static const inline std::string s_discretisationExecutionTimeSecondsPlaceHolder { "%dx" };

    [[nodiscard]] int getDiscreteLtlFormulaTotalAtomicPropositions() const { return m_discreteLtlFormulaTotalAtomicPropositions; }
    [[nodiscard]] int getDiscreteLtlFormulaLength() const { return m_discreteLtlFormulaLength; }
    [[nodiscard]] double getDiscretisationExecutionTimeSeconds() const { return m_discretisationExecutionTimeSeconds; }

    void setDiscreteLtlFormulaTotalAtomicPropositions(const int totalAtomicPropositions)
    {
        m_discreteLtlFormulaTotalAtomicPropositions = totalAtomicPropositions;
        m_valueByPlaceholder[s_discreteLtlFormulaTotalAtomicPropositionsPlaceHolder] = std::to_string(m_discreteLtlFormulaTotalAtomicPropositions);
    }

    void setDiscreteLtlFormulaLength(const int formulaLength)
    {
        m_discreteLtlFormulaLength = formulaLength;
        m_valueByPlaceholder[s_discreteLtlFormulaLengthPlaceHolder] = std::to_string(m_discreteLtlFormulaLength);
    }

    void setDiscretisationExecutionTimeSeconds(const double executionTime)
    {
        m_discretisationExecutionTimeSeconds = executionTime;
        m_valueByPlaceholder[s_discretisationExecutionTimeSecondsPlaceHolder] = std::to_string(m_discretisationExecutionTimeSeconds);
    }
protected:
    int m_discreteLtlFormulaTotalAtomicPropositions {};
    int m_discreteLtlFormulaLength {};
    double m_discretisationExecutionTimeSeconds {};
};
