//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEM_H
#define POLYHEDRALSYSTEM_H

#include <optional>

#include "ANTLRInputStream.h"
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
    [[nodiscard]] std::optional<const AtomInterpretation* const> getInterpretation(std::string_view atomId) const;
    [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
    [[nodiscard]] int getTotalAtoms() const;

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = delete;
    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;
    PolyhedralSystem(PolyhedralSystem&&) = delete;

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

PolyhedralSystem buildPolyhedralSystem(std::string_view);
PolyhedralSystem buildPolyhedralSystem(std::istream&);
PolyhedralSystem buildPolyhedralSystem(antlr4::ANTLRInputStream*);
#endif //POLYHEDRALSYSTEM_H
