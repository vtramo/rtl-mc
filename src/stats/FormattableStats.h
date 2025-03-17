#pragma once

#include <unordered_map>
#include <string>

class FormattableStats
{
public:
    static const inline std::string s_placeholderPrefix { "%" };
    static const inline std::string s_valueNotAvailable { "N/A" };

    virtual std::string format(const std::string_view s) const
    {
        std::string result { s };
        size_t pos { 0 };

        while ((pos = result.find(s_placeholderPrefix, pos)) != std::string::npos) {
            size_t endPos { result.find(' ', pos) };
            if (endPos == std::string::npos) {
                endPos = result.length();
            }

            std::string placeholder { result.substr(pos, endPos - pos) };
            std::string value { getPlaceholderValue(placeholder) };
            result.replace(pos, endPos - pos, value);

            pos += value.length();
        }

        return result;
    }

    const std::unordered_map<std::string, std::string>& getValueByPlaceholderMap() const { return m_valueByPlaceholder; }

    virtual ~FormattableStats() {}
protected:
    std::unordered_map<std::string, std::string> m_valueByPlaceholder {};

    std::string getPlaceholderValue(const std::string_view placeholder) const
    {
        std::string placeholderStr { placeholder };
        if (m_valueByPlaceholder.count(placeholderStr))
        {
            return m_valueByPlaceholder.at(placeholderStr);
        }

        return s_valueNotAvailable;
    }
};