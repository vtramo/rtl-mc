//
// Created by vincenzo on 01/01/25.
//

#ifndef LOCATION_H
#define LOCATION_H

class LocationError
{
public:

    [[nodiscard]] std::size_t line() const { return m_line; }
    [[nodiscard]] std::size_t charPositionInLine() const { return m_charPositionInLine; }

    LocationError(const std::size_t m_line, const std::size_t m_char_position_in_line)
        : m_charPositionInLine { m_char_position_in_line }
        , m_line { m_line }
    {

    }

    LocationError() = default;

private:
    std::size_t m_charPositionInLine {};
    std::size_t m_line {};
};

#endif //LOCATION_H
