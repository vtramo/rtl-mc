#ifndef ATOMSET_H
#define ATOMSET_H

#include <spot_constants.h>
#include <spot/tl/apcollect.hh>

class AtomSet
{
public:
    using AtomSetIterator = std::set<spot::formula>::iterator;

    AtomSet() = default;

    explicit AtomSet(spot::atomic_prop_set&& atomSet);
    AtomSet(AtomSet&&) noexcept;

    [[nodiscard]] const spot::atomic_prop_set& atoms() const;
    [[nodiscard]] int size() const;
    [[nodiscard]] bool containsAtom(const spot::formula& atom) const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] AtomSetIterator begin() const { return m_atoms.begin(); }
    [[nodiscard]] AtomSetIterator end() const { return m_atoms.end(); }
    [[nodiscard]] size_t hash() const;

    friend bool operator== (const AtomSet& atomSet1, const AtomSet& atomSet2);
    friend std::ostream& operator<< (std::ostream& out, const AtomSet& atomSet);
    friend struct std::hash<AtomSet>;

private:
    spot::atomic_prop_set m_atoms {};
    std::size_t m_hashcode {};
};

bool operator!= (const AtomSet& atomSet1, const AtomSet& atomSet2);

template <>
struct std::hash<AtomSet>
{
    static std::size_t hashCode(const AtomSet* const atomSet) noexcept
    {
        std::string result {};
        result.reserve(atomSet->size());
        for (const auto& atom: atomSet->atoms())
        {
            result += atom.ap_name();
        }
        return std::hash<std::string>{}(result);
    }

    std::size_t operator() (const AtomSet* const atomSet) const noexcept
    {
        return atomSet->m_hashcode;
    }

    std::size_t operator() (const AtomSet& atomSet) const noexcept
    {
        return operator() (&atomSet);
    }
};

#endif //ATOMSET_H
