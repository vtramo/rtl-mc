#pragma once

#include <stdexcept>

template <typename T>
class ClosedInterval
{
public:
    ClosedInterval() = default;

    ClosedInterval(T minimum, T maximum) : m_min { minimum }, m_max { maximum }
    {
        if (minimum > maximum)
        {
            throw std::invalid_argument("minimum > maximum !!!");
        }
    }

    [[nodiscard]] const T& min() const { return m_min; }
    [[nodiscard]] const T& max() const { return m_max; }
    [[nodiscard]] bool contains(T value) const { return (m_min <= value) && (value <= m_max); }
private:
    T m_min {};
    T m_max {};
};