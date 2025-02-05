#include <spdlog/fmt/fmt.h>
#include "McPointBuilderVisitor.h"

static bool startsWithMinusSign(const McPointParser::ValidIntegerValueContext* valueContext);
static bool startsWithMinusSign(const antlr4::Token* token);

McPointBuilderVisitor::McPointBuilderVisitor(const PolyhedralSystemSymbolTable& symbolTable)
    : m_visitor { McPointVisitor { symbolTable } }
{
}

McPointBuilderVisitor::McPointVisitor::McPointVisitor(const PolyhedralSystemSymbolTable& symbolTable)
    : m_symbolTable { symbolTable }
{
}

const std::vector<ParserError>& McPointBuilderVisitor::McPointBuilderVisitor::errors() const
{
    return m_visitor.m_errors;
}

bool McPointBuilderVisitor::McPointBuilderVisitor::hasErrors() const
{
    return !m_visitor.m_errors.empty();
}

PPL::Generator McPointBuilderVisitor::buildMcPoint(McPointParser::ArrayContext* parseTree)
{
    m_visitor.visit(parseTree);

    std::vector<std::string> missingVariables {};
    for (const auto& variable: m_visitor.m_symbolTable.getVariableNames())
        if (m_visitor.m_valueByVariable.count(variable) == 0)
            missingVariables.push_back(variable);

    if (!missingVariables.empty())
    {
        m_visitor.addMissingVariablesError(parseTree, std::move(missingVariables));
        return PPL::point();
    }

    return computeMcPoint();
}

PPL::Generator McPointBuilderVisitor::computeMcPoint()
{
    if (m_visitor.m_valueByVariable.size() == 1)
    {
        const auto & [variableName, fraction] = *m_visitor.m_valueByVariable.begin();
        PPL::Variable variable { *m_visitor.m_symbolTable.getVariable(variableName) };
        return PPL::point(fraction.get_num() * variable, fraction.get_den());
    }

    auto variableNameAndFractionIt { m_visitor.m_valueByVariable.begin() };
    const auto& [firstVariableName, firstFraction] = *variableNameAndFractionIt++;
    const PPL::Variable firstVariable { *m_visitor.m_symbolTable.getVariable(firstVariableName) };
    PPL::Linear_Expression pointLinearExpression { firstFraction.get_num() * firstVariable };
    PPL::GMP_Integer mcd { firstFraction.get_num() };
    PPL::GMP_Integer mcm { firstFraction.get_den() };
    for (; variableNameAndFractionIt != m_visitor.m_valueByVariable.end(); ++variableNameAndFractionIt)
    {
        const auto& [variableName, fraction] = *variableNameAndFractionIt;
        mcm = lcm(mcm, fraction.get_den());
        mcd = gcd(mcd, fraction.get_num());
        const PPL::Variable variable { *m_visitor.m_symbolTable.getVariable(variableName) };
        pointLinearExpression += fraction.get_num() * variable;
    }
    const auto mcdFraction { mcd / mcm };
    const auto mcdFractionNumerator { mcdFraction.get_val1() };
    const auto mcdFractionDenominator { mcdFraction.get_val2() };
    return PPL::point(pointLinearExpression * mcdFractionNumerator, mcdFractionDenominator);
}

std::any McPointBuilderVisitor::McPointVisitor::visitValidArray(McPointParser::ValidArrayContext* context)
{
    const std::vector pairContexts { context->pair() };
    for (McPointParser::PairContext* pairContext: pairContexts)
        visit(pairContext);

    return 0;
}

std::any McPointBuilderVisitor::McPointVisitor::visitValidPair(McPointParser::ValidPairContext* context)
{
    const std::string variable { context->VARID()->getText() };
    const Rational rational { std::any_cast<Rational>(visit(context->value())) };

    if (!symbolTableHasVariable(variable))
        addVariableNotPresentInPolySystemError(context->VARID());

    if (m_valueByVariable.count(variable) == 1)
        addDuplicateVariableParserError(context->VARID());

    m_valueByVariable[variable] = rational;

    return 0;
}

std::any McPointBuilderVisitor::McPointVisitor::visitValidIntegerValue(McPointParser::ValidIntegerValueContext* context)
{
    static constexpr int denominator { 1 };
    const PPL::GMP_Integer numerator { context->UNSIGNED_INT()->getText() };
    const Rational rational { numerator, denominator };
    return startsWithMinusSign(context) ? -rational : rational;
}

std::any McPointBuilderVisitor::McPointVisitor::visitValidRationalValue(McPointParser::ValidRationalValueContext* context)
{
    const bool numeratorStartsWithMinusSign { startsWithMinusSign(context->opNum) };
    const bool denominatorStartsWithMinusSign { startsWithMinusSign(context->opDen) };
    const bool minus { numeratorStartsWithMinusSign != denominatorStartsWithMinusSign };
    const PPL::GMP_Integer numerator { context->UNSIGNED_INT()[0]->getText() };

    PPL::GMP_Integer denominator { context->UNSIGNED_INT()[1]->getText() };
    if (denominator == 0)
    {
        addDivisionByZeroError(context);
        denominator = 1;
    }

    const Rational rational { numerator, denominator };
    return minus ? -rational : rational;
}

void McPointBuilderVisitor::McPointVisitor::addDivisionByZeroError(const McPointParser::ValidRationalValueContext* ctx)
{
    antlr4::Token* start { ctx->getStart() };
    antlr4::Token* end { ctx->getStop() };
    PositionError startPositionError { start->getLine(), start->getCharPositionInLine() };
    PositionError endPositionError { end->getLine(), end->getCharPositionInLine() };
    std::string errorMessage { "Division by zero!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

bool McPointBuilderVisitor::McPointVisitor::symbolTableHasVariable(const std::string_view variable) const
{
    return m_symbolTable.containsVariable(variable);
}

void McPointBuilderVisitor::McPointVisitor::addDuplicateVariableParserError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    PositionError startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string duplicatedVariable { ctx->getText() };
    PositionError endPositionError { start->getLine(), start->getCharPositionInLine() + duplicatedVariable.length() };
    std::string errorMessage { "Variable '" + duplicatedVariable + "' already has a value!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

void McPointBuilderVisitor::McPointVisitor::addVariableNotPresentInPolySystemError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    PositionError startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string variableNotPresent { ctx->getText() };
    PositionError endPositionError { start->getLine(), start->getCharPositionInLine() + variableNotPresent.length() };
    std::string errorMessage { "Variable '" + variableNotPresent + "' is not defined in the Polyhedral System!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

void McPointBuilderVisitor::McPointVisitor::addMissingVariablesError(
    const McPointParser::ArrayContext* parseTree,
    std::vector<std::string>&& missingVariables
) {
    std::string variablesList { antlrcpp::join(missingVariables, ", ") };
    std::string errorMessage { fmt::format(
        "Variables [{}] are not defined in the Polyhedral System!",
        variablesList
    )};
    PositionError startPositionError { 0, 0 };
    PositionError endPositionError { parseTree->getStop()->getLine(), parseTree->getStop()->getCharPositionInLine() };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

static bool startsWithMinusSign(const McPointParser::ValidIntegerValueContext* valueContext)
{
    return startsWithMinusSign(valueContext->op);
}

static bool startsWithMinusSign(const antlr4::Token* token)
{
    if (!token)
        return false;

    return token->getType() == McPointParser::MINUS;
}