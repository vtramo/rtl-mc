#pragma once
#include <spot/tl/parse.hh>

/*!
 * \class Position
 * \brief Represents a position in an input, including the line number and character position within the line.
 *
 * The \c Position class encapsulates information about a specific location in an input, such as the line number
 * and the character position within that line. It is commonly used to report errors or other events that occur
 * at a specific location during parsing or processing of an input (e.g., a string, file, or stream).
 *
 * \note This class can be constructed from a \c spot::position object, which is used by the Spot library to represent
 *       positions in an input.
 *
 * \see spot::position
 */
class Position
{
public:

     /*!
     * \brief Returns the line number of the position.
     * \return The line number as a \c std::size_t.
     */
    [[nodiscard]] std::size_t line() const { return m_line; }

     /*!
     * \brief Returns the character position within the line.
     * \return The character position as a \c std::size_t.
     */
    [[nodiscard]] std::size_t charPositionInLine() const { return m_charPositionInLine; }

     /*!
     * \brief Constructs a \c PositionError with the specified line number and character position.
     * \param m_line The line number.
     * \param m_charPositionInLine The character position within the line.
     */
    Position(const std::size_t m_line, const std::size_t m_charPositionInLine)
        : m_charPositionInLine { m_charPositionInLine }
        , m_line { m_line }
    {
    }

     /*!
     * \brief Constructs a \c PositionError from a \c spot::position object.
     * \param position The \c spot::position object containing the line and column information.
     */
    explicit Position(const spot::position& position): Position(position.line, position.column - 1)
    {}

    Position() = default;

private:
    std::size_t m_charPositionInLine {}; ///< The character position within the line.
    std::size_t m_line {}; ///< The line number.
};
