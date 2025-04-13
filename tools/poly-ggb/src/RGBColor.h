#pragma once

#include <cstdint>
#include <ostream>

class RGBColor
{
public:
    // static const inline RGBColor BLACK   { 0, 0, 0 };
    // static const inline RGBColor WHITE   { 255, 255, 255 };
    // static const inline RGBColor RED     { 255, 0, 0 };
    // static const inline RGBColor GREEN   { 0, 255, 0 };
    // static const inline RGBColor BLUE    { 0, 0, 255 };
    // static const inline RGBColor YELLOW  { 255, 255, 0 };
    // static const inline RGBColor CYAN    { 0, 255, 255 };
    // static const inline RGBColor MAGENTA { 255, 0, 255 };
    // static const inline RGBColor GRAY    { 128, 128, 128 };
    // static const inline RGBColor ORANGE  { 255, 165, 0 };

    RGBColor();
    RGBColor(uint8_t red, uint8_t green, uint8_t blue);

    [[nodiscard]] uint8_t red() const;
    [[nodiscard]] uint8_t green() const;
    [[nodiscard]] uint8_t blue() const;

    static RGBColor hex(uint32_t hexValue);

    bool operator== (const RGBColor& other) const;
    bool operator!=(const RGBColor& other) const;

private:
    uint8_t m_r {};
    uint8_t m_g {};
    uint8_t m_b {};
};

inline std::ostream& operator<< (std::ostream& out, const RGBColor& color)
{
    out << "RGB[" << static_cast<int>(color.red())
        << ", " << static_cast<int>(color.green())
        << ", " << static_cast<int>(color.blue()) << "]";

    return out;
}