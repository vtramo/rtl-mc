#pragma once

#include "PolyhedralSystem.h"
#include "AtomSet.h"
#include "ppl_aliases.h"

class PolyhedralSystemLabelDenotationMap
{
public:
    explicit PolyhedralSystemLabelDenotationMap(const PolyhedralSystemSharedPtr& polyhedralSystem);
    PolyhedralSystemLabelDenotationMap(PolyhedralSystemLabelDenotationMap&& other) noexcept;

    PowersetConstSharedPtr getOrComputeDenotation(const AtomSet& labels);
    [[nodiscard]] const PolyhedralSystem& getPolyhedralSystem() const;
    [[nodiscard]] bool containsDenotation(const AtomSet& labels) const;

    friend std::ostream& operator<< (std::ostream& out, PolyhedralSystemLabelDenotationMap& polyhedralSystemLabelDenotationMap);

private:
    using AtomSetToString = std::string;

    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    std::unordered_map<AtomSetHash, std::tuple<PowersetConstSharedPtr, AtomSetToString>> m_powersetByAtomSet {};

    friend class BackwardNFA;
    PolyhedralSystemLabelDenotationMap() = default;

    PowersetConstSharedPtr computeLabelDenotation(const AtomSet& labels);
    void insertLabelDenotation(const AtomSet& labels, PowersetConstSharedPtr denotation);
    const AtomInterpretation* getAtomInterpretation(const spot::formula& atom) const;
};