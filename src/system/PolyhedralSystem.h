#pragma once

#include <optional>
#include <spot/tl/formula.hh>

#include "ppl_aliases.h"
#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"
#include "Observable.h"

class PolyhedralSystem;

using PolyhedralSystemSharedPtr = std::shared_ptr<PolyhedralSystem>;
using PolyhedralSystemConstSharedPtr = std::shared_ptr<const PolyhedralSystem>;

class PolyhedralSystem {
public:
    PolyhedralSystem() = default;

    [[nodiscard]] PPL::dimension_type spaceDimension() const;

    [[nodiscard]] const Poly& flow() const;
    [[nodiscard]] const Poly& preFlow() const;
    [[nodiscard]] bool hasOmnidirectionalFlow() const;
    [[nodiscard]] bool hasClosedFlow() const;
    [[nodiscard]] bool isMovementForced() const;

    [[nodiscard]] const Powerset& invariant() const;
    [[nodiscard]] bool hasBoundedInvariant() const;

    [[nodiscard]] const PolyhedralSystemSymbolTable& symbolTable() const;
    [[nodiscard]] const spot::atomic_prop_set& atoms() const;
    [[nodiscard]] bool containsAtom(spot::formula atom) const;
    [[nodiscard]] bool containsAtom(std::string_view atom) const;
    [[nodiscard]] int totalAtoms() const;

    [[nodiscard]] std::optional<const AtomInterpretation* const> getAtomInterpretation(std::string_view atom) const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> getAtomInterpretation(const spot::formula& atom) const;
    const AtomInterpretation& addAtomInterpretation(const spot::formula& atom, const Powerset& interpretation);
    const AtomInterpretation& addAtomInterpretation(std::string_view atom, const Powerset& interpretation);

    [[nodiscard]] spot::bdd_dict_ptr bddDict() const;
    [[nodiscard]] std::vector<Observable> generateObservables(bool filterEmptyObservables = true) const;

    void setConstraintOutputMinimized(bool);

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = default;
    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;
    PolyhedralSystem(PolyhedralSystem&&) noexcept;

    friend bool operator== (const PolyhedralSystem&, const PolyhedralSystem&);
    friend std::istream& operator>> (std::istream&, PolyhedralSystem&);
private:
    Powerset m_invariant { 0, PPL::EMPTY };
    Poly m_flow { 0, PPL::EMPTY };
    bool m_hasOmnidirectionalFlow { false };
    bool m_isMovementForced { false };
    Poly m_preFlow { 0, PPL::EMPTY };
    spot::bdd_dict_ptr m_bddDict {};
    std::unordered_map<spot::formula, AtomInterpretation> m_denotation {};
    PolyhedralSystemSymbolTable m_symbolTable {};
    bool m_minimizeConstraintsOutput { false };

    PolyhedralSystem(
        const Powerset& invariant,
        const Poly& flow,
        const std::unordered_map<spot::formula, AtomInterpretation>& denotation,
        const PolyhedralSystemSymbolTable& symbolTable
    );

    PolyhedralSystem(
        Powerset&& invariant,
        Poly&& flow,
        std::unordered_map<spot::formula, AtomInterpretation>&& denotation,
        PolyhedralSystemSymbolTable&& symbolTable
    );

    friend std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);
    PolyhedralSystem& operator= (PolyhedralSystem&&) noexcept;

    friend class PolyhedralSystemBuilder;

    void makeBddDict();
    void computePreFlow();
    void evaluateFlowProperties();
};

std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);
