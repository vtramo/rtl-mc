#include <catch2/catch_test_macros.hpp>
#include "ppl_utils.h"
#include "closed.h"

using PPL::IO_Operators::operator<<;

TEST_CASE("isClosed function")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable j { 4 };

    SECTION("UNIVERSE is closed")
    {
        Powerset universe { 5 };
        REQUIRE(isClosed(universe));
    }

    SECTION("{{ x >= -2, x <= 2 }} is closed")
    {
        Powerset powerset { PPLUtils::powerset({{ x >= -2, x <= 2 }}) };
        REQUIRE(isClosed(powerset));
    }

    SECTION("{{ x >= -2, x < 2 }} is NOT closed")
    {
        Powerset powerset { PPLUtils::powerset({{ x >= -2, x < 2 }}) };
        REQUIRE(!isClosed(powerset));
    }

    SECTION("{{ x >= -2, x < 2 }, { x >= 2, x <= 4 }} is closed")
    {
        Powerset powerset { PPLUtils::powerset({{ x >= -2, x < 2 }, { x >= 2, x <= 4 }}) };
        REQUIRE(isClosed(powerset));
        REQUIRE(!powerset.is_topologically_closed());
    }

    SECTION("{{ x >= -2, x < 2 }, { x > 2, x <= 4 }} is NOT closed")
    {
        Powerset powerset { PPLUtils::powerset({{ x >= -2, x < 2 }, { x > 2, x <= 4 }}) };
        REQUIRE(!isClosed(powerset));
    }

    SECTION(
        "{\n"
        "\t{ x <= 4, y <= 4, y > 2, 2x - y >= 0 },\n"
        "\t{ -x + 2y >= 0, y <= 2, 2x - y >= 0, x > 0 }\n"
        "}"
    )
    {
        Powerset powerset {
            PPLUtils::powerset(
              {
                  { x <= 4, y <= 4, y < 2, 2*x - y >= 0 },
                  { -x + 2*y >= 0, y <= 2, 2*x - y >= 0, x > 0 }
              }
            )
        };
        REQUIRE(isClosed(powerset));
        REQUIRE(!powerset.is_topologically_closed());
    }
}
