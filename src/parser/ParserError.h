#pragma once

#include <optional>
#include <string>
#include "Position.h"

/*!
 * \class ParserError
 * \brief Represents an error encountered during parsing, including its type, message, and location.
 *
 * The \c ParserError class encapsulates information about errors that occur during parsing, such as
 * lexical, syntax, or semantic errors. It stores the error message, the starting position of the error,
 * and optionally the ending position. The error type is also stored to distinguish between different
 * kinds of parsing errors.
 *
 * \note This class is designed to work with custom parsers and can be used to standardize error reporting.
 *
 * \see Position
 */
class ParserError {
public:

    /*!
     * \enum Type
     * \brief Represents the type of parsing error.
     */
    enum class Type
    {
      semantic,
      lexical,
      syntax
    };

    /*!
     * \brief Returns the error message.
     * \return A string view containing the error message.
     */
    [[nodiscard]] std::string_view errorMessage() const { return m_errorMessage; }

    /*!
     * \brief Returns the starting position of the error.
     * \return A \ref Position object representing the starting position.
     */
    [[nodiscard]] Position startLocation() const { return m_startPosition; }

    /*!
     * \brief Returns the ending position of the error, if available.
     * \return An optional \ref Position object representing the ending position.
     */
    [[nodiscard]] std::optional<Position> endLocation() const { return m_endPosition; }

    /*!
     * \brief Returns the type of the error.
     * \return The error type as a \c Type enum value.
     */
    [[nodiscard]] Type type() const { return m_type; }

    /*!
     * \brief Constructs a \c ParserError with a starting position, error message, and type.
     * \param startPosition The starting position of the error.
     * \param errorMessage The error message.
     * \param type The type of the error.
     */
    ParserError(
        const Position& startPosition,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { startPosition }
      , m_type { type }
      {
      }

    /*!
     * \brief Constructs a \c ParserError with a line number, character position, error message, and type.
     * \param errorMessage The error message.
     * \param line The line number where the error occurred.
     * \param charPositionInLine The character position in the line where the error occurred.
     * \param type The type of the error.
     */
    ParserError(
        const std::string_view errorMessage,
        const std::size_t line,
        const std::size_t charPositionInLine,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { Position { line, charPositionInLine } }
      , m_type { type }
      {
      }

    /*!
     * \brief Constructs a \c ParserError with a starting position, ending position, error message, and type.
     * \param startPosition The starting position of the error.
     * \param endPosition The ending position of the error.
     * \param errorMessage The error message.
     * \param type The type of the error.
     */
    ParserError(
        const Position& startPosition,
        const Position& endPosition,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { startPosition }
      , m_endPosition { endPosition }
      , m_type { type }
      {
      }

    /*!
     * \brief Constructs a \c ParserError from a \c spot::one_parse_error object.
     * \param spotParserError The \c spot::one_parse_error object containing the error information.
     */
    explicit ParserError(spot::one_parse_error&& spotParserError)
      : ParserError(
          Position { spotParserError.first.begin },
          Position { spotParserError.first.end },
          std::move(spotParserError.second),
          Type::syntax
        )
    {
    }

private:
      std::string m_errorMessage {}; ///< The error message.
      Position m_startPosition {}; ///< The starting position of the error.
      std::optional<Position> m_endPosition {}; ///< The optional ending position of the error.
      Type m_type {}; ///< The type of the error.
};

/*!
 * \brief Outputs a list of \c ParserError objects to a stream.
 * \param os The output stream.
 * \param errors The list of \c ParserError objects to output.
 * \return The output stream.
 */
inline std::ostream& operator<< (std::ostream& os, const std::vector<ParserError>& errors)
{
    for (const auto& parserError : errors)
    {
        auto startPositionError { parserError.startLocation() };
        os << "Error at line " << startPositionError.line()
           << ", position " << startPositionError.charPositionInLine();

        if (parserError.endLocation())
        {
            Position endPositionError { parserError.endLocation().value() };
            os << " to line " << endPositionError.line()
               << ", position " << endPositionError.charPositionInLine();
        }

        os << "\n  Error message: " << parserError.errorMessage() << '\n';
    }

    return os;
}