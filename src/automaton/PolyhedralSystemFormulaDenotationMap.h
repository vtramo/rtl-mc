#pragma once

#include "PolyhedralSystem.h"
#include "ppl_aliases.h"

class PolyhedralSystemFormulaDenotationMap
{
public:
    explicit PolyhedralSystemFormulaDenotationMap(PolyhedralSystemConstSharedPtr polyhedralSystem);
    PolyhedralSystemFormulaDenotationMap(PolyhedralSystemFormulaDenotationMap&& other) noexcept;
    PolyhedralSystemFormulaDenotationMap(const PolyhedralSystemFormulaDenotationMap& other) = default;

    PowersetConstSharedPtr getOrComputeDenotation(const spot::formula& formula);
    [[nodiscard]] const PolyhedralSystem& getPolyhedralSystem() const;
    [[nodiscard]] bool containsDenotation(const spot::formula& formula) const;

    friend std::ostream& operator<< (std::ostream& out, PolyhedralSystemFormulaDenotationMap& polyhedralSystemFormulaDenotationMap);

private:
    using FormulaToString = std::string;
    using FormulaId = size_t;

    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    std::unordered_map<FormulaId, std::tuple<PowersetConstSharedPtr, FormulaToString>> m_powersetByFormula {};

    friend class BackwardNFA;
    friend class FiniteLtlAutomaton;
    PolyhedralSystemFormulaDenotationMap() = default;

    PowersetConstSharedPtr computeFormulaDenotation(const spot::formula& formula);
    void saveFormulaDenotation(const spot::formula& formula, PowersetConstSharedPtr denotation);
    const AtomInterpretation* getAtomInterpretation(const spot::formula& formula) const;
};