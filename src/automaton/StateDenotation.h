#pragma once

#include "ppl_aliases.h"
#include "AtomSet.h"

class StateDenotation
{
public:
    [[nodiscard]] PowersetConstSharedPtr denotation() const { return m_powerset; }
    [[nodiscard]] const AtomSet& labels() const { return m_labels; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }

    StateDenotation(AtomSet&& labels, PowersetConstSharedPtr powerset, const bool isSing = false)
        : m_labels { std::move(labels) }
        , m_powerset { std::move(powerset) }
        , m_isSing { isSing }
    {
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_labels { std::move(other.m_labels) }
        , m_powerset { std::move(other.m_powerset) }
        , m_isSing { other.isSingular() }
    {
    }

private:
    AtomSet m_labels {};
    PowersetConstSharedPtr m_powerset {};
    bool m_isSing {};
};
