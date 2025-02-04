#include "ErrorListener.h"

void ErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    [[maybe_unused]] antlr4::Token* offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const std::string &msg,
    [[maybe_unused]] std::exception_ptr e
)
{
    ParserError::Type errorType {};

    if (const antlr4::Lexer* lexer { dynamic_cast<antlr4::Lexer*>(recognizer) }; lexer != nullptr)
    {
        errorType = ParserError::Type::lexical;
    } else if (const antlr4::Parser* parser { dynamic_cast<antlr4::Parser*>(recognizer) }; parser != nullptr)
    {
        errorType = ParserError::Type::syntax;
    } else
    {
        errorType = ParserError::Type::semantic;
    }

    addError({ msg, line, charPositionInLine, errorType });
}

void ErrorListener::addError(ParserError&& error)
{
    m_errors.push_back(std::move(error));
}

bool ErrorListener::hasErrors() const
{
    return !m_errors.empty();
}

std::vector<ParserError> ErrorListener::errors() const
{
    return m_errors;
}
