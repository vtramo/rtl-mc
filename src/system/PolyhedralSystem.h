#pragma once

#include <optional>
#include <spot/tl/formula.hh>

#include "ppl_aliases.h"
#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"

using Atom = std::string;

class PolyhedralSystem;

using PolyhedralSystemSharedPtr = std::shared_ptr<PolyhedralSystem>;
using PolyhedralSystemConstSharedPtr = std::shared_ptr<const PolyhedralSystem>;

class PolyhedralSystem {
public:
    PolyhedralSystem() = default;

    [[nodiscard]] const Poly& flow() const;
    [[nodiscard]] const Poly& preFlow() const;
    [[nodiscard]] bool isOmnidirectionalFlow() const;
    [[nodiscard]] bool isClosedFlow() const;
    [[nodiscard]] bool isMovementForced() const;

    [[nodiscard]] const Powerset& invariant() const;
    [[nodiscard]] bool isBoundedInvariant() const;

    [[nodiscard]] const PolyhedralSystemSymbolTable& symbolTable() const;
    [[nodiscard]] const spot::atomic_prop_set& atoms() const;
    [[nodiscard]] int totalAtoms() const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> interpretation(std::string_view atom) const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> interpretation(const spot::formula& atom) const;
    [[nodiscard]] PPL::dimension_type spaceDimension() const;
    void setConstraintOutputMinimized(bool);

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = delete;
    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;
    PolyhedralSystem(PolyhedralSystem&&) noexcept;

    friend bool operator== (const PolyhedralSystem&, const PolyhedralSystem&);
    friend std::istream& operator>> (std::istream&, PolyhedralSystem&);
private:
    Powerset m_invariant { 0, PPL::EMPTY };
    Poly m_flow { 0, PPL::EMPTY };
    bool m_isOmnidirectionalFlow { false };
    bool m_isMovementForced { false };
    Poly m_preFlow { 0, PPL::EMPTY };
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
    void evaluateFlowProperties();
};

std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);
