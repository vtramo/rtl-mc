#include <catch2/catch_test_macros.hpp>
#include "ppl_utils.h"
#include "cone.h"

using PPL::IO_Operators::operator<<;

TEST_CASE("cone function")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable j { 4 };

    SECTION("The cone of { x >= 2, x <= 4, y >= 2, y <= 4 }")
    {
        Poly square { PPLUtils::poly({ x >= 2, x <= 4, y >= 2, y <= 4 }) };
        Poly expectedCone { PPLUtils::poly({ 2*x - y >= 0, -x + 2*y >= 0, -x >= -4, -y >= -4 }) };
        Powerset expectedConePowerset { expectedCone };

        PowersetUniquePtr coneGeometricResult { coneGeometric(square) };
        PolyUniquePtr coneGeneratorResult { coneGenerator(square) };

        INFO("Expected cone " + PPLUtils::toString(expectedCone));
        INFO("Cone geometric" + PPLUtils::toString(*coneGeometricResult));
        INFO("Cone generator" + PPLUtils::toString(*coneGeneratorResult));
        REQUIRE(*coneGeometricResult == expectedConePowerset);
        REQUIRE(*coneGeneratorResult == expectedCone);
    }
}
