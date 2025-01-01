#ifndef LOCATION_H
#define LOCATION_H

#include <spot/tl/parse.hh>

class PositionError
{
public:

    [[nodiscard]] std::size_t line() const { return m_line; }
    [[nodiscard]] std::size_t charPositionInLine() const { return m_charPositionInLine; }

    PositionError(const std::size_t m_line, const std::size_t m_charPositionInLine)
        : m_charPositionInLine { m_charPositionInLine }
        , m_line { m_line }
    {
    }

    explicit PositionError(const spot::position& position): PositionError(position.line, position.column)
    {}

    PositionError() = default;

private:
    std::size_t m_charPositionInLine {};
    std::size_t m_line {};
};

#endif //LOCATION_H
