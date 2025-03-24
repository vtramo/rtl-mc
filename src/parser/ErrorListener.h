#pragma once

#include "antlr4-runtime.h"
#include "ParserError.h"

/*!
 * \class ErrorListener
 * \brief A custom error listener for ANTLR4 that converts parsing errors into a standardized format.
 *
 * This class extends \c antlr4::BaseErrorListener to capture and convert syntax, lexical, and semantic errors
 * encountered during parsing into a collection of \ref ParserError objects. It provides methods to check if errors
 * occurred and to retrieve the list of errors.
 *
 * \see ParserError
 * \see https://www.antlr.org/api/Java/org/antlr/v4/runtime/BaseErrorListener.html
 */
class ErrorListener final: public antlr4::BaseErrorListener {
public:
    /*!
     * \brief Called when a syntax error is encountered during parsing.
     * \param recognizer The recognizer that detected the error (e.g., lexer or parser).
     * \param offendingSymbol The token that caused the error.
     * \param line The line number where the error occurred.
     * \param charPositionInLine The character position in the line where the error occurred.
     * \param msg The error message.
     * \param e An exception pointer (unused in this implementation).
     */
    void syntaxError(
      antlr4::Recognizer *recognizer,
      antlr4::Token* offendingSymbol,
      size_t line,
      size_t charPositionInLine,
      const std::string &msg,
      std::exception_ptr e) override;

    /*!
     * \brief Checks if any errors were encountered during parsing.
     * \return \c true if errors were encountered, \c false otherwise.
     */
    [[nodiscard]] bool hasErrors() const;

    /*!
     * \brief Returns the list of errors encountered during parsing.
     * \return A vector of \ref ParserError objects representing the errors.
     */
    [[nodiscard]] std::vector<ParserError> errors() const;
private:
    std::vector<ParserError> m_errors {}; ///< The list of errors encountered during parsing.

    /*!
     * \brief Adds an error to the list of errors.
     * \param error The \ref ParserError object to add.
     */
    void addError(ParserError&& error);
};