#pragma once

#include "FormattableStats.h"

class RtlFormulaStats: public FormattableStats
{
public:
    static const inline std::string s_totalAtomicPropositionsPlaceHolder { "%rta" };
    static const inline std::string s_lengthPlaceHolder { "%rl" };

    [[nodiscard]] int getTotalAtomicPropositions() const { return m_totalAtomicPropositions; }
    [[nodiscard]] int getLength() const { return m_length; }

    void setTotalAtomicPropositions(const int totalAtomicPropositions)
    {
        m_totalAtomicPropositions = totalAtomicPropositions;
        m_valueByPlaceholder[s_totalAtomicPropositionsPlaceHolder] = std::to_string(m_totalAtomicPropositions);
    }

    void setLength(const int length)
    {
        m_length = length;
        m_valueByPlaceholder[s_lengthPlaceHolder] = std::to_string(m_length);
    }
protected:
    int m_totalAtomicPropositions {};
    int m_length {};
};