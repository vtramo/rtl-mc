#pragma once

#include "PolyhedraBaseListener.h"
#include "SymbolTable.h"
#include "PolyhedraParser.h"

class SymbolTableListener final : public PolyhedraBaseListener
{
public:
    void enterIntTimesVar(PolyhedraParser::IntTimesVarContext* ctx) override;
    void enterVar(PolyhedraParser::VarContext* ctx) override;
    void enterAtomPowerset(PolyhedraParser::AtomPowersetContext* ctx) override;
    void enterAtomEmpty(PolyhedraParser::AtomEmptyContext* ctx) override;
    void enterAtomPoly(PolyhedraParser::AtomPolyContext* ctx) override;

    [[nodiscard]] SymbolTable symbolTable() const;

private:
    SymbolTable m_symbolTable {};

    void addVariable(std::string_view varId);
};