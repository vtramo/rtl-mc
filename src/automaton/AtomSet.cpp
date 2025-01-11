#include "AtomSet.h"

#include <spot_constants.h>
#include <string>

AtomSet::AtomSet(spot::atomic_prop_set&& atomSet)
    : m_atoms { std::move(atomSet) }
{
    m_hashcode = std::hash<AtomSet>::hashCode(this);
}

AtomSet::AtomSet(AtomSet&& other) noexcept
    : m_atoms { std::move(other.m_atoms) }
{
    m_hashcode = other.m_hashcode;
    other.m_hashcode = {};
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
    return m_atoms.count(atom);
}

bool AtomSet::isEmpty() const
{
    return m_atoms.empty();
}

size_t AtomSet::hash() const
{
    return std::hash<AtomSet>::hashCode(this);
}

bool operator== (const AtomSet& atomSet1, const AtomSet& atomSet2)
{
    return atomSet1.m_hashcode == atomSet2.m_hashcode;
}

bool operator!= (const AtomSet& atomSet1, const AtomSet& atomSet2)
{
    return !(atomSet1 == atomSet2);
}

std::ostream& operator<< (std::ostream& out, const AtomSet& atomSet)
{
    if (atomSet.isEmpty()) return out << "{}";

    out << "{ ";
    bool first = true;
    for (const auto& atom: atomSet)
    {
        out << (first ? "" : ", ") + atom.ap_name();
        first = false;
    }
    return out << " }";
}