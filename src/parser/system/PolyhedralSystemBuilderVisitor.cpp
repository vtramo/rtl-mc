#include "PolyhedralSystemBuilderVisitor.h"

#include "spot_utils.h"

using namespace SpotUtils;

namespace
{
    int extractCoefficient(PolyhedralSystemParser::IntTimesVarContext* context);
    bool startsWithMinusSign(const PolyhedralSystemParser::LinearExprContext* linearExprContext);
    bool isNegativeTerm(const PolyhedralSystemParser::SignTermContext* ctx);
}

PolyhedralSystemBuilderVisitor::PolyhedralSystemBuilderVisitor(PolyhedralSystemSymbolTable& symbolTable)
    : m_visitor { PolyhedralSystemVisitor { symbolTable } }
{}

PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::PolyhedralSystemVisitor(PolyhedralSystemSymbolTable& symbolTable)
    : m_symbolTable { symbolTable }
{}

PolyhedralSystem PolyhedralSystemBuilderVisitor::buildPolyhedralSystem(PolyhedralSystemParser::SystemContext* parseTree)
{
    m_visitor.visitSystem(parseTree);
    return PolyhedralSystem::builder()
        .flow(m_visitor.getFlow())
        .invariant(m_visitor.getInvariant())
        .denotation(m_visitor.getDenotation())
        .symbolTable(m_visitor.getSymbolTable())
        .build();
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitSystem(PolyhedralSystemParser::SystemContext* ctx)
{
    const int flowKey { std::any_cast<int>(visit(ctx->flow())) };
    const auto flow { popPoly(flowKey) };
    m_flow.m_swap(*flow);

    auto any = visit(ctx->inv());
    const int invariantKey { std::any_cast<int>(any) };
    const auto invariant { popPowerset(invariantKey) };
    m_invariant.m_swap(*invariant);

    const std::vector atomContexts { ctx->atom() };
    for (PolyhedralSystemParser::AtomContext* atomContext: atomContexts)
    {
        visit(atomContext);
    }

    return 0; // This value does not matter.
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitInv(PolyhedralSystemParser::InvContext* ctx)
{
    int any_cast = std::any_cast<int>(visit(ctx->powerset()));
    return any_cast;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitFlow(PolyhedralSystemParser::FlowContext* ctx)
{
    // The first rule is the right one (ctx->poly()[0])! (flow: FLOW poly). The remaining ones are error alternatives.
    int any_cast = std::any_cast<int>(visit(ctx->poly()[0]));
    return any_cast;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->powerset())) };
    auto powerset { popPowerset(key) };
    const std::string atomId { ctx->VARID()->getText() };
    if (containsAtom(atomId))
        addDuplicateAtomParserError(ctx->VARID());

    m_denotation[ap(atomId)] = *powerset;
    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->poly())) };
    const auto poly { popPoly(key) };
    const std::string atomId { ctx->VARID()->getText() };
    if (containsAtom(atomId))
        addDuplicateAtomParserError(ctx->VARID());
    auto powerset { std::make_unique<Powerset>(*poly) };
    m_denotation[ap(atomId)] = *powerset;
    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx)
{
    const std::string atomId { ctx->VARID()->getText() };
    if (containsAtom(atomId)){}
        addDuplicateAtomParserError(ctx->VARID());

    auto bottomPowerset { std::make_unique<Powerset>(m_symbolTable.get().getSpaceDimension(), PPL::EMPTY) };
    m_denotation[ap(atomId)] = *bottomPowerset;
    m_powersets[m_visitKey] = std::move(bottomPowerset);
    return m_visitKey++;
}

void PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::addDuplicateAtomParserError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    PositionError startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string duplicatedAtom { ctx->getText() };
    PositionError endPositionError { start->getLine(), start->getCharPositionInLine() + duplicatedAtom.length() };
    std::string errorMessage { "The atomic proposition " + duplicatedAtom + " is already defined!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPowersetEmptyOrNotEmpty(PolyhedralSystemParser::PowersetEmptyOrNotEmptyContext* ctx)
{
    auto powerset { std::make_unique<Powerset>(m_symbolTable.get().getSpaceDimension(), PPL::EMPTY) };

    const std::vector polyContexts { ctx->poly() };
    for (PolyhedralSystemParser::PolyContext* polyContext: polyContexts)
    {
        const int key { std::any_cast<int>(visit(polyContext)) };
        const auto polyhedron { popPoly(key) };
        powerset->add_disjunct(*polyhedron);
    }

    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPowersetTrue([[maybe_unused]] PolyhedralSystemParser::PowersetTrueContext* ctx)
{
    auto powerset { std::make_unique<Powerset>(m_symbolTable.get().getSpaceDimension(), PPL::UNIVERSE) };
    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPolyAtLeastOneConstr(PolyhedralSystemParser::PolyAtLeastOneConstrContext* ctx)
{
    PPL::Constraint_System constraintSystem {};
    constraintSystem.set_space_dimension(m_symbolTable.get().getSpaceDimension());

    const std::vector constraintContexts { ctx->constr() };
    for (PolyhedralSystemParser::ConstrContext* constraintContext: constraintContexts)
    {
        const int key { std::any_cast<int>(visit(constraintContext)) };
        const auto constraint { popConstraint(key) };
        constraintSystem.insert(*constraint);
    }

    m_polyhedra[m_visitKey] = std::move(std::make_unique<Poly>(constraintSystem, PPL::Recycle_Input {}));
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitPolyTrue([[maybe_unused]] PolyhedralSystemParser::PolyTrueContext* ctx)
{
    m_polyhedra[m_visitKey] = std::move(std::make_unique<Poly>(PPL::UNIVERSE));
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitConstr(PolyhedralSystemParser::ConstrContext* ctx)
{
    const std::vector linearExpressionContexts { ctx->linearExpr() };
    assert(linearExpressionContexts.size() == 2);

    const int leftKey { std::any_cast<int>(visit(linearExpressionContexts[0])) };
    const int rightKey { std::any_cast<int>(visit(linearExpressionContexts[1])) };

    const auto leftLinearExpr { popLinearExpression(leftKey) };
    const auto rightLinearExpr { popLinearExpression(rightKey) };

    switch (ctx->op->getType())
    {
    case PolyhedralSystemParser::LE:
        m_constraints[m_visitKey] = std::move(std::make_unique<PPL::Constraint>(*leftLinearExpr <= *rightLinearExpr));
        break;
    case PolyhedralSystemParser::LT:
        m_constraints[m_visitKey] = std::move(std::make_unique<PPL::Constraint>(*leftLinearExpr < *rightLinearExpr));
        break;
    case PolyhedralSystemParser::EQ:
        m_constraints[m_visitKey] = std::move(std::make_unique<PPL::Constraint>(*leftLinearExpr == *rightLinearExpr));
        break;
    case PolyhedralSystemParser::GE:
        m_constraints[m_visitKey] = std::move(std::make_unique<PPL::Constraint>(*leftLinearExpr >= *rightLinearExpr));
        break;
    case PolyhedralSystemParser::GT:
        m_constraints[m_visitKey] = std::move(std::make_unique<PPL::Constraint>(*leftLinearExpr > *rightLinearExpr));
        break;
    default:
        throw std::runtime_error("PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitConstr::visitConstr: Unknown constraint");
    }

    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitLinearExpr(PolyhedralSystemParser::LinearExprContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->term())) };
    auto linearExpressionResult { popLinearExpression(key) };

    if (startsWithMinusSign(ctx))
    {
        auto minusLinearExpression { -(*linearExpressionResult) };
        linearExpressionResult->m_swap(minusLinearExpression);
    }

    for (PolyhedralSystemParser::SignTermContext* signTerm : ctx->signTerm())
    {
        const int signTermKey { std::any_cast<int>(visit(signTerm)) };
        const auto linearExpression { popLinearExpression(signTermKey) };
        *linearExpressionResult += *linearExpression;
    }

    m_linearExpressions[m_visitKey] = std::move(linearExpressionResult);
    return std::make_any<int>(m_visitKey++);
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitSignTerm(PolyhedralSystemParser::SignTermContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->term())) };

    auto linearExpression { popLinearExpression(key) };
    if (isNegativeTerm(ctx))
    {
        auto minusLinearExpression { -(*linearExpression) };
        linearExpression->m_swap(minusLinearExpression);
    }

    m_linearExpressions[m_visitKey] = std::move(linearExpression);
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* context)
{
    const std::string varId { context->VARID()->getText() };
    const PPL::Variable variable { *m_symbolTable.get().getVariable(varId) };
    const int coefficient { extractCoefficient(context) };
    auto linearExpression { std::make_unique<PPL::Linear_Expression>(coefficient * variable) };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitVar(PolyhedralSystemParser::VarContext* context)
{
    const std::string varId { context->VARID()->getText() };
    const auto variable { *m_symbolTable.get().getVariable(varId) };
    auto linearExpression { std::make_unique<PPL::Linear_Expression>(variable) };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

std::any PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitInt(PolyhedralSystemParser::IntContext* context)
{
    antlr4::tree::TerminalNode* unsignedIntToken { context->UNSIGNED_INT() };
    const std::string unsignedIntTokenStr { unsignedIntToken->getText() };
    const int coefficient { std::stoi(unsignedIntTokenStr) };
    auto linearExpression { std::make_unique<PPL::Linear_Expression>(coefficient) };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

bool PolyhedralSystemBuilderVisitor::hasErrors() const
{
    return m_visitor.m_errors.size() != 0;
}

const std::vector<ParserError>& PolyhedralSystemBuilderVisitor::errors() const
{
    return m_visitor.m_errors;
}

bool PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::containsAtom(const std::string_view atom) const
{
    return containsAtom(ap(atom));
}

bool PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::containsAtom(const spot::formula& atom) const
{
    return m_denotation.count(atom) == 1;
}

Poly PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getFlow() const
{
    return m_flow;
}

std::unordered_map<spot::formula, Powerset>
PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getDenotation() const
{
    return m_denotation;
}

Powerset PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getInvariant() const
{
    return m_invariant;
}

PolyhedralSystemSymbolTable PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::getSymbolTable() const
{
    return m_symbolTable;
}

std::unique_ptr<PPL::Linear_Expression> PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::popLinearExpression(const int visitKey)
{
    const auto pair { m_linearExpressions.find(visitKey) };
    assert(pair != m_linearExpressions.end());
    auto linearExpression { std::move(pair->second) };
    m_linearExpressions.erase(pair);
    return linearExpression;
}

PolyUniquePtr PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::popPoly(const int visitKey)
{
    const auto pair { m_polyhedra.find(visitKey) };
    assert(pair != m_polyhedra.end());
    auto poly { std::move(pair->second) };
    m_polyhedra.erase(pair);
    return poly;
}

PowersetUniquePtr PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::popPowerset(const int visitKey)
{
    const auto pair { m_powersets.find(visitKey) };
    assert(pair != m_powersets.end());
    auto powerset { std::move(pair->second) };
    m_powersets.erase(pair);
    return powerset;
}

std::unique_ptr<PPL::Constraint> PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::popConstraint(const int visitKey)
{
    const auto pair { m_constraints.find(visitKey) };
    assert(pair != m_constraints.end());
    auto constraint { std::move(pair->second) };
    m_constraints.erase(pair);
    return constraint;
}

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

    bool isNegativeTerm(const PolyhedralSystemParser::SignTermContext* ctx)
    {
        return ctx->op->getType() == PolyhedralSystemParser::MINUS;
    }
}
