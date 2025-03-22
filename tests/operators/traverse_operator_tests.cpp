#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "traverse.h"
#include "spot_utils.h"
#include "reflection.h"
#include "Observable.h"
#include "formula.h"

using namespace PPLUtils;
using namespace SpotUtils;

TEST_CASE(
    "Observable1: (\n"
    "   { x >= 1 & x <= 3 & y >= 0 & y <= 2 }\n"
    "   { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
    ")\n"
    "Observable2: (\n"
    "   { x >= 1 & x <= 3 & y > 3 & y <= 4 }\n"
    "   { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
    ")\n"
    "Observable3: (\n"
    "   { x >= 3 & x <= 5 & y > 5 & y <= 6 }\n"
    "   { x >= 4 & x <= 5 & y >= 6 & y < 7 }\n"
    ")\n\n"

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
        "0. Observable1 Patch2 PLUS: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "1. Observable1 Patch2 ZERO: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"

        "2. Observable2 Patch1 PLUS: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "3. Observable2 Patch1 ZERO: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"

        "4. Observable2 Patch2 PLUS: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "5. Observable2 Patch2 ZERO: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"

        "6. Observable3 Patch1 PLUS: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "7. Observable3 Patch1 ZERO: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"

        "8. Observable3 Patch2 PLUS: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n"
        "9. Observable3 Patch2 ZERO: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n\n"
    )
    {
        std::vector observablePatches {
            ObservablePatch  { observable1, observable1Patch2 },
            ObservablePatch  { observable1, observable1Patch2 },

            ObservablePatch  { observable2, observable2Patch1 },
            ObservablePatch  { observable2, observable2Patch1 },

            ObservablePatch  { observable2, observable2Patch2 },
            ObservablePatch  { observable2, observable2Patch2 },

            ObservablePatch  { observable3, observable3Patch1 },
            ObservablePatch  { observable3, observable3Patch1 },

            ObservablePatch  { observable3, observable3Patch2 },
            ObservablePatch  { observable3, observable3Patch2 },
        };

        SECTION("Traverse Zero should return ( { x >= 1 & x < 2 & y = 3 } ) starting from 1.")
        {
            ObservablePatchSequence sequence {
                std::vector(observablePatches.begin() + 1, observablePatches.end()),
                2
            };
            Powerset expectedResult { powerset({{ x >= 1, x < 2, y == 3 }}) };

            PowersetSharedPtr traverseZeroResult { traverseZero(sequence, preFlow) };
            INFO("Traverse Zero Result: " + PPLUtils::toString(*traverseZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traverseZeroResult == expectedResult);
        }

        SECTION("Traverse Plus should return ( { x >= 1 & -x + y > 1 & -y > -3 } )")
        {
            ObservablePatchSequence sequence { observablePatches, 2 };
            Powerset expectedResult { powerset({{ x >= 1, -x + y > 1, -y > -3 }}) };

            PowersetSharedPtr traversePlusResult { traversePlus(sequence, preFlow) };
            INFO("Traverse Plus Result: " + PPLUtils::toString(*traversePlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversePlusResult == expectedResult);
        }
    }

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable1 Patch2 PLUS: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "1. Observable1 Patch2 ZERO: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"

        "2. Observable2 Patch1 PLUS: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "3. Observable2 Patch1 ZERO: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"

        "4. Observable2 Patch2 PLUS: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "5. Observable2 Patch2 ZERO: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"

        "6. Observable3 Patch1 PLUS: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "7. Observable3 Patch1 ZERO: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n\n"
    )
    {
        std::vector observablePatches {
            ObservablePatch  { observable1, observable1Patch2 },
            ObservablePatch  { observable1, observable1Patch2 },

            ObservablePatch  { observable2, observable2Patch1 },
            ObservablePatch  { observable2, observable2Patch1 },

            ObservablePatch  { observable2, observable2Patch2 },
            ObservablePatch  { observable2, observable2Patch2 },

            ObservablePatch  { observable3, observable3Patch1 },
            ObservablePatch  { observable3, observable3Patch1 },
        };

        SECTION("Traverse Zero should return ( { x >= 1 & x <= 2 & y = 3 } ) starting from 1.")
        {
            ObservablePatchSequence sequence {
                std::vector(observablePatches.begin() + 1, observablePatches.end()),
                2
            };
            Powerset expectedResult { powerset({{ x >= 1, x <= 2, y == 3 }}) };

            PowersetSharedPtr traverseZeroResult { traverseZero(sequence, preFlow) };

            INFO("Traverse Zero Result: " + PPLUtils::toString(*traverseZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traverseZeroResult == expectedResult);
        }

        SECTION("Traverse Plus should return ( { x >= 1 & -x + y >= 1 & y < 3 } )")
        {
            ObservablePatchSequence sequence { observablePatches, 2 };
            Powerset expectedResult { powerset({{ x >= 1, -x + y >= 1, y < 3 }}) };

            PowersetSharedPtr traversePlusResult { traversePlus(sequence, preFlow) };
            INFO("Traverse Plus Result: " + PPLUtils::toString(*traversePlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversePlusResult == expectedResult);
        }
    }

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable2 Patch1 PLUS: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "1. Observable2 Patch1 ZERO: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"

        "2. Observable2 Patch2 PLUS: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"
        "3. Observable2 Patch2 ZERO: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"

        "4. Observable3 Patch1 PLUS: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "5. Observable3 Patch1 ZERO: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"

        "6. Observable3 Patch2 PLUS: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n"
        "7. Observable3 Patch2 ZERO: { x >= 4 & x <= 5 & y >= 6 & y <  7 }\n\n"
    )
    {
        std::vector observablePatches {
            ObservablePatch  { observable2, observable2Patch1 },
            ObservablePatch  { observable2, observable2Patch1 },

            ObservablePatch  { observable2, observable2Patch2 },
            ObservablePatch  { observable2, observable2Patch2 },

            ObservablePatch  { observable3, observable3Patch1 },
            ObservablePatch  { observable3, observable3Patch1 },

            ObservablePatch  { observable3, observable3Patch2 },
            ObservablePatch  { observable3, observable3Patch2 },
        };

        SECTION("Traverse Zero should return ( { x >= 2 & x < 3 & y == 4 } )")
        {
            ObservablePatchSequence sequence {
                std::vector(observablePatches.begin() + 1, observablePatches.end()),
                2
            };
            PowersetSharedPtr traverseZeroResult { traverseZero(sequence, preFlow) };

            Powerset expectedResult { powerset({{ x >= 2, x < 3, y == 4 }}) };

            INFO("Traverse Zero Result: " + PPLUtils::toString(*traverseZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traverseZeroResult == expectedResult);
        }

        SECTION("Traverse Plus should return ( { -x + y > 1 & -y > -4 & y > 3 & x - y >= -2 } )")
        {
            ObservablePatchSequence sequence { observablePatches, 2 };
            Powerset expectedResult { powerset({{ -x + y > 1, -y > -4, y > 3, x - y >= -2 }}) };

            PowersetSharedPtr traversePlusResult { traversePlus(sequence, preFlow) };

            INFO("Traverse Plus Result: " + PPLUtils::toString(*traversePlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversePlus(sequence, preFlow) == expectedResult);
        }
    }

    SECTION(
        "ObservablePatchSequence:\n"
        "0. Observable1 Patch2 ZERO: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "1. Observable1 Patch2 PLUS: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"

        "2. Observable2 Patch1 PLUS: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"
        "3. Observable2 Patch1 ZERO: { x >= 1 & x <= 3 & y >  3 & y <= 4 }\n"

        "4. Observable2 Patch2 PLUS: { x >= 1 & x <= 2 & y >= 2 & y <= 3 }\n"
        "5. Observable2 Patch2 ZERO: { x >= 2 & x <= 4 & y >= 4 & y <= 5 }\n"

        "6. Observable3 Patch1 PLUS: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n"
        "7. Observable3 Patch1 ZERO: { x >= 3 & x <= 5 & y >  5 & y <= 6 }\n\n"
    )
    {
        std::vector observablePatches {
            ObservablePatch  { observable1, observable1Patch2 },
            ObservablePatch  { observable2, observable2Patch2 },

            ObservablePatch  { observable2, observable2Patch1 },
            ObservablePatch  { observable2, observable2Patch1 },

            ObservablePatch  { observable1, observable1Patch2 },
            ObservablePatch  { observable2, observable2Patch2 },

            ObservablePatch  { observable3, observable3Patch1 },
            ObservablePatch  { observable3, observable3Patch1 },
        };

        SECTION("Traverse Zero should return FALSE")
        {
            ObservablePatchSequence sequence { observablePatches, 2 };
            Powerset expectedResult { 2, PPL::EMPTY };

            PowersetSharedPtr traverseZeroResult { traverseZero(sequence, preFlow) };

            INFO("Traverse Zero Result: " + PPLUtils::toString(*traverseZeroResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traverseZeroResult == expectedResult);
        }

        SECTION("Traverse Plus should return FALSE starting from 1.")
        {
            ObservablePatchSequence sequence {
                std::vector(observablePatches.begin() + 1, observablePatches.end()),
                2
            };
            Powerset expectedResult { 2, PPL::EMPTY };

            PowersetSharedPtr traversePlusResult { traversePlus(sequence, preFlow) };
            INFO("Traverse Plus Result: " + PPLUtils::toString(*traversePlusResult));
            INFO("Expected Result: " + PPLUtils::toString(expectedResult));
            REQUIRE(*traversePlusResult == expectedResult);
        }
    }
}