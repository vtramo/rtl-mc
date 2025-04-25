#pragma once

#include "ppl_aliases.h"
#include <unordered_map>

using Rational = mpq_class; ///< Type for rational number representation

class RationalPoint
{
public:
    RationalPoint(
        const PPL::Generator& generator,
        const std::unordered_map<PPL::dimension_type, Rational>& coefficients
    )
        : m_generator(generator)
        , m_coefficients(coefficients)
    {
    }

    const PPL::Generator& generator() const { return m_generator; }
    const std::unordered_map<PPL::dimension_type, Rational>& coefficients() const { return m_coefficients; }

private:
    PPL::Generator m_generator {};
    std::unordered_map<PPL::dimension_type, Rational> m_coefficients {};
};