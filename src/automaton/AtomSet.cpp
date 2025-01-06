#include "AtomSet.h"

#include <spot_constants.h>
#include <string>

AtomSet::AtomSet(spot::atomic_prop_set&& atomSet, const bool excludeSingForOptimization)
    : m_atoms { std::move(atomSet) }
{
    if (excludeSingForOptimization)
    {
        m_isSingRemoved = m_atoms.erase(spot::constants::g_sing);
    }

    m_hashcode = std::hash<AtomSet>{}(this);
}

const spot::atomic_prop_set& AtomSet::atoms() const
{
    return m_atoms;
}

int AtomSet::size() const
{
    return m_atoms.size();
}

bool AtomSet::containsAtom(const spot::formula& atom) const
{
    if (m_isSingRemoved && spot::constants::g_sing == atom)
    {
        return true;
    }

    return m_atoms.count(atom);
}

bool AtomSet::isEmpty() const
{
    return m_isSingRemoved || m_atoms.empty();
}

bool operator==(const AtomSet& atomSet1, const AtomSet& atomSet2)
{
    return atomSet1.m_hashcode == atomSet2.m_hashcode;
}

bool operator!=(const AtomSet& atomSet1, const AtomSet& atomSet2)
{
    return !(atomSet1 == atomSet2);
}

std::ostream& operator<<(std::ostream& out, const AtomSet& atomSet)
{
    out << "{" + (atomSet.m_isSingRemoved ? spot::constants::g_sing.ap_name() : "");
    bool first = !atomSet.m_isSingRemoved;
    for (const auto& atom: atomSet)
    {
        out << (first ? "" : ", ") + atom.ap_name();
        first = false;
    }
    return out << "}";
}