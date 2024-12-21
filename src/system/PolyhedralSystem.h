//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEM_H
#define POLYHEDRALSYSTEM_H

#include <optional>

#include "ANTLRInputStream.h"
#include "ppl.hh"
#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"

PolyhedralSystem buildPolyhedralSystem(std::string_view);
PolyhedralSystem buildPolyhedralSystem(std::istream&);
PolyhedralSystem buildPolyhedralSystem(antlr4::ANTLRInputStream*);
std::istream& operator>>(std::istream& istream, PolyhedralSystem& polyhedralSystem);

class PolyhedralSystem {
public:
    PolyhedralSystem();

    [[nodiscard]] const Poly& getFlow() const;
    [[nodiscard]] const Poly& getPreFlow() const;
    [[nodiscard]] const Powerset& getInvariant() const;
    [[nodiscard]] const PolyhedralSystemSymbolTable& getSymbolTable() const;
    [[nodiscard]] std::optional<const AtomInterpretation*> getInterpretation(std::string_view atomId) const;
    [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
    [[nodiscard]] int getTotalAtoms() const;

    [[nodiscard]] static PolyhedralSystemBuilder builder();

    bool operator==(const PolyhedralSystem& other) const;
    friend std::istream& operator>>(std::istream&, PolyhedralSystem&);

private:
    Powerset m_invariant {};
    Poly m_flow {};
    Poly m_preFlow {};
    std::map<std::string, AtomInterpretation> m_denotation {};
    PolyhedralSystemSymbolTable m_symbolTable {};

    PolyhedralSystem(
        const Powerset& invariant,
        const Poly& flow,
        const std::map<std::string, AtomInterpretation>& denotation,
        PolyhedralSystemSymbolTable& symbolTable
    );

    friend class PolyhedralSystemBuilder;

    void computeReflectedFlow();
};

#endif //POLYHEDRALSYSTEM_H
