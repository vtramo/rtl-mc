
//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMBUILDER_H
#define POLYHEDRALSYSTEMBUILDER_H

#include "PolyhedralSystemSymbolTable.h"
#include "ppl.hh"
#include "ppl_aliases.h"

class PolyhedralSystem;

class PolyhedralSystemBuilder {
public:
    PolyhedralSystemBuilder& flow(const Poly& flow);
    PolyhedralSystemBuilder& invariant(const Powerset& invariant);
    PolyhedralSystemBuilder& denotation(const std::map<std::string, Powerset>& denotation);
    PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable);
    [[nodiscard]] PolyhedralSystem build() const;

    ~PolyhedralSystemBuilder();

private:
    PolyhedralSystemSymbolTable* m_symbolTable { nullptr };
    std::map<std::string, Powerset>* m_denotation { nullptr };
    Powerset* m_invariant { nullptr };
    Poly* m_flow { nullptr };

    void checkInvariant() const;
    void checkSymbolTable() const;
    void checkFlow() const;
    void checkDenotation() const;

    void checkSpaceDimensions() const;

    [[nodiscard]] PolyhedralSystem buildPolyhedralSystem() const;
};



#endif //POLYHEDRALSYSTEMBUILDER_H
