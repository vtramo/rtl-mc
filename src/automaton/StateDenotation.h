#pragma once

#include "ppl_aliases.h"
#include "AtomSet.h"

class StateDenotation
{
public:
    [[nodiscard]] const Powerset& denotation() const { return m_powerset; }
    [[nodiscard]] const AtomSet& labels() const { return m_labels; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }

    StateDenotation(AtomSet&& labels, Powerset&& powerset)
        : m_labels { std::move(labels) }
    {
        m_powerset.m_swap(powerset);
        m_isSing = m_labels.containsAtom(SpotUtils::sing());
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_labels { std::move(other.m_labels) }
    {
        m_powerset.m_swap(other.m_powerset);
        m_isSing = other.m_isSing;
        other.m_isSing = false;
    }

private:
    Powerset m_powerset {};
    AtomSet m_labels {};
    bool m_isSing {};
};
