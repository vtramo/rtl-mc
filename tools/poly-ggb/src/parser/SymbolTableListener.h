#pragma once

#include "PolyhedraBaseListener.h"
#include "SimpleSymbolTable.h"
#include "PolyhedraParser.h"

class SymbolTableListener final : public PolyhedraBaseListener
{
public:
    void enterIntTimesVar(PolyhedraParser::IntTimesVarContext* ctx) override;
    void enterVar(PolyhedraParser::VarContext* ctx) override;
    void enterAtomPowerset(PolyhedraParser::AtomPowersetContext* ctx) override;
    void enterAtomEmpty(PolyhedraParser::AtomEmptyContext* ctx) override;
    void enterAtomPoly(PolyhedraParser::AtomPolyContext* ctx) override;

    [[nodiscard]] SimpleSymbolTable symbolTable() const;

private:
    SimpleSymbolTable m_symbolTable {};

    void addVariable(std::string_view varId);
};