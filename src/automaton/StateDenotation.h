#pragma once

#include "ppl_aliases.h"

class StateDenotation
{
public:
    [[nodiscard]] PowersetConstSharedPtr denotation() const { return m_powerset; }
    [[nodiscard]] const spot::formula& formula() const { return m_formula; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }
    [[nodiscard]] bool isUniverse() const { return m_powerset->is_top(); }
    [[nodiscard]] bool isEmpty() const { return m_powerset->is_empty(); }

    StateDenotation(spot::formula&& formula, PowersetConstSharedPtr powerset, const bool isSing = false)
        : m_formula { std::move(formula) }
        , m_powerset { std::move(powerset) }
        , m_isSing { isSing }
    {
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_formula { std::move(other.m_formula) }
        , m_powerset { std::move(other.m_powerset) }
        , m_isSing { other.isSingular() }
    {
    }

private:
    spot::formula m_formula {};
    PowersetConstSharedPtr m_powerset {};
    bool m_isSing {};
};
