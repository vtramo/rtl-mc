#include "GeogebraPatchGenerator.h"

#include <formula.h>

namespace
{
    int extractCoefficient(PolyhedraParser::IntTimesVarContext* context);
    bool startsWithMinusSign(const PolyhedraParser::LinearExprContext* linearExprContext);
    bool isNegativeTerm(const PolyhedraParser::SignTermContext* ctx);
    RGBColor toRGBColor(const antlr4::tree::TerminalNode* atomContext);
}

GeogebraPatchGenerator::GeogebraPatchGenerator(SimpleSymbolTable& symbolTable): m_visitor { symbolTable }
{
}

bool GeogebraPatchGenerator::hasErrors() const
{
    return !m_visitor.m_errors.empty();
}

const std::vector<ParserError>& GeogebraPatchGenerator::errors() const
{
    return m_visitor.m_errors;
}

GeogebraPatchGenerator::GeogebraPatchVisitor::GeogebraPatchVisitor(SimpleSymbolTable& symbolTable)
    : m_symbolTable { symbolTable }
{
}

std::string GeogebraPatchGenerator::buildGeogebraPatchLabel(spot::formula ap, const int countPatch)
{
    return ap.ap_name() + "_{" + std::to_string(countPatch) + "}";
}

std::vector<GeogebraPatch> GeogebraPatchGenerator::generateGeogebraPatches(PolyhedraParser::PolyhedraContext* parseTree)
{
    m_visitor.visitPolyhedra(parseTree);

    std::vector<GeogebraPatch> geogebraPatches {};
    for (auto& [atomName, powersetAndColor] : m_visitor.m_denotation)
    {
        auto& [powerset, powersetColor] { powersetAndColor };
        int countPatch { 1 };
        if (powerset.is_empty())
        {
            std::string patchLabel { buildGeogebraPatchLabel(atomName, countPatch) };
            geogebraPatches.push_back(GeogebraPatch { patchLabel, Poly { powerset.space_dimension(), PPL::EMPTY }, powersetColor });
            continue;
        }

        if (powerset.is_universe())
        {
            std::string patchLabel { buildGeogebraPatchLabel(atomName, countPatch) };
            geogebraPatches.push_back(GeogebraPatch { patchLabel, Poly { powerset.space_dimension(), PPL::UNIVERSE }, powersetColor });
            continue;
        }

        for (auto& patch: powerset)
        {
            std::string patchLabel { buildGeogebraPatchLabel(atomName, countPatch) };
            geogebraPatches.push_back(GeogebraPatch { patchLabel, patch.pointset(), powersetColor });
            countPatch++;
        }
    }

    return geogebraPatches;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitPolyhedra(PolyhedraParser::PolyhedraContext* ctx)
{
    for (PolyhedraParser::AtomContext* atomCtx : ctx->atom())
    {
        visit(atomCtx);
    }

    return 0;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitAtomPowerset(PolyhedraParser::AtomPowersetContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->powerset())) };
    Powerset powerset { popPowerset(key) };
    const std::string atomId { ctx->VARID()->getText() };
    if (containsAtom(atomId))
    {
        addDuplicateAtomParserError(ctx->VARID());
    }

    auto hexColorCode { ctx->HEX_COLOR_CODE() };
    RGBColor powersetColor { toRGBColor(hexColorCode) };
    m_denotation[ap(atomId)] = { powerset, powersetColor };
    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitAtomPoly(PolyhedraParser::AtomPolyContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->poly())) };
    const Poly patch { popPatch(key) };
    const std::string atomId { ctx->VARID()->getText() };
    if (containsAtom(atomId))
    {
        addDuplicateAtomParserError(ctx->HEX_COLOR_CODE());
    }

    Powerset powerset { Powerset { patch } };
    auto hexColorCode { ctx->HEX_COLOR_CODE() };
    RGBColor powersetColor { toRGBColor(hexColorCode) };
    m_denotation[ap(atomId)] = { powerset, powersetColor };
    m_powersets[m_visitKey] = powerset;
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitAtomEmpty(PolyhedraParser::AtomEmptyContext* ctx)
{
    const std::string atomId { ctx->HEX_COLOR_CODE()->getText() };
    if (containsAtom(atomId))
    {
        addDuplicateAtomParserError(ctx->HEX_COLOR_CODE());
    }

    Powerset bottomPowerset { m_symbolTable.get().spaceDimension(), PPL::EMPTY };
    auto hexColorCode { ctx->HEX_COLOR_CODE() };
    RGBColor powersetColor { toRGBColor(hexColorCode) };
    m_denotation[ap(atomId)] = { bottomPowerset, powersetColor };
    m_powersets[m_visitKey] = std::move(bottomPowerset);
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext* ctx)
{
    Powerset powerset { m_symbolTable.get().spaceDimension(), PPL::EMPTY };

    const std::vector polyContexts { ctx->poly() };
    for (PolyhedraParser::PolyContext* polyContext: polyContexts)
    {
        const int key { std::any_cast<int>(visit(polyContext)) };
        const auto polyhedron { popPatch(key) };
        powerset.add_disjunct(polyhedron);
    }

    m_powersets[m_visitKey] = powerset;
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitPowersetTrue(PolyhedraParser::PowersetTrueContext* ctx)
{
    Powerset powerset { m_symbolTable.get().spaceDimension(), PPL::UNIVERSE };
    m_powersets[m_visitKey] = std::move(powerset);
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext* ctx)
{
    PPL::Constraint_System constraintSystem {};
    constraintSystem.set_space_dimension(m_symbolTable.get().spaceDimension());

    const std::vector constraintContexts { ctx->constr() };
    for (PolyhedraParser::ConstrContext* constraintContext: constraintContexts)
    {
        const int key { std::any_cast<int>(visit(constraintContext)) };
        const auto constraint { popConstraint(key) };
        constraintSystem.insert(constraint);
    }

    m_patches[m_visitKey] = Poly { constraintSystem };
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitPolyTrue(PolyhedraParser::PolyTrueContext* ctx)
{
    m_patches[m_visitKey] = Poly { m_symbolTable.get().spaceDimension(), PPL::UNIVERSE };
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitConstr(PolyhedraParser::ConstrContext* ctx)
{
    const std::vector linearExpressionContexts { ctx->linearExpr() };
    assert(linearExpressionContexts.size() == 2);

    const int leftKey { std::any_cast<int>(visit(linearExpressionContexts[0])) };
    const int rightKey { std::any_cast<int>(visit(linearExpressionContexts[1])) };

    const auto leftLinearExpr { popLinearExpression(leftKey) };
    const auto rightLinearExpr { popLinearExpression(rightKey) };

    switch (ctx->op->getType())
    {
    case PolyhedraParser::LE:
        m_constraints[m_visitKey] = std::move(leftLinearExpr <= rightLinearExpr);
        break;
    case PolyhedraParser::LT:
        m_constraints[m_visitKey] = std::move(leftLinearExpr < rightLinearExpr);
        break;
    case PolyhedraParser::EQ:
        m_constraints[m_visitKey] = std::move(leftLinearExpr == rightLinearExpr);
        break;
    case PolyhedraParser::GE:
        m_constraints[m_visitKey] = std::move(leftLinearExpr >= rightLinearExpr);
        break;
    case PolyhedraParser::GT:
        m_constraints[m_visitKey] = std::move(leftLinearExpr > rightLinearExpr);
        break;
    default:
        throw std::runtime_error("PolyhedralSystemBuilderVisitor::PolyhedralSystemVisitor::visitConstr::visitConstr: Unknown constraint");
    }

    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitLinearExpr(PolyhedraParser::LinearExprContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->term())) };
    auto linearExpressionResult { popLinearExpression(key) };

    if (startsWithMinusSign(ctx))
    {
        auto minusLinearExpression { -linearExpressionResult };
        linearExpressionResult = minusLinearExpression;
    }

    for (PolyhedraParser::SignTermContext* signTerm : ctx->signTerm())
    {
        const int signTermKey { std::any_cast<int>(visit(signTerm)) };
        const auto linearExpression { popLinearExpression(signTermKey) };
        linearExpressionResult += linearExpression;
    }

    m_linearExpressions[m_visitKey] = std::move(linearExpressionResult);
    return std::make_any<int>(m_visitKey++);
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitIntTimesVar(PolyhedraParser::IntTimesVarContext* context)
{
    const std::string varId { context->VARID()->getText() };
    const PPL::Variable variable { *m_symbolTable.get().getVariable(varId) };
    const int coefficient { extractCoefficient(context) };
    auto linearExpression { coefficient * variable };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitSignTerm(PolyhedraParser::SignTermContext* ctx)
{
    const int key { std::any_cast<int>(visit(ctx->term())) };

    auto linearExpression { popLinearExpression(key) };
    if (isNegativeTerm(ctx))
    {
        auto minusLinearExpression { -linearExpression };
        linearExpression = minusLinearExpression;
    }

    m_linearExpressions[m_visitKey] = std::move(linearExpression);
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitVar(PolyhedraParser::VarContext* context)
{
    const std::string varId { context->VARID()->getText() };
    const auto variable { *m_symbolTable.get().getVariable(varId) };
    auto linearExpression { variable };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

std::any GeogebraPatchGenerator::GeogebraPatchVisitor::visitInt(PolyhedraParser::IntContext* context)
{
    antlr4::tree::TerminalNode* unsignedIntToken { context->UNSIGNED_INT() };
    const std::string unsignedIntTokenStr { unsignedIntToken->getText() };
    const int coefficient { std::stoi(unsignedIntTokenStr) };
    auto linearExpression { coefficient };
    m_linearExpressions.insert(std::make_pair(m_visitKey, std::move(linearExpression)));
    return m_visitKey++;
}

bool GeogebraPatchGenerator::GeogebraPatchVisitor::containsAtom(const spot::formula& atom) const
{
    return m_denotation.count(atom) == 1;
}

bool GeogebraPatchGenerator::GeogebraPatchVisitor::containsAtom(const std::string_view atom) const
{
    return containsAtom(ap(atom));
}

void GeogebraPatchGenerator::GeogebraPatchVisitor::addDuplicateAtomParserError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    Position startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string duplicatedAtom { ctx->getText() };
    Position endPositionError { start->getLine(), start->getCharPositionInLine() + duplicatedAtom.length() };
    std::string errorMessage { "The atomic proposition " + duplicatedAtom + " is already defined!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

Poly GeogebraPatchGenerator::GeogebraPatchVisitor::popPatch(const int visitKey)
{
    const auto pair { m_patches.find(visitKey) };
    assert(pair != m_patches.end());
    auto poly { std::move(pair->second) };
    m_patches.erase(pair);
    return poly;
}

Powerset GeogebraPatchGenerator::GeogebraPatchVisitor::popPowerset(const int visitKey)
{
    const auto pair { m_powersets.find(visitKey) };
    assert(pair != m_powersets.end());
    auto powerset { std::move(pair->second) };
    m_powersets.erase(pair);
    return powerset;
}

PPL::Constraint GeogebraPatchGenerator::GeogebraPatchVisitor::popConstraint(const int visitKey)
{
    const auto pair { m_constraints.find(visitKey) };
    assert(pair != m_constraints.end());
    auto constraint { std::move(pair->second) };
    m_constraints.erase(pair);
    return constraint;
}

PPL::Linear_Expression GeogebraPatchGenerator::GeogebraPatchVisitor::popLinearExpression(const int visitKey)
{
    const auto pair { m_linearExpressions.find(visitKey) };
    assert(pair != m_linearExpressions.end());
    auto linearExpression { std::move(pair->second) };
    m_linearExpressions.erase(pair);
    return linearExpression;
}

namespace
{
    int extractCoefficient(PolyhedraParser::IntTimesVarContext* context)
    {
        antlr4::tree::TerminalNode* unsignedIntToken { context->UNSIGNED_INT() };
        const std::string unsignedIntTokenStr { unsignedIntToken->getText() };
        return std::stoi(unsignedIntTokenStr);
    }

    bool startsWithMinusSign(const PolyhedraParser::LinearExprContext* linearExprContext)
    {
        const antlr4::Token* token { linearExprContext->op };
        if (!token)
        {
            return false;
        }

        return token->getType() == PolyhedraParser::MINUS;
    }

    bool isNegativeTerm(const PolyhedraParser::SignTermContext* ctx)
    {
        return ctx->op->getType() == PolyhedraParser::MINUS;
    }

    RGBColor toRGBColor(const antlr4::tree::TerminalNode* atomContext)
    {
        char* p;
        return atomContext
            ? RGBColor::hex(static_cast<uint32_t>(strtoul(atomContext->getSymbol()->getText().c_str(), &p, 16 )))
            : RGBColor { 0, 0, 255 };
    }
}