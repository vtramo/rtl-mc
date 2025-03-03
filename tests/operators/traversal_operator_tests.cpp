#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "traversal.h"
#include "spot_utils.h"
#include "Observable.h"

using namespace PPLUtils;
using namespace SpotUtils;

TEST_CASE(
    "Observable1: ( { x >= 1 & x <= 3 & y >= 0 & y <= 2 } { x >= 1 & x <= 2 & y >= 2 & y <= 3 } )\n"
    "Observable2: ( { x >= 1 & x <= 3 & y > 3 & y <= 4 } { x >= 2 & x <= 4 & y >= 4 & y <= 5 } )\n"
    "Observable3: ( { x >= 3 & x <= 5 & y > 5 & y <= 6 } { x >= 4 & x <= 5 & y >= 6 & y < 7 } )\n\n"

    "Space dimension: 2\n"
    "Variables: [x, y]\n"
    "Flow: { x = 1 & y = 1 }\n\n"
)
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };

    Poly observable1Patch1 { poly({ x >= 1, x <= 3, y >= 0, y <= 2 }) };
    Poly observable1Patch2 { poly({ x >= 1, x <= 2, y >= 2, y <= 3 }) };
    Powerset observable1Interpretation { 2, PPL::EMPTY };
    observable1Interpretation.add_disjunct(observable1Patch1);
    observable1Interpretation.add_disjunct(observable1Patch2);

    Poly observable2Patch1 { poly({ x >= 1, x <= 3, y >  3, y <= 4 }) };
    Poly observable2Patch2 { poly({ x >= 2, x <= 4, y >= 4, y <= 5 }) };
    Powerset observable2Interpretation { 2, PPL::EMPTY };
    observable2Interpretation.add_disjunct(observable2Patch1);
    observable2Interpretation.add_disjunct(observable2Patch2);

    Poly observable3Patch1 { poly({ x >= 3, x <= 5, y >  5, y <= 6 }) };
    Poly observable3Patch2 { poly({ x >= 4, x <= 5, y >= 6, y <  7 }) };
    Powerset observable3Interpretation { 2, PPL::EMPTY };
    observable3Interpretation.add_disjunct(observable3Patch1);
    observable3Interpretation.add_disjunct(observable3Patch2);

    Observable observable1 { AP({"p"}), observable1Interpretation };
    Observable observable2 { AP({"q"}), observable2Interpretation };
    Observable observable3 { AP({"r"}), observable3Interpretation };

    Poly preFlow { PPLUtils::point(1*x + 1*y) };
    reflectionAffineImage(preFlow);

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable1 Patch2: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "1. Observable2 Patch1: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "2. Observable2 Patch2: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "3. Observable3 Patch1: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "4. Observable3 Patch2: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n\n"
    )
    {
        ObservablePatchSequence sequence {
            std::vector {
                ObservablePatch  { observable1, observable1Patch2 },
                ObservablePatch  { observable2, observable2Patch1 },
                ObservablePatch  { observable2, observable2Patch2 },
                ObservablePatch  { observable3, observable3Patch1 },
                ObservablePatch  { observable3, observable3Patch2 },
            }, 2
        };

        SECTION("Traversal Zero should return ( { x >= 1 & x <= 2 & y = 3 } )")
        {
            PowersetSharedPtr traversalZeroResult { traversalZero(sequence, preFlow) };
            Powerset expectedResult { powerset({{ x >= 1, x <= 2, y == 3 }}) };
            INFO("Traversal Zero Result: " + PPLUtils::toString(*traversalZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalZeroResult == expectedResult);
        }

        SECTION("Traversal Plus should return ( { x >= 1 & -x + y > 1 & -y >= -3 } )")
        {
            PowersetSharedPtr traversalPlusResult { traversalPlus(sequence, preFlow) };
            Powerset expectedResult { powerset({{ x >= 1, -x + y > 1, -y >= -3 }}) };
            INFO("Traversal Plus Result: " + PPLUtils::toString(*traversalPlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalPlusResult == expectedResult);
        }
    }

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable1 Patch2: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "1. Observable2 Patch1: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "2. Observable2 Patch2: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "3. Observable3 Patch1: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n\n"
    )
    {
        ObservablePatchSequence sequence {
            std::vector {
                ObservablePatch  { observable1, observable1Patch2 },
                ObservablePatch  { observable2, observable2Patch1 },
                ObservablePatch  { observable2, observable2Patch2 },
                ObservablePatch  { observable3, observable3Patch1 },
            }, 2
        };

        SECTION("Traversal Zero should return ( { x >= 1 & x <= 2 & y = 3 } )")
        {
            PowersetSharedPtr traversalZeroResult { traversalZero(sequence, preFlow) };
            Powerset expectedResult { powerset({{ x >= 1, x <= 2, y == 3 }}) };
            INFO("Traversal Zero Result: " + PPLUtils::toString(*traversalZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalZeroResult == expectedResult);
        }

        SECTION("Traversal Plus should return ( { x >= 1 & -x + y >= 1 & y <= 3 } )")
        {
            PowersetSharedPtr traversalPlusResult { traversalPlus(sequence, preFlow) };
            Powerset expectedResult { powerset({{ x >= 1, -x + y >= 1, y <= 3 }}) };
            INFO("Traversal Zero Result: " + PPLUtils::toString(*traversalPlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalPlusResult == expectedResult);
        }
    }

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable2 Patch1: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "1. Observable2 Patch2: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "2. Observable3 Patch1: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "3. Observable3 Patch2: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n\n"
    )
    {
        ObservablePatchSequence sequence {
            std::vector {
                ObservablePatch  { observable2, observable2Patch1 },
                ObservablePatch  { observable2, observable2Patch2 },
                ObservablePatch  { observable3, observable3Patch1 },
                ObservablePatch  { observable3, observable3Patch2 },
            }, 2
        };

        SECTION("Traversal Zero should return ( { x >= 2 & x < 3 & y == 4 } )")
        {
            PowersetSharedPtr traversalZeroResult { traversalZero(sequence, preFlow) };
            Powerset expectedResult { powerset({{ x >= 2, x < 3, y == 4 }}) };
            INFO("Traversal Zero Result: " + PPLUtils::toString(*traversalZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalZeroResult == expectedResult);
        }

        SECTION("Traversal Plus should return ( { -x + y >= 1 & -y >= -4 & y > 3 & x - y >= -2 } )")
        {
            PowersetSharedPtr traversalPlusResult { traversalPlus(sequence, preFlow) };
            Powerset expectedResult { powerset({{ -x + y >= 1, -y >= -4, y > 3, x - y >= -2 }}) };
            INFO("Traversal Plus Result: " + PPLUtils::toString(*traversalPlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversalPlus(sequence, preFlow) == expectedResult);
        }
    }
}