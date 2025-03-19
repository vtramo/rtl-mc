#pragma once

#include "PolyhedralSystem.h"
#include "formula.h"
#include "systemparser.h"

inline std::tuple<PolyhedralSystemSharedPtr, spot::formula> nogap(const int k, const int t)
{
    if (k < 0)
    {
        throw std::invalid_argument("k must be non-negative!");
    }

    if (t < 0)
    {
        throw std::invalid_argument("t must be non-negative!");
    }

    PolyhedralSystemSharedPtr polyhedralSystem{
        std::make_shared<PolyhedralSystem>(
            std::move(
                *parsePolyhedralSystem(
                    "Inv ( { a >= 0 & b >= 0 } )"
                    "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                    "p { a > b }"
                    "q { b > a }"
                    "t0 { t = 0 }"
                    "t1 { t <= " + std::to_string(t) + " }"
                )
            )
        )
    };

    spot::formula rtlf{andAtoms({ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q"))})};

    return {std::move(polyhedralSystem), std::move(rtlf)};
}