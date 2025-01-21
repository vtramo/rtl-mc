#pragma once

#include <vector>
#include <memory>

#include "ParserError.h"
#include "PolyhedralSystem.h"

bool isLexicalError(const ParserError& error);
bool isSyntaxError(const ParserError& error);
bool isSemanticError(const ParserError& error);

class PolyhedralSystemParsingResult {
public:
    [[nodiscard]] bool ok() const { return m_errors.empty(); }
    [[nodiscard]] int errorCount() const { return static_cast<int>(m_errors.size()); }
    [[nodiscard]] int lexicalErrorCount() const { return m_lexicalErrorCount; }
    [[nodiscard]] int syntaxErrorCount() const { return m_syntaxErrorCount; }
    [[nodiscard]] int semanticErrorCount() const { return m_semanticErrorCount; }

    [[nodiscard]] std::vector<ParserError> errors() const { return m_errors; }
    [[nodiscard]] std::vector<ParserError> lexicalErrors() const
    {
        return {
            m_errors.begin(),
            m_errors.begin() + m_lexicalErrorCount
        };
    }

    [[nodiscard]] std::vector<ParserError> syntaxErrors() const
    {
        return {
            m_errors.begin() + m_lexicalErrorCount,
            m_errors.begin() + m_lexicalErrorCount + m_syntaxErrorCount
        };
    }

    [[nodiscard]] std::vector<ParserError> semanticErrors() const
    {
        return {
            m_errors.begin() + m_lexicalErrorCount + m_syntaxErrorCount,
            m_errors.end()
        };
    }

    explicit PolyhedralSystemParsingResult(std::vector<ParserError>&& errors)
        : m_errors { std::move(errors) }
    {
        const auto itBeginSyntaxErrors { std::partition(m_errors.begin(), m_errors.end(), isLexicalError) };
        const auto itBeginSemanticErrors { std::partition(itBeginSyntaxErrors, m_errors.end(), isSyntaxError) };

        m_lexicalErrorCount = static_cast<int>(std::distance(m_errors.begin(), itBeginSyntaxErrors));
        m_syntaxErrorCount = static_cast<int>(std::distance(itBeginSyntaxErrors, itBeginSemanticErrors));
        m_semanticErrorCount = static_cast<int>(std::distance(itBeginSemanticErrors, m_errors.end()));
    }

    explicit PolyhedralSystemParsingResult(PolyhedralSystem&& polyhedralSystem)
        : m_polyhedralSystem { std::make_unique<PolyhedralSystem>(std::move(polyhedralSystem)) } {}

    [[nodiscard]] PolyhedralSystem& operator* () const
    {
        if (!m_polyhedralSystem)
        {
            throw std::runtime_error("Parsing failed: PolyhedralSystem is null due to parsing errors.");
        }

        return *m_polyhedralSystem;
    }

    [[nodiscard]] bool operator! () const { return ok(); }

private:
    std::vector<ParserError> m_errors {};
    std::unique_ptr<PolyhedralSystem> m_polyhedralSystem { nullptr };
    int m_lexicalErrorCount {};
    int m_syntaxErrorCount {};
    int m_semanticErrorCount {};
};

inline bool isLexicalError(const ParserError& error)
{
    return error.type() == ParserError::Type::lexical;
}

inline bool isSyntaxError(const ParserError& error)
{
    return error.type() == ParserError::Type::syntax;
}

inline bool isSemanticError(const ParserError& error)
{
    return error.type() == ParserError::Type::semantic;
}

inline std::ostream& operator<< (std::ostream& os, const PolyhedralSystemParsingResult& result)
{
    if (result.ok())
        return os << *result;

    return os << result.errors();
}