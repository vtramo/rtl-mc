#include <spdlog/fmt/fmt.h>

#include "RationalPointBuilderVisitor.h"

static bool startsWithMinusSign(const RationalPointParser::ValidIntegerValueContext* valueContext);
static bool startsWithMinusSign(const antlr4::Token* token);

RationalPointBuilderVisitor::RationalPointBuilderVisitor(const SymbolTable& symbolTable)
    : m_visitor { RationalPointVisitor { symbolTable } }
{
}

RationalPointBuilderVisitor::RationalPointVisitor::RationalPointVisitor(const SymbolTable& symbolTable)
    : m_symbolTable { symbolTable }
{
}

const std::vector<ParserError>& RationalPointBuilderVisitor::RationalPointBuilderVisitor::errors() const
{
    return m_visitor.m_errors;
}

bool RationalPointBuilderVisitor::RationalPointBuilderVisitor::hasErrors() const
{
    return !m_visitor.m_errors.empty();
}

RationalPoint RationalPointBuilderVisitor::buildRationalPoint(RationalPointParser::ArrayContext* parseTree,
                                                              const bool requireAllVariables)
{
    m_visitor.visit(parseTree);

    std::vector<std::string> missingVariables {};

    if (requireAllVariables)
    {
        for (const auto& [_, variable]: m_visitor.m_symbolTable.getVariableNameBySpaceDimension())
        {
            if (m_visitor.m_valueByVariable.count(variable) == 0)
            {
                missingVariables.push_back(variable);
            }
        }
    }

    if (!missingVariables.empty())
    {
        m_visitor.addMissingVariablesError(parseTree, std::move(missingVariables));
        return RationalPoint { PPL::point(), {} };
    }

    return computeRationalPoint();
}

RationalPoint RationalPointBuilderVisitor::computeRationalPoint()
{
    if (hasErrors())
    {
        throw std::runtime_error("Cannot compute point with parsing errors");
    }

    std::unordered_map<PPL::dimension_type, Rational> valueByDimension {};
    if (m_visitor.m_valueByVariable.size() == 1)
    {
        const auto & [variableName, fraction] = *m_visitor.m_valueByVariable.begin();
        PPL::Variable variable { *m_visitor.m_symbolTable.getVariable(variableName) };
        valueByDimension.insert({ variable.id(), fraction });
        return RationalPoint { PPL::point(fraction.get_num() * variable, fraction.get_den()), valueByDimension };
    }

    auto variableNameAndFractionIt { m_visitor.m_valueByVariable.begin() };
    const auto& [firstVariableName, firstFraction] = *variableNameAndFractionIt++;
    const PPL::Variable firstVariable { *m_visitor.m_symbolTable.getVariable(firstVariableName) };
    valueByDimension.insert({ firstVariable.id(), firstFraction });
    PPL::Linear_Expression pointLinearExpression { firstFraction.get_num() * firstVariable };
    PPL::GMP_Integer mcd { firstFraction.get_num() };
    PPL::GMP_Integer mcm { firstFraction.get_den() };
    for (; variableNameAndFractionIt != m_visitor.m_valueByVariable.end(); ++variableNameAndFractionIt)
    {
        const auto& [variableName, fraction] = *variableNameAndFractionIt;
        mcm = lcm(mcm, fraction.get_den());
        mcd = gcd(mcd, fraction.get_num());
        const PPL::Variable variable { *m_visitor.m_symbolTable.getVariable(variableName) };
        valueByDimension.insert({ variable.id(), fraction });
        pointLinearExpression += fraction.get_num() * variable;
    }
    const auto mcdFraction { mcd / mcm };
    const auto mcdFractionNumerator { mcdFraction.get_val1() };
    const auto mcdFractionDenominator { mcdFraction.get_val2() };
    return RationalPoint { PPL::point(pointLinearExpression * mcdFractionNumerator, mcdFractionDenominator), valueByDimension };
}

std::any RationalPointBuilderVisitor::RationalPointVisitor::visitValidArray(RationalPointParser::ValidArrayContext* context)
{
    const std::vector pairContexts { context->pair() };
    for (RationalPointParser::PairContext* pairContext: pairContexts)
        visit(pairContext);

    return 0;
}

std::any RationalPointBuilderVisitor::RationalPointVisitor::visitValidPair(RationalPointParser::ValidPairContext* context)
{
    const std::string variable { context->VARID()->getText() };
    const Rational rational { std::any_cast<Rational>(visit(context->value())) };

    if (!symbolTableHasVariable(variable))
        addVariableNotPresentInSymbolTableError(context->VARID());

    if (m_valueByVariable.count(variable) == 1)
        addDuplicateVariableParserError(context->VARID());

    m_valueByVariable[variable] = rational;

    return 0;
}

std::any RationalPointBuilderVisitor::RationalPointVisitor::visitValidIntegerValue(RationalPointParser::ValidIntegerValueContext* context)
{
    static constexpr int denominator { 1 };
    const PPL::GMP_Integer numerator { context->UNSIGNED_INT()->getText() };
    const Rational rational { numerator, denominator };
    return startsWithMinusSign(context) ? -rational : rational;
}

std::any RationalPointBuilderVisitor::RationalPointVisitor::visitValidRationalValue(RationalPointParser::ValidRationalValueContext* context)
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

void RationalPointBuilderVisitor::RationalPointVisitor::addDivisionByZeroError(const RationalPointParser::ValidRationalValueContext* ctx)
{
    antlr4::Token* start { ctx->getStart() };
    antlr4::Token* end { ctx->getStop() };
    Position startPositionError { start->getLine(), start->getCharPositionInLine() };
    Position endPositionError { end->getLine(), end->getCharPositionInLine() };
    std::string errorMessage { "Division by zero!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

bool RationalPointBuilderVisitor::RationalPointVisitor::symbolTableHasVariable(const std::string_view variable) const
{
    return m_symbolTable.containsVariable(variable);
}

void RationalPointBuilderVisitor::RationalPointVisitor::addDuplicateVariableParserError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    Position startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string duplicatedVariable { ctx->getText() };
    Position endPositionError { start->getLine(), start->getCharPositionInLine() + duplicatedVariable.length() };
    std::string errorMessage { "Variable '" + duplicatedVariable + "' already has a value!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

void RationalPointBuilderVisitor::RationalPointVisitor::addVariableNotPresentInSymbolTableError(antlr4::tree::TerminalNode* ctx)
{
    antlr4::Token* start { ctx->getSymbol() };
    Position startPositionError { start->getLine(), start->getCharPositionInLine() };
    std::string variableNotPresent { ctx->getText() };
    Position endPositionError { start->getLine(), start->getCharPositionInLine() + variableNotPresent.length() };
    std::string errorMessage { "Variable '" + variableNotPresent + "' is not defined in the Polyhedral System!" };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

void RationalPointBuilderVisitor::RationalPointVisitor::addMissingVariablesError(
    const RationalPointParser::ArrayContext* parseTree,
    std::vector<std::string>&& missingVariables
) {
    std::string variablesList { antlrcpp::join(missingVariables, ", ") };
    std::string errorMessage { fmt::format(
        "Variables [{}] are not defined in the Polyhedral System!",
        variablesList
    )};
    Position startPositionError { 0, 0 };
    Position endPositionError { parseTree->getStop()->getLine(), parseTree->getStop()->getCharPositionInLine() };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

static bool startsWithMinusSign(const RationalPointParser::ValidIntegerValueContext* valueContext)
{
    return startsWithMinusSign(valueContext->op);
}

static bool startsWithMinusSign(const antlr4::Token* token)
{
    if (!token)
        return false;

    return token->getType() == RationalPointParser::MINUS;
}