#include <catch2/catch_test_macros.hpp>
#include "ppl_utils.h"

using PPL::IO_Operators::operator<<;

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