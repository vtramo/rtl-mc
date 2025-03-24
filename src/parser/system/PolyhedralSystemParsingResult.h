#pragma once

#include <vector>
#include <memory>

#include "ParserError.h"
#include "PolyhedralSystem.h"

/*!
 * \brief Checks if a \c ParserError is a lexical error.
 * \param error The \c ParserError to check.
 * \return \c true if the error is a lexical error, \c false otherwise.
 */
bool isLexicalError(const ParserError& error);

/*!
 * \brief Checks if a \c ParserError is a syntax error.
 * \param error The \c ParserError to check.
 * \return \c true if the error is a syntax error, \c false otherwise.
 */
bool isSyntaxError(const ParserError& error);

/*!
 * \brief Checks if a \c ParserError is a semantic error.
 * \param error The \c ParserError to check.
 * \return \c true if the error is a semantic error, \c false otherwise.
 */
bool isSemanticError(const ParserError& error);

/*!
 * \class PolyhedralSystemParsingResult
 * \brief Represents the result of parsing a \c PolyhedralSystem, including any errors encountered.
 *
 * The \c PolyhedralSystemParsingResult class encapsulates the result of parsing a \c PolyhedralSystem,
 * including the parsed system (if successful) and any errors encountered during parsing. Errors are
 * categorized into lexical, syntax, and semantic errors, and can be retrieved individually or as a whole.
 *
 * \note If parsing is successful, the \c PolyhedralSystem can be accessed using the dereference operator (\c *).
 *       If parsing fails, the errors can be retrieved using the \c errors() method.
 *
 * \see ParserError
 * \see PolyhedralSystem
 */
class PolyhedralSystemParsingResult {
public:
    /*!
     * \brief Checks if parsing was successful (i.e., no errors were encountered).
     * \return \c true if parsing was successful, \c false otherwise.
     */
    [[nodiscard]] bool ok() const { return m_errors.empty(); }

    /*!
     * \brief Returns the total number of errors encountered during parsing.
     * \return The number of errors as an integer.
     */
    [[nodiscard]] int errorCount() const { return static_cast<int>(m_errors.size()); }

    /*!
     * \brief Returns the number of lexical errors encountered during parsing.
     * \return The number of lexical errors as an integer.
     */
    [[nodiscard]] int lexicalErrorCount() const { return m_lexicalErrorCount; }

    /*!
     * \brief Returns the number of syntax errors encountered during parsing.
     * \return The number of syntax errors as an integer.
     */
    [[nodiscard]] int syntaxErrorCount() const { return m_syntaxErrorCount; }

    /*!
     * \brief Returns the number of semantic errors encountered during parsing.
     * \return The number of semantic errors as an integer.
     */
    [[nodiscard]] int semanticErrorCount() const { return m_semanticErrorCount; }

    /*!
     * \brief Returns the list of all errors encountered during parsing.
     * \return A vector of \c ParserError objects representing the errors.
     */
    [[nodiscard]] std::vector<ParserError> errors() const { return m_errors; }

    /*!
     * \brief Returns the list of lexical errors encountered during parsing.
     * \return A vector of \c ParserError objects representing the lexical errors.
     */
    [[nodiscard]] std::vector<ParserError> lexicalErrors() const
    {
        return {
            m_errors.begin(),
            m_errors.begin() + m_lexicalErrorCount
        };
    }

    /*!
     * \brief Returns the list of syntax errors encountered during parsing.
     * \return A vector of \c ParserError objects representing the syntax errors.
     */
    [[nodiscard]] std::vector<ParserError> syntaxErrors() const
    {
        return {
            m_errors.begin() + m_lexicalErrorCount,
            m_errors.begin() + m_lexicalErrorCount + m_syntaxErrorCount
        };
    }

    /*!
     * \brief Returns the list of semantic errors encountered during parsing.
     * \return A vector of \c ParserError objects representing the semantic errors.
     */
    [[nodiscard]] std::vector<ParserError> semanticErrors() const
    {
        return {
            m_errors.begin() + m_lexicalErrorCount + m_syntaxErrorCount,
            m_errors.end()
        };
    }

    /*!
     * \brief Constructs a \c PolyhedralSystemParsingResult with a list of parsing errors.
     * \param errors A vector of \c ParserError objects representing the errors encountered during parsing.
     */
    explicit PolyhedralSystemParsingResult(std::vector<ParserError>&& errors)
        : m_errors { std::move(errors) }
    {
        const auto itBeginSyntaxErrors { std::partition(m_errors.begin(), m_errors.end(), isLexicalError) };
        const auto itBeginSemanticErrors { std::partition(itBeginSyntaxErrors, m_errors.end(), isSyntaxError) };

        m_lexicalErrorCount = static_cast<int>(std::distance(m_errors.begin(), itBeginSyntaxErrors));
        m_syntaxErrorCount = static_cast<int>(std::distance(itBeginSyntaxErrors, itBeginSemanticErrors));
        m_semanticErrorCount = static_cast<int>(std::distance(itBeginSemanticErrors, m_errors.end()));
    }

    /*!
     * \brief Constructs a \c PolyhedralSystemParsingResult with a successfully parsed \c PolyhedralSystem.
     * \param polyhedralSystem The successfully parsed \c PolyhedralSystem.
     */
    explicit PolyhedralSystemParsingResult(PolyhedralSystem&& polyhedralSystem)
        : m_polyhedralSystem { std::make_unique<PolyhedralSystem>(std::move(polyhedralSystem)) } {}

    /*!
     * \brief Dereferences the result to access the parsed \c PolyhedralSystem.
     * \return A reference to the parsed \c PolyhedralSystem.
     * \throws std::runtime_error If parsing failed and no \c PolyhedralSystem is available.
     */
    [[nodiscard]] PolyhedralSystem& operator* () const
    {
        if (!m_polyhedralSystem)
        {
            throw std::runtime_error("Parsing failed: PolyhedralSystem is null due to parsing errors.");
        }

        return *m_polyhedralSystem;
    }

    /*!
     * \brief Checks if parsing failed (i.e., errors were encountered).
     * \return \c true if parsing failed, \c false otherwise.
     */
    [[nodiscard]] bool operator! () const { return ok(); }

private:
    std::vector<ParserError> m_errors {}; ///< The list of errors encountered during parsing.
    std::unique_ptr<PolyhedralSystem> m_polyhedralSystem { nullptr }; ///< The parsed \c PolyhedralSystem (if successful).
    int m_lexicalErrorCount {}; ///< The number of lexical errors.
    int m_syntaxErrorCount {}; ///< The number of syntax errors.
    int m_semanticErrorCount {}; ///< The number of semantic errors.
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