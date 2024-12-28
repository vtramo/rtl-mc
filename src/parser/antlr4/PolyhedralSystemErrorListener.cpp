//
// Created by vincenzo on 28/12/24.
//

#include "PolyhedralSystemErrorListener.h"

void PolyhedralSystemErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    antlr4::Token* offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const std::string &msg,
    std::exception_ptr e
)
{
    PolyhedralSystemParserError::Type errorType {};

    if (const antlr4::Lexer* lexer { dynamic_cast<antlr4::Lexer*>(recognizer) }; lexer != nullptr)
    {
        errorType = PolyhedralSystemParserError::Type::lexical;
    } else if (const antlr4::Parser* parser { dynamic_cast<antlr4::Parser*>(recognizer) }; parser != nullptr)
    {
        errorType = PolyhedralSystemParserError::Type::syntax;
    } else
    {
        errorType = PolyhedralSystemParserError::Type::unknown;
    }

    addError({ msg, line, charPositionInLine, errorType });
}

void PolyhedralSystemErrorListener::addError(PolyhedralSystemParserError&& error)
{
    m_errors.push_back(std::move(error));
}

bool PolyhedralSystemErrorListener::hasErrors() const
{
    return !m_errors.empty();
}

std::vector<PolyhedralSystemParserError> PolyhedralSystemErrorListener::getErrors() const
{
    return m_errors;
}
