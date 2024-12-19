//
// Created by vincenzo on 16/12/24.
//

#include "PolyhedralSystemBuilderVisitor.h"

namespace
{
    int extractCoefficient(PolyhedralSystemParser::IntTimesVarContext* context)
    {
        antlr4::tree::TerminalNode* unsignedIntToken { context->UNSIGNED_INT() };
        const std::string unsignedIntTokenStr { unsignedIntToken->getText() };
        return std::stoi(unsignedIntTokenStr);
    }

    bool startsWithMinusSign(const PolyhedralSystemParser::LinearExprContext* linearExprContext)
    {
        const antlr4::Token* token { linearExprContext->op };
        if (!token)
            return false;

        return token->getType() == PolyhedralSystemParser::MINUS;
    }
}


PolyhedralSystemBuilderVisitor::PolyhedralSystemBuilderVisitor(PolyhedralSystemSymbolTable symbolTable)
    : m_visitor { PolyhedralSystemVisitor { std::move(symbolTable) } } {}

PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::PolyhedralSystemVisitor(PolyhedralSystemSymbolTable symbolTable)
    : m_symbolTable { std::move(symbolTable) } {}

PolyhedralSystem PolyhedralSystemBuilderVisitor::buildPolyhedralSystem(PolyhedralSystemParser::SystemContext* parseTree)
{
    m_visitor.visit(parseTree);
    return PolyhedralSystem::builder()
        .flow(m_visitor.getFlow())
        .invariant(m_visitor.getInvariant())
        .denotation(m_visitor.getDenotation())
        .symbolTable(m_visitor.getSymbolTable())
        .build();
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitSystem(PolyhedralSystemParser::SystemContext* ctx)
{
    const std::any flowAny { visit(ctx->flow()) };
    m_flow = std::any_cast<Poly>(flowAny);

    const std::any invAny { visit(ctx->inv()) };
    m_invariant = std::any_cast<Powerset>(invAny);

    const std::vector atomContexts { ctx->atom() };
    for (PolyhedralSystemParser::AtomContext* atomContext: atomContexts)
    {
        visit(atomContext);
    }

    return PolyhedralSystemBaseVisitor::visitSystem(ctx);
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitInv(PolyhedralSystemParser::InvContext* ctx)
{
    const std::any any { visit(ctx->powerset()) };
    return { std::any_cast<Powerset>(any) };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitFlow(PolyhedralSystemParser::FlowContext* ctx)
{
    const std::any any { visit(ctx->poly()) };
    return { std::any_cast<Poly>(any) };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx)
{
    const std::any any { visit(ctx->powerset()) };
    const Powerset powerset { std::any_cast<Powerset>(any) };
    const std::string atomId { ctx->ID()->getText() };
    return m_denotation[atomId] = powerset;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx)
{
    const std::any any { visit(ctx->poly()) };
    const Poly polyhedron { std::any_cast<Poly>(any) };
    const std::string atomId { ctx->ID()->getText() };
    return m_denotation[atomId] = Powerset { polyhedron };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx)
{
    const std::string atomId { ctx->ID()->getText() };
    return m_denotation[atomId] = Powerset { m_symbolTable.getSpaceDimension(), PPL::EMPTY };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPowerset(PolyhedralSystemParser::PowersetContext* ctx)
{
    Powerset powerset { m_symbolTable.getSpaceDimension() };

    const std::vector polyContexts { ctx->poly() };
    for (PolyhedralSystemParser::PolyContext* polyContext: polyContexts)
    {
        const std::any any { visit(polyContext) };
        const Poly polyhedron { std::any_cast<Poly>(any) };
        powerset.add_disjunct(polyhedron);
    }

    return powerset;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPoly(PolyhedralSystemParser::PolyContext* ctx)
{
    PPL::Constraint_System constraintSystem {};
    constraintSystem.set_space_dimension(m_symbolTable.getSpaceDimension());

    const std::vector constraintContexts { ctx->constr() };
    for (PolyhedralSystemParser::ConstrContext* constraintContext: constraintContexts)
    {
        const std::any any { visit(constraintContext) };
        const PPL::Constraint constraint { std::any_cast<PPL::Constraint>(any) };
        constraintSystem.insert(constraint);
    }

    return Poly { constraintSystem };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitConstr(PolyhedralSystemParser::ConstrContext* ctx)
{
    const std::vector linearExpressionContexts { ctx->linearExpr() };
    const std::any leftAny { visit(linearExpressionContexts[0]) };
    const std::any rightAny { visit(linearExpressionContexts[1]) };
    const auto leftLinearExpr { std::any_cast<PPL::Linear_Expression>(leftAny) };
    const auto rightLinearExpr { std::any_cast<PPL::Linear_Expression>(rightAny) };

    Parma_Polyhedra_Library::dimension_type spaceDimensions { m_symbolTable.getSpaceDimension() };
    switch (ctx->op->getType())
    {
    case PolyhedralSystemParser::LE: return PPL::Constraint { leftLinearExpr <= rightLinearExpr, spaceDimensions };
    case PolyhedralSystemParser::LT: return PPL::Constraint { leftLinearExpr <  rightLinearExpr, spaceDimensions };
    case PolyhedralSystemParser::EQ: return PPL::Constraint { leftLinearExpr == rightLinearExpr, spaceDimensions };
    case PolyhedralSystemParser::GE: return PPL::Constraint { leftLinearExpr >= rightLinearExpr, spaceDimensions };
    case PolyhedralSystemParser::GT: return PPL::Constraint { leftLinearExpr >  rightLinearExpr, spaceDimensions };
    default: throw std::runtime_error("PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitConstr::visitConstr: Unknown constraint");
    }
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitLinearExpr(PolyhedralSystemParser::LinearExprContext* ctx)
{
    const std::any term { visit(ctx->term()) };
    const auto linearExpr { std::any_cast<PPL::Linear_Expression>(term) };
    PPL::Linear_Expression signedLinearExpression { startsWithMinusSign(ctx) ? -linearExpr : linearExpr };
    for (PolyhedralSystemParser::SignTermContext* signTerm : ctx->signTerm())
    {
        const std::any any { visit(signTerm) };
        const PPL::Linear_Expression linearExpression { std::any_cast<PPL::Linear_Expression>(any) };
        signedLinearExpression += linearExpression;
    }
    return signedLinearExpression;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitSignTerm(PolyhedralSystemParser::SignTermContext* ctx)
{
    const std::any term { visit(ctx->term()) };
    const auto linearExpr { std::any_cast<PPL::Linear_Expression>(term) };
    const bool isMinusSing { ctx->op->getType() == PolyhedralSystemParser::MINUS };
    return isMinusSing ? -linearExpr : linearExpr;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* context)
{
    const std::string varId { context->ID()->getText() };
    const PPL::Variable variable { *m_symbolTable.getVariable(varId) };
    const int coefficient { extractCoefficient(context) };
    return PPL::Linear_Expression { coefficient * variable };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitVar(PolyhedralSystemParser::VarContext* context)
{
    const std::string varId { context->ID()->getText() };
    const auto variable { *m_symbolTable.getVariable(varId) };
    return PPL::Linear_Expression { variable };
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitInt(PolyhedralSystemParser::IntContext* context)
{
    antlr4::tree::TerminalNode* unsignedIntToken { context->UNSIGNED_INT() };
    const std::string unsignedIntTokenStr { unsignedIntToken->getText() };
    const int coefficient { std::stoi(unsignedIntTokenStr) };
    return PPL::Linear_Expression { coefficient };
}

Poly PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getFlow() const
{
    return m_flow;
}

std::map<std::string, Powerset> PolyhedralSystemBuilderVisitor::
PolyhedralSystemVisitor::getDenotation() const
{
    return m_denotation;
}

Powerset
PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getInvariant() const
{
    return m_invariant;
}

PolyhedralSystemSymbolTable PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getSymbolTable() const
{
    return m_symbolTable;
}
