#ifndef LABELDENOTATIONMAP_H
#define LABELDENOTATIONMAP_H

#include "PolyhedralSystem.h"
#include "AtomSet.h"
#include "ppl_aliases.h"
#include <spot/tl/apcollect.hh>

class LabelDenotationMap {
public:
  explicit LabelDenotationMap(const PolyhedralSystem& polyhedralSystem);

  [[nodiscard]] Powerset getDenotation(const AtomSet& labels);
  [[nodiscard]] bool containsDenotation(const AtomSet& labels) const;

  friend std::ostream& operator<< (std::ostream& out, LabelDenotationMap& atomSetDenotationMap);
private:
  std::unordered_map<AtomSet, Powerset> m_denotation {};
  std::reference_wrapper<const PolyhedralSystem> m_polyhedralSystem;
};

#endif //LABELDENOTATIONMAP_H
