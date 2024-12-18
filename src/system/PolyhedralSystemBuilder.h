
//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMBUILDER_H
#define POLYHEDRALSYSTEMBUILDER_H

#include "PolyhedralSystemSymbolTable.h"
#include "ppl.hh"

class PolyhedralSystem;

namespace PPL = Parma_Polyhedra_Library;

class PolyhedralSystemBuilder {
public:
    PolyhedralSystemBuilder& flow(const PPL::NNC_Polyhedron& flow);
    PolyhedralSystemBuilder& invariant(const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant);
    PolyhedralSystemBuilder& denotation(const std::map<std::string, PPL::Pointset_Powerset<PPL::NNC_Polyhedron>>& denotation);
    PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable);
    [[nodiscard]] PolyhedralSystem build() const;

private:
    PolyhedralSystemSymbolTable* m_symbolTable { nullptr };
    std::map<std::string, PPL::Pointset_Powerset<PPL::NNC_Polyhedron>>* m_denotation { nullptr };
    PPL::Pointset_Powerset<PPL::NNC_Polyhedron>* m_invariant { nullptr };
    PPL::NNC_Polyhedron* m_flow { nullptr };

    void checkInvariant() const;
    void checkSymbolTable() const;
    void checkFlow() const;
    void checkDenotation() const;

    void checkSpaceDimensions() const;

    [[nodiscard]] PolyhedralSystem buildPolyhedralSystem() const;
};



#endif //POLYHEDRALSYSTEMBUILDER_H
