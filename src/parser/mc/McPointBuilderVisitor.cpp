#include <spdlog/fmt/fmt.h>
#include "McPointBuilderVisitor.h"

static bool startsWithMinusSign(const McPointParser::ValidValueContext* valueContext);

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
        return PPL::point(PPL::Representation::DENSE);
    }

    PPL::Linear_Expression pointLinearExpression {};
    for (const auto& [variableName, value]: m_visitor.m_valueByVariable)
    {
        PPL::Variable variable { *m_visitor.m_symbolTable.getVariable(variableName) };
        pointLinearExpression += value * variable;
    }

    return PPL::point(pointLinearExpression);
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
    int value { std::any_cast<int>(visit(context->value())) };

    if (!symbolTableHasVariable(variable))
        addVariableNotPresentInPolySystemError(context->VARID());

    if (m_valueByVariable.count(variable) == 1)
        addDuplicateVariableParserError(context->VARID());

    m_valueByVariable[variable] = value;

    return 0;
}

std::any McPointBuilderVisitor::McPointVisitor::visitValidValue(McPointParser::ValidValueContext* context)
{
    const int value { std::stoi(context->UNSIGNED_INT()->getText()) };
    return startsWithMinusSign(context) ? -value : value;
}

bool McPointBuilderVisitor::McPointVisitor::symbolTableHasVariable(std::string_view variable) const
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

void McPointBuilderVisitor::McPointVisitor::addMissingVariablesError(McPointParser::ArrayContext* parseTree, std::vector<std::string>&& missingVariables) {
    std::string variablesList { antlrcpp::join(missingVariables, ", ") };
    std::string errorMessage { fmt::format(
        "Errore: Le variabili [{}] non sono definite nel sistema poliedrale!",
        variablesList
    )};
    PositionError startPositionError { 0, 0 };
    PositionError endPositionError { parseTree->getStop()->getLine(), parseTree->getStop()->getCharPositionInLine() };
    m_errors.emplace_back(startPositionError, endPositionError, errorMessage, ParserError::Type::semantic);
}

static bool startsWithMinusSign(const McPointParser::ValidValueContext* valueContext)
{
    const antlr4::Token* token { valueContext->op };
    if (!token)
        return false;

    return token->getType() == McPointParser::MINUS;
}