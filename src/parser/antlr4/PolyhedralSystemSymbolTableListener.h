//
// Created by vincenzo on 16/12/24.
//

#ifndef PPLVARSYMBOLTABLELISTENER_H
#define PPLVARSYMBOLTABLELISTENER_H

#include "PolyhedralSystemBaseListener.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemSymbolTable.h"

class PolyhedralSystemSymbolTableListener final : public PolyhedralSystemBaseListener
{
public:
    void enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* ctx) override;
    void enterVar(PolyhedralSystemParser::VarContext* ctx) override;
    void enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx) override;
    void enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx) override;
    void enterAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx) override;

    [[nodiscard]] PolyhedralSystemSymbolTable getSymbolTable() const;

private:
    PolyhedralSystemSymbolTable m_symbolTable {};

    void addVariable(std::string_view varId);
    void addAtom(std::string_view atomId);
};



#endif //PPLVARSYMBOLTABLELISTENER_H
