#include "PolyhedralSystemSymbolTableListener.h"
#include "PolyhedralSystemSymbolTable.h"

void PolyhedralSystemSymbolTableListener::enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* ctx)
{
    addVariable(ctx->VARID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterVar(PolyhedralSystemParser::VarContext* ctx)
{
    addVariable(ctx->VARID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx)
{
    addAtom(ctx->VARID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx)
{
    addAtom(ctx->VARID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx)
{
    addAtom(ctx->VARID()->getText());
}

[[nodiscard]] PolyhedralSystemSymbolTable PolyhedralSystemSymbolTableListener::getSymbolTable() const
{
    return m_symbolTable;
}

void PolyhedralSystemSymbolTableListener::addVariable(const std::string_view varId)
{
    if (m_symbolTable.containsVariable(varId))
    {
        return;
    }

    m_symbolTable.addVariable(varId);
}

void PolyhedralSystemSymbolTableListener::addAtom(const std::string_view atomId)
{
    if (m_symbolTable.containsAtom(atomId))
    {
        return;
    }

    m_symbolTable.addAtom(atomId);
}