//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMBUILDERVISITOR_H
#define POLYHEDRALSYSTEMBUILDERVISITOR_H

#include <PolyhedralSystem.h>
#include <PolyhedralSystemSymbolTable.h>

#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemBaseVisitor.h"
#include "ppl.hh"

class PolyhedralSystemBuilderVisitor final
{
private:
    class PolyhedralSystemVisitor final : public PolyhedralSystemBaseVisitor
    {

    public:
        explicit PolyhedralSystemVisitor(PolyhedralSystemSymbolTable symbolTable);

        std::any visitSystem(PolyhedralSystemParser::SystemContext* ctx) override;
        std::any visitInv(PolyhedralSystemParser::InvContext* ctx) override;
        std::any visitFlow(PolyhedralSystemParser::FlowContext* ctx) override;
        std::any visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx) override;
        std::any visitAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx) override;
        std::any visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx) override;
        std::any visitPowerset(PolyhedralSystemParser::PowersetContext* ctx) override;
        std::any visitPoly(PolyhedralSystemParser::PolyContext* ctx) override;
        std::any visitConstr(PolyhedralSystemParser::ConstrContext* ctx) override;
        std::any visitLinearExpr(PolyhedralSystemParser::LinearExprContext* ctx) override;
        std::any visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* context) override;
        std::any visitSignTerm(PolyhedralSystemParser::SignTermContext* ctx) override;
        std::any visitVar(PolyhedralSystemParser::VarContext* context) override;
        std::any visitInt(PolyhedralSystemParser::IntContext* context) override;

        [[nodiscard]] std::map<std::string, Powerset> getDenotation() const;
        [[nodiscard]] Powerset getInvariant() const;
        [[nodiscard]] Poly getFlow() const;
        [[nodiscard]] PolyhedralSystemSymbolTable getSymbolTable() const;

    private:
        PolyhedralSystemSymbolTable m_symbolTable {};
        std::map<std::string, Powerset> m_denotation {};
        Powerset m_invariant {};
        Poly m_flow {};
    };

    PolyhedralSystemVisitor m_visitor;

public:
    explicit PolyhedralSystemBuilderVisitor(PolyhedralSystemSymbolTable symbolTable);
    PolyhedralSystem buildPolyhedralSystem(PolyhedralSystemParser::SystemContext* parseTree);
};



#endif //POLYHEDRALSYSTEMBUILDERVISITOR_H
