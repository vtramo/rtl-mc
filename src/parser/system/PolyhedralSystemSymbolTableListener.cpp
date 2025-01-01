#include "PolyhedralSystemSymbolTableListener.h"
#include "PolyhedralSystemSymbolTable.h"

void PolyhedralSystemSymbolTableListener::enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* ctx)
{
    addVariable(ctx->ID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterVar(PolyhedralSystemParser::VarContext* ctx)
{
    addVariable(ctx->ID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx)
{
    addAtom(ctx->ID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx)
{
    addAtom(ctx->ID()->getText());
}

void PolyhedralSystemSymbolTableListener::enterAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx)
{
    addAtom(ctx->ID()->getText());
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
        // TODO: gestire duplicati
        return;
    }

    m_symbolTable.addAtom(atomId);
}