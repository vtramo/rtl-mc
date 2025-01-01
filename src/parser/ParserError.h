#ifndef PARSERERROR_H
#define PARSERERROR_H

#include <optional>
#include <string>
#include "LocationError.h"


class ParserError {
public:
    enum class Type
    {
      unknown,
      lexical,
      syntax
    };

    [[nodiscard]] std::string_view errorMessage() const { return m_errorMessage; }
    [[nodiscard]] LocationError startLocation() const { return m_startLocation; }
    [[nodiscard]] std::optional<LocationError> endLocation() const { return m_endLocation; }
    [[nodiscard]] Type type() const { return m_type; }

    ParserError(
        const LocationError& startLocation,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startLocation { startLocation }
      , m_type { type }
      {
          
      }

    ParserError(
        const std::string_view errorMessage,
        const std::size_t line,
        const std::size_t charPositionInLine,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startLocation { LocationError { line, charPositionInLine } }
      , m_type { type }
      {
          
      }

    ParserError(
        const LocationError& startLocation,
        const LocationError& endLocation,
        const std::string_view errorMessage,
        const Type type
    ) : m_errorMessage { errorMessage }
      , m_startLocation { startLocation }
      , m_endLocation { endLocation }
      , m_type { type }
      {
          
      }

private:
      std::string m_errorMessage {};
      LocationError m_startLocation {};
      std::optional<LocationError> m_endLocation {};
      Type m_type {};
};

#endif //PARSERERROR_H
