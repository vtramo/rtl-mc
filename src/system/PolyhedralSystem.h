#ifndef POLYHEDRALSYSTEM_H
#define POLYHEDRALSYSTEM_H

#include <optional>
#include <spot/tl/formula.hh>

#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"

using Atom = std::string;

class PolyhedralSystem {
public:
    PolyhedralSystem() = default;

    [[nodiscard]] const Poly& getFlow() const;
    [[nodiscard]] const Poly& getPreFlow() const;
    [[nodiscard]] const Powerset& getInvariant() const;
    [[nodiscard]] const PolyhedralSystemSymbolTable& getSymbolTable() const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> getInterpretation(std::string_view ap) const;
    [[nodiscard]] std::optional<const AtomInterpretation* const> getInterpretation(spot::formula ap) const;
    [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
    [[nodiscard]] int getTotalAtoms() const;

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = delete;
    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;
    PolyhedralSystem(PolyhedralSystem&&) noexcept;

    friend bool operator==(const PolyhedralSystem&, const PolyhedralSystem&);
    friend std::istream& operator>>(std::istream&, PolyhedralSystem&&);
private:
    Powerset m_invariant {};
    Poly m_flow {};
    Poly m_preFlow {};
    std::unordered_map<Atom, AtomInterpretation> m_denotation {};
    PolyhedralSystemSymbolTable m_symbolTable {};

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

    PolyhedralSystem& operator= (PolyhedralSystem&&) noexcept;

    friend class PolyhedralSystemBuilder;

    void computePreFlow();
};

#endif //POLYHEDRALSYSTEM_H
