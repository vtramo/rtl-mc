#ifndef ATOMSET_H
#define ATOMSET_H

#include <string>
#include <spot/tl/apcollect.hh>

class AtomSet {
public:
  AtomSet() = default;

  explicit AtomSet(spot::atomic_prop_set&& atomSet)
      : m_atoms { std::move(atomSet) }
  {
    m_hashcode = std::hash<std::string>{}(this->toString());
  }

  [[nodiscard]] const spot::atomic_prop_set& atoms() const { return m_atoms; }
  [[nodiscard]] bool containsAtom(const spot::formula& atom) const { return m_atoms.count(atom); }
  [[nodiscard]] std::string toString() const
  {
    std::string result {};
    result.reserve(m_atoms.size());
    result += "{";
    bool first = true;
    for (const auto& atom : m_atoms)
    {
      result += (first ? "" : ", ") + atom.ap_name();
      first = false;
    }
    return result += "}";
  }

  friend bool operator== (const AtomSet& atomSet1, const AtomSet& atomSet2);
  friend struct std::hash<AtomSet>;
private:
  spot::atomic_prop_set m_atoms {};
  std::size_t m_hashcode {};
};

inline bool operator== (const AtomSet& atomSet1, const AtomSet& atomSet2)
{
  return atomSet1.m_hashcode == atomSet2.m_hashcode;
}

inline bool operator!= (const AtomSet& atomSet1, const AtomSet& atomSet2)
{
  return !(atomSet1 == atomSet2);
}

inline std::ostream& operator<< (std::ostream& out, const AtomSet& atomSet)
{
  return out << atomSet.toString();
}

template<>
struct std::hash<AtomSet> {
  std::size_t operator()(const AtomSet& atomSet) const noexcept
  {
    return atomSet.m_hashcode;
  }
};

#endif //ATOMSET_H