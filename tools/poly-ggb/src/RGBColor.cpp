#include "RGBColor.h"

RGBColor::RGBColor()
{
}

RGBColor::RGBColor(const uint8_t red, const uint8_t green, const uint8_t blue)
    : m_r { red }
    , m_g { green }
    , m_b { blue }
{
}

uint8_t RGBColor::red() const
{
    return m_r;
}

uint8_t RGBColor::green() const
{
    return m_g;
}

uint8_t RGBColor::blue() const
{
    return m_b;
}

RGBColor RGBColor::hex(const uint32_t hexValue)
{
    return RGBColor {
        static_cast<uint8_t>((hexValue >> 16) & 0xFF),
        static_cast<uint8_t>((hexValue >> 8) & 0xFF),
        static_cast<uint8_t>(hexValue & 0xFF)
    };
}

bool RGBColor::operator==(const RGBColor& other) const
{
    return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b;
}

bool RGBColor::operator!=(const RGBColor& other) const
{
    return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b;
}

