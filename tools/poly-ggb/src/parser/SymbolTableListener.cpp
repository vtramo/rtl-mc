#include "SymbolTableListener.h"

void SymbolTableListener::enterIntTimesVar(PolyhedraParser::IntTimesVarContext* ctx)
{
    addVariable(ctx->VARID()->getText());
}

void SymbolTableListener::enterVar(PolyhedraParser::VarContext* ctx)
{
    addVariable(ctx->VARID()->getText());
}

[[nodiscard]] SymbolTable SymbolTableListener::symbolTable() const
{
    return m_symbolTable;
}

void SymbolTableListener::addVariable(const std::string_view varId)
{
    if (m_symbolTable.containsVariable(varId))
    {
        return;
    }

    m_symbolTable.addVariable(varId);
}

void SymbolTableListener::enterAtomPowerset(PolyhedraParser::AtomPowersetContext* ctx)
{
    PolyhedraBaseListener::enterAtomPowerset(ctx);
}

void SymbolTableListener::enterAtomEmpty(PolyhedraParser::AtomEmptyContext* ctx)
{
    PolyhedraBaseListener::enterAtomEmpty(ctx);
}

void SymbolTableListener::enterAtomPoly(PolyhedraParser::AtomPolyContext* ctx)
{
    PolyhedraBaseListener::enterAtomPoly(ctx);
}
