#pragma once

#include <vector>
#include <memory>

#include "ParserError.h"
#include "PolyhedralSystem.h"

bool isLexicalError(const ParserError& error);
bool isSyntaxError(const ParserError& error);
bool isUnknownError(const ParserError& error);

class PolyhedralSystemParsingResult {
public:
    [[nodiscard]] bool ok() const { return m_syntaxErrors.empty(); }
    [[nodiscard]] int errorCount() const { return static_cast<int>(m_syntaxErrors.size()); }
    [[nodiscard]] int lexicalErrorCount() const { return m_lexicalErrorCount; }
    [[nodiscard]] int syntaxErrorCount() const { return m_syntaxErrorCount; }
    [[nodiscard]] int unknownErrorCount() const { return m_unknownErrorCount; }

    [[nodiscard]] std::vector<ParserError> errors() const { return m_syntaxErrors; }
    [[nodiscard]] std::vector<ParserError> lexicalErrors() const
    {
        return {
            m_syntaxErrors.begin(),
            m_syntaxErrors.begin() + m_lexicalErrorCount
        };
    }

    [[nodiscard]] std::vector<ParserError> syntaxErrors() const
    {
        return {
            m_syntaxErrors.begin() + m_lexicalErrorCount,
            m_syntaxErrors.begin() + m_lexicalErrorCount + m_syntaxErrorCount
        };
    }

    [[nodiscard]] std::vector<ParserError> unknownErrors() const
    {
        return {
            m_syntaxErrors.begin() + m_lexicalErrorCount + m_syntaxErrorCount,
            m_syntaxErrors.end()
        };
    }

    explicit PolyhedralSystemParsingResult(std::vector<ParserError> syntaxErrors)
        : m_syntaxErrors { std::move(syntaxErrors) }
    {
        const auto itBeginSyntaxErrors { std::partition(m_syntaxErrors.begin(), m_syntaxErrors.end(), isLexicalError) };
        const auto itBeginUnknownErrors { std::partition(itBeginSyntaxErrors, m_syntaxErrors.end(), isSyntaxError) };

        m_lexicalErrorCount = static_cast<int>(std::distance(m_syntaxErrors.begin(), itBeginSyntaxErrors));
        m_syntaxErrorCount = static_cast<int>(std::distance(itBeginSyntaxErrors, itBeginUnknownErrors));
        m_unknownErrorCount = static_cast<int>(std::distance(itBeginUnknownErrors, m_syntaxErrors.end()));
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
    std::vector<ParserError> m_syntaxErrors {};
    std::unique_ptr<PolyhedralSystem> m_polyhedralSystem { nullptr };
    int m_lexicalErrorCount {};
    int m_syntaxErrorCount {};
    int m_unknownErrorCount {};
};

inline bool isLexicalError(const ParserError& error)
{
    return error.type() == ParserError::Type::lexical;
}

inline bool isSyntaxError(const ParserError& error)
{
    return error.type() == ParserError::Type::syntax;
}

inline bool isUnknownError(const ParserError& error)
{
    return error.type() == ParserError::Type::unknown;
}

inline std::ostream& operator<< (std::ostream& os, const PolyhedralSystemParsingResult& result)
{
    if (result.ok())
        return os << *result;

    return os << result.errors();
}