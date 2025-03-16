#pragma once

#include "ppl_utils.h"

inline PolyUniquePtr cone(const Poly& poly)
{
    const PPL::Generator_System& generatorSystem { poly.generators() };

    PPL::Generator_System cone {};
    for (const PPL::Generator& generator: generatorSystem)
        if (generator.is_ray())
            cone.insert(generator);

    PPL::Generator origin { PPL::point(PPLUtils::zeroPointLinearExpression(poly.space_dimension())) };
    cone.insert(origin);

    return std::make_unique<Poly>(cone);
}