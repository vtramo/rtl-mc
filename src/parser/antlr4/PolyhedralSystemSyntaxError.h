//
// Created by vincenzo on 28/12/24.
//

#ifndef POLYHEDRALSYSTEMSYNTAXERROR_H
#define POLYHEDRALSYSTEMSYNTAXERROR_H

#include <string>


class PolyhedralSystemParserError {
public:
      enum class Type
      {
          unknown,
          lexical,
          syntax
      };

      [[nodiscard]] std::string_view errorMessage() const { return m_errorMessage; }
      [[nodiscard]] std::size_t line() const { return m_line; }
      [[nodiscard]] std::size_t charPositionInLine() const { return m_charPositionInLine; }
      [[nodiscard]] Type type() const { return m_type; }

      PolyhedralSystemParserError(
          const std::string_view errorMessage,
          const std::size_t line,
          const std::size_t charPositionInLine,
          const Type type
      ) : m_errorMessage { errorMessage }
        , m_line { line }
        , m_charPositionInLine { charPositionInLine }
        , m_type { type }
      {
      }

private:
      std::string m_errorMessage {};
      std::size_t m_line {};
      std::size_t m_charPositionInLine {};
      Type m_type {};
};

#endif //POLYHEDRALSYSTEMSYNTAXERROR_H
