#include <catch2/catch_test_macros.hpp>
#include "ppl_utils.h"
#include "omnidirectional.h"

using PPL::IO_Operators::operator<<;

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
            REQUIRE(!isOmnidirectionalFlow(poly));
        }

        SECTION("{ x = 0 & y = 0 & z = 0 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y == 0, z == 0 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!isOmnidirectionalFlow(poly));
        }

        SECTION("{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 & z = 1 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x + y >= -2, x + y <= 2, x >= -1, x <= 1, y >= -2, y <= 2, z == 1 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!isOmnidirectionalFlow(poly));
        }

        SECTION("{ x = 0 & y <= 1 & y >= -1 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!isOmnidirectionalFlow(poly));
        }

        SECTION("{ x = 0 & y <= 1 & y >= -1 & z = 0 } is NOT an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x == 0, y <= 1, y >= -1, z == 0 }) };
            REQUIRE(poly.space_dimension() == 3);

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(!isOmnidirectionalFlow(poly));
        }
    }

    SECTION("Omnidirectional flows")
    {
        SECTION("{ x <= 1 & x >= -1 & y <= 1 & y >= -1 } is an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x <= 1, x >= -1, y <= 1, y >= -1 }) };

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(isOmnidirectionalFlow(poly));
        }

        SECTION("{ x <= 1 & x >= -1 & y <= 1 & y >= -1 & z <= 1 & z >= -1 } is an omnidirectional flow")
        {
            Poly poly { PPLUtils::poly({ x <= 1, x >= -1, y <= 1, y >= -1, z <= 1, z >= -1 }) };
            REQUIRE(poly.space_dimension() == 3);

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(isOmnidirectionalFlow(poly));
        }

        SECTION("{ x + y >= -2 & x + y <= 2 & x >= -1 & x <= 1 & y >= -2 & y <= 2 }")
        {
            Poly poly { PPLUtils::poly({ x + y >= -2, x + y <= 2, x >= -1, x <= 1, y >= -2, y <= 2 }) };
            REQUIRE(poly.space_dimension() == 2);

            INFO("Poly: " + PPLUtils::toString(poly));
            REQUIRE(isOmnidirectionalFlow(poly));
        }
    }
}