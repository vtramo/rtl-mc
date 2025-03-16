#pragma once

#include "interior.h"

inline bool isOmnidirectionalFlow(const Poly& flow)
{
    PolyUniquePtr interiorFlow { interior(flow) };
    Poly zeroPoint { PPLUtils::zeroPoint(flow.space_dimension()) };
    return interiorFlow->contains(zeroPoint);
}