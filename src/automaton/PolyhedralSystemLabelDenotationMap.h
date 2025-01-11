#pragma once

#include "PolyhedralSystem.h"
#include "AtomSet.h"
#include "ppl_aliases.h"

class PolyhedralSystemLabelDenotationMap
{
public:
    explicit PolyhedralSystemLabelDenotationMap(const PolyhedralSystemSharedPtr& polyhedralSystem);
    PolyhedralSystemLabelDenotationMap(PolyhedralSystemLabelDenotationMap&& other) noexcept;

    [[nodiscard]] const PolyhedralSystem& getPolyhedralSystem() const;
    [[nodiscard]] PowersetUniquePtr getDenotation(const AtomSet& labels);
    [[nodiscard]] bool containsDenotation(const AtomSet& labels) const;

    friend std::ostream& operator<< (std::ostream& out, PolyhedralSystemLabelDenotationMap& PolyhedralSystemLabelDenotationMap);

private:
    using LabelsHash = std::size_t;

    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    std::unordered_map<LabelsHash, PowersetSharedPtr> m_powersetByLabelsHash {};

    friend class BackwardNFA;
    PolyhedralSystemLabelDenotationMap() = default;

    PowersetSharedPtr computeLabelDenotation(const AtomSet& labels);
    void insertLabelDenotation(LabelsHash labelsHash, LabelsHash labelsHashWithoutSing, PowersetSharedPtr denotation);
    void insertLabelDenotation(LabelsHash labelsHash, PowersetSharedPtr denotation);
    const AtomInterpretation* getAtomInterpretation(const spot::formula& atom) const;
};