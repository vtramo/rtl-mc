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

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ x + y < z + 2 } SHOULD BE { -x - y + z > -2 }")
    {
        Poly poly { PPLUtils::poly({ x + y < z + 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > -2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ x + y <= z - 2 } SHOULD BE { -x - y + z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + y <= z - 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ x + y < z - 2 } SHOULD BE { -x - y + z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + y < z - 2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ - x - y + z > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ x + z < -2 } SHOULD BE { -x - z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z < -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ x + z <= -2 } SHOULD BE { -x - z => 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z <= -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }
    SECTION("{ x + z < -2 } SHOULD BE { -x - z > 2 }")
    {
        Poly poly { PPLUtils::poly({ x + z < -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -x - z > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ -z - x <= -2 } SHOULD BE { z + x > 2 }")
    {
        Poly poly { PPLUtils::poly({ -z - x <= -2 }) };
        Poly expectedPolyInterior { PPLUtils::poly({ z + x > 2 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION("{ 32x - 3 + 8z <= 3 + 3k - 10j  } SHOULD BE { -32x + 3k - 10j - 8z > -6  }")
    {
        Poly poly { PPLUtils::poly({ 32*x - 3 + 8*z <= 3 + 3*k - 10*j }) };
        Poly expectedPolyInterior { PPLUtils::poly({ -32*x + 3*k - 10*j - 8*z > -6 }) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION(
        "{ x + y + z + k >= -2 - j - z & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & j = 1 } "
        "SHOULD BE "
        "false"
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
        Poly expectedPolyInterior { 5, PPL::EMPTY };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }

    SECTION(
        "{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & z = 1 } "
        "SHOULD BE "
        "false"
    )
    {
        Poly poly {
            PPLUtils::poly({
                { x + y >= -2 },
                { x + y <= 2 },
                { x >= -1 },
                { x <= 1 },
                { y >= -2 },
                { y <= 2 },
                { z == 1 },
            }, 3)
        };
        Poly expectedPolyInterior { 3, PPL::EMPTY };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected Poly Interior: " + PPLUtils::toString(expectedPolyInterior));

        SECTION("interior")
        {
            PolyUniquePtr polyInterior { PPLUtils::interior(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }

        SECTION("interiorGeometric")
        {
            PolyUniquePtr polyInterior { PPLUtils::interiorGeometric(poly) };
            INFO("Poly Interior: " + PPLUtils::toString(*polyInterior));
            REQUIRE(*polyInterior == expectedPolyInterior);
        }
    }
}

TEST_CASE("removeSingleVariableZeroEqualityConstraints")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable j { 4 };

    SECTION("{ x = 0 } should be false")
    {
        Poly poly { PPLUtils::poly({ x == 0 }) };
        Poly expectedPoly { poly.space_dimension(), PPL::EMPTY };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 1);
        REQUIRE(*result == expectedPoly);
    }

    SECTION("{ x = 0 & y = 0 & z = 0 } should be false")
    {
        Poly poly { PPLUtils::poly({ x == 0, y == 0, z == 0 }) };
        Poly expectedPoly { poly.space_dimension(), PPL::EMPTY };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 3);
        REQUIRE(*result == expectedPoly);
    }

    SECTION("{ x = 0 } should be false")
    {
        Poly poly { PPLUtils::poly({ x == 0 }) };
        Poly expectedPoly { poly.space_dimension(), PPL::EMPTY };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 1);
        REQUIRE(*result == expectedPoly);
    }

    SECTION("{ x = 0 & y = 0 & z = 0 & k = 0 & j = 0} should be false")
    {
        Poly poly { PPLUtils::poly({ x == 0, y == 0, z == 0, k == 0, j == 0 }) };
        Poly expectedPoly { poly.space_dimension(), PPL::EMPTY };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 5);
        REQUIRE(*result == expectedPoly);
    }

    SECTION(
        "{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & z = 1 }\n"
        "SHOULD BE\n"
        "{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & z = 1 }"
    )
    {
        Poly poly { PPLUtils::poly({ x + y >= -2, x + y <= 2, x >= -1, x <= 1, y >= -2, y <= 2, z == 1 }) };
        Poly expectedPoly { poly };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 3);
        REQUIRE(*result == expectedPoly);
    }


    SECTION(
        "{ x = 0 & y <= 1 & y >= -1 }\n"
        "SHOULD BE\n"
        "{ y <= 1 & y >= -1 }"
    )
    {
        Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1 }) };
        Poly expectedPoly { PPLUtils::poly({ y <= 1, y >= -1 }) };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 2);
        REQUIRE(*result == expectedPoly);
    }

    SECTION(
        "{ x == 0, y <= 1, y >= -1, z == 0 }\n"
        "SHOULD BE\n"
        "{ y <= 1, y >= -1 }"
    )
    {
        Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1, z == 0 }) };
        Poly expectedPoly { PPLUtils::poly({ y <= 1, y >= -1 }, 3) };

        PolyUniquePtr result { PPLUtils::removeSingleVariableZeroEqualityConstraints(poly) };

        INFO("Poly: " + PPLUtils::toString(poly));
        INFO("Expected poly: " + PPLUtils::toString(expectedPoly));
        INFO("Result: " + PPLUtils::toString(*result));
        REQUIRE(result->space_dimension() == 3);
        REQUIRE(*result == expectedPoly);
    }
}

TEST_CASE("isOmnidirectionalFlow")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable j { 4 };

    SECTION("Non-omnidirectional flows")
    {
        SECTION("{ x = 0 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!PPLUtils::isOmnidirectionalFlow(poly));
        }

        SECTION("{ x = 0 & y = 0 & z = 0 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y == 0, z == 0 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!PPLUtils::isOmnidirectionalFlow(poly));
        }

        SECTION("{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & z = 1 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x + y >= -2, x + y <= 2, x >= -1, x <= 1, y >= -2, y <= 2, z == 1 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!PPLUtils::isOmnidirectionalFlow(poly));
        }
    }

    SECTION("Omnidirectional flows")
    {
        SECTION("{ x = 0 & y <= 1 & y >= -1 } is an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(PPLUtils::isOmnidirectionalFlow(poly));
        }

        SECTION("{ x = 0 & y <= 1 & y >= -1 & z = 0 } is an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1, z == 0 }) };
            REQUIRE(poly.space_dimension() == 3);

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(PPLUtils::isOmnidirectionalFlow(poly));
        }
    }
}