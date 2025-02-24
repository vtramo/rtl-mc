#include <catch2/catch_test_macros.hpp>
#include "ppl_utils.h"

using PPL::IO_Operators::operator<<;

TEST_CASE("Interior")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable j { 4 };

    SECTION("{ x + y <= z + 2 } SHOULD BE { -x - y + z > -2 }")
    {
        Poly poly { PPLUtils::poly({ x + y <= z + 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > -2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ x + y < z + 2 } SHOULD BE { -x - y + z > -2 }")
    {
        Poly poly { PPLUtils::poly({ x + y < z + 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > -2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ x + y <= z - 2 } SHOULD BE { -x - y + z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + y <= z - 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ x + y < z - 2 } SHOULD BE { -x - y + z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + y < z - 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ x + z < -2 } SHOULD BE { -x - z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z < -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ x + z <= -2 } SHOULD BE { -x - z => 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z <= -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }
    SECTION("{ x + z < -2 } SHOULD BE { -x - z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z < -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ -z - x <= -2 } SHOULD BE { z + x > 2 }")
    {
        Poly poly { PPLUtils::poly({ -z - x <= -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ z + x > 2 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION("{ 32x - 3 + 8z <= 3 + 3k - 10j  } SHOULD BE { -32x + 3k - 10j - 8z > -6  }")
    {
        Poly poly { PPLUtils::poly({ 32*x - 3 + 8*z <= 3 + 3*k - 10*j }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -32*x + 3*k - 10*j - 8*z > -6 }) };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }

    SECTION(
        "{ x + y + z + k >= -2 - j - z & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & j = 1 } "
        "SHOULD BE "
        "{ x + y + 2*z + k + j > -2 & -x - y > -2 & x > -1 & -x > -1 & y > -2 & -y > -2 & j = 1 }"
    )
    {
        Poly poly {
            PPLUtils::poly({
                { x + y + z + k >= -2 - j - z },
                { x + y <= 2 },
                { x >= -1 },
                { x <= 1 },
                { y >= -2 },
                { y <= 2 },
                { j == 1 },
            })
        };
        Poly expectedPolyInterior {
            PPLUtils::poly({
                { x + y + 2*z + k + j > -2 },
                { -x - y > -2 },
                { x > -1 },
                { -x > -1 },
                { y > -2 },
                { -y > -2 },
                { j == 1 },
            })
        };

        PolyUniquePtr polyInterior { PPLUtils::interior(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));
        INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
        REQUIRE(*polyInterior == expectedPolyInterior);
    }
}