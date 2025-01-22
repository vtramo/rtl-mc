#pragma once

#include "ppl_output.h"
#include "ppl_aliases.h"

class StateDenotation
{
public:
    [[nodiscard]] PowersetConstSharedPtr denotation() const { return m_denotation; }
    [[nodiscard]] const spot::formula& formula() const { return m_formula; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }
    [[nodiscard]] bool isUniverse() const { return m_denotation->is_top(); }
    [[nodiscard]] bool isEmpty() const { return m_denotation->is_empty(); }

    void print(std::ostream& ostream, const PolyhedralSystemSymbolTable& symbolTable) const
    {
        ostream << std::boolalpha;
        ostream << "Formula: " << m_formula << '\n';
        ostream << "IsSing: " << m_isSing << '\n';
        ostream << "Denotation: " << PPLOutput::toString(*m_denotation, symbolTable) << '\n';
        ostream << std::noboolalpha;
    }

    StateDenotation(spot::formula&& formula, PowersetConstSharedPtr denotation, const bool isSing = false)
        : m_formula { std::move(formula) }
        , m_denotation { std::move(denotation) }
        , m_isSing { isSing }
    {
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_formula { std::move(other.m_formula) }
        , m_denotation { std::move(other.m_denotation) }
        , m_isSing { other.isSingular() }
    {
    }

private:
    spot::formula m_formula {};
    PowersetConstSharedPtr m_denotation {};
    bool m_isSing {};
};
