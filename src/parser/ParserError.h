#ifndef PARSERERROR_H
#define PARSERERROR_H

#include <optional>
#include <string>
#include "PositionError.h"


class ParserError {
public:
    enum class Type
    {
      unknown,
      lexical,
      syntax
    };

    [[nodiscard]] std::string_view errorMessage() const { return m_errorMessage; }
    [[nodiscard]] PositionError startLocation() const { return m_startPosition; }
    [[nodiscard]] std::optional<PositionError> endLocation() const { return m_endPosition; }
    [[nodiscard]] Type type() const { return m_type; }

    ParserError(
        const PositionError& startPosition,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { startPosition }
      , m_type { type }
      {
      }

    ParserError(
        const std::string_view errorMessage,
        const std::size_t line,
        const std::size_t charPositionInLine,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { PositionError { line, charPositionInLine } }
      , m_type { type }
      {
      }

    ParserError(
        const PositionError& startPosition,
        const PositionError& endPosition,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startPosition { startPosition }
      , m_endPosition { endPosition }
      , m_type { type }
      {
      }

    explicit ParserError(spot::one_parse_error&& spotParserError)
      : ParserError(
          PositionError { spotParserError.first.begin },
          PositionError { spotParserError.first.end },
          std::move(spotParserError.second),
          Type::unknown
        )
    {
    }

private:
      std::string m_errorMessage {};
      PositionError m_startPosition {};
      std::optional<PositionError> m_endPosition {};
      Type m_type {};
};

#endif //PARSERERROR_H
