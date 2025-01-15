#pragma once

#include <AtomSet.h>
#include <optional>
#include <spot/tl/formula.hh>

#include "ppl_aliases.h"
#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"

using Atom = std::string;

class PolyhedralSystem;

using PolyhedralSystemSharedPtr = std::shared_ptr<PolyhedralSystem>;

class PolyhedralSystem {
public:
    PolyhedralSystem() = default;

    [[nodiscard]] const Poly& getFlow() const;
    [[nodiscard]] const Poly& getPreFlow() const;
    [[nodiscard]] const Powerset& getInvariant() const;
    [[nodiscard]] const PolyhedralSystemSymbolTable& getSymbolTable() const;
    [[nodiscard]] const spot::atomic_prop_set& getAtoms() const;
    [[nodiscard]] int getTotalAtoms() const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> getInterpretation(std::string_view atom) const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> getInterpretation(const spot::formula& atom) const;
    [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
    void setConstraintOutputMinimized(bool);

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = delete;
    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;
    PolyhedralSystem(PolyhedralSystem&&) noexcept;

    friend bool operator== (const PolyhedralSystem&, const PolyhedralSystem&);
    friend std::istream& operator>> (std::istream&, PolyhedralSystem&);
private:
    Powerset m_invariant {};
    Poly m_flow {};
    Poly m_preFlow {};
    std::unordered_map<Atom, AtomInterpretation> m_denotation {};
    PolyhedralSystemSymbolTable m_symbolTable {};
    bool m_minimizeConstraintsOutput { false };

    PolyhedralSystem(
        const Powerset& invariant,
        const Poly& flow,
        const std::unordered_map<Atom, AtomInterpretation>& denotation,
        const PolyhedralSystemSymbolTable& symbolTable
    );

    PolyhedralSystem(
        Powerset&& invariant,
        Poly&& flow,
        std::unordered_map<Atom, AtomInterpretation>&& denotation,
        PolyhedralSystemSymbolTable&& symbolTable
    );

    friend std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);
    PolyhedralSystem& operator= (PolyhedralSystem&&) noexcept;

    friend class PolyhedralSystemBuilder;

    void computePreFlow();
};

std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);
