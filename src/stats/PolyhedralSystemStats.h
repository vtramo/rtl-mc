#pragma once

#include "FormattableStats.h"

class PolyhedralSystemStats: public FormattableStats
{
public:
    static const inline std::string s_totalAtomicPropositionsPlaceHolder { "%Pta" };
    static const inline std::string s_spaceDimensionPlaceHolder { "%Psp" };

    [[nodiscard]] int getTotalAtomicPropositions() const { return m_totalAtomicPropositions; }
    [[nodiscard]] int getSpaceDimension() const { return m_spaceDimension; }

    void setTotalAtomicPropositions(const int totalAtomicPropositions)
    {
        m_totalAtomicPropositions = totalAtomicPropositions;
        m_valueByPlaceholder[s_totalAtomicPropositionsPlaceHolder] = std::to_string(m_totalAtomicPropositions);
    }

    void setSpaceDimension(const int spaceDimension)
    {
        m_spaceDimension = spaceDimension;
        m_valueByPlaceholder[s_spaceDimensionPlaceHolder] = std::to_string(m_spaceDimension);
    }
protected:
    int m_totalAtomicPropositions {};
    int m_spaceDimension {};
};