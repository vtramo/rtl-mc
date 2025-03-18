#include <catch2/catch_test_macros.hpp>
#include "ppl_output.h"
#include "PolyhedralSystemParsingResult.h"
#include "ppl_utils.h"
#include "spot_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "formula.h"

using namespace SpotUtils;
using namespace PPLUtils;

TEST_CASE("Correctly parse PolyhedralSystem", "[good]")
{
    SECTION("Building PolyhedralSystem with 'Flow' constraint first")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Flow { X <= 4 }"
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
               "q { X > 3 & Y >= 4 }"
           )
       };

        REQUIRE(polyhedralSystemParsingResult.ok());

        const PolyhedralSystem& polyhedralSystem { *polyhedralSystemParsingResult };
        REQUIRE(polyhedralSystem.spaceDimension() == 2);
        REQUIRE(polyhedralSystem.totalAtoms() == 2);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Building PolyhedralSystem with 'Inv' constraint first")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "Flow { X <= 4 }"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
               "q { X > 3 & Y >= 4 }"
           )
       };

        REQUIRE(polyhedralSystemParsingResult.ok());

        const PolyhedralSystem& polyhedralSystem { *polyhedralSystemParsingResult };
        REQUIRE(polyhedralSystem.spaceDimension() == 2);
        REQUIRE(polyhedralSystem.totalAtoms() == 2);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Building PolyhedralSystem using '>>' operator with input stream")
    {
        std::istringstream input {
            "Flow { X <= 4 }"
            "Inv ( { X + Y >= 3 & Y >= 4 } )"
            "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "q { X > 3 & Y >= 4 }"
        };
        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 2);
        REQUIRE(polyhedralSystem.totalAtoms() == 2);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Building PolyhedralSystem with 'Flow' last in input stream")
    {
        std::istringstream input {
            "x ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "Inv ( { X + Y >= 3 & Y >= 4 } )"
            "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "q { X > 3 & Y >= 4 }"
            "Flow { X <= 4 }"
        };
        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 2);
        REQUIRE(polyhedralSystem.totalAtoms() == 3);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Building PolyhedralSystem with multiple constraints and atoms (3 dimensions & 4 atoms)")
    {
        std::istringstream input {
            "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "q ( { X < 3 & Y <= 3 } { X > 10 & Y <= 2 } { Y <= 10 } { X = 3 } )"
            "r { X > 3 & Y >= 4 }"
            "v { X < 1 & Z < 1 }"
            "Inv ( { X + Y >= 3 & Y >= 4 } )"
            "Flow { X <= 4 }"
        };
        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 3);
        REQUIRE(polyhedralSystem.totalAtoms() == 4);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Flow not closed, not omnidirectional, and forced movement")
    {
        std::istringstream input {
            "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "q ( { X < 3 & Y <= 3 } { X > 10 & Y <= 2 } { Y <= 10 } { X = 3 } )"
            "r { X > 3 & Y >= 4 }"
            "v { X < 1 & Z < 1 }"
            "Inv ( { X + Y >= 3 & Y >= 4 } )"
            "Flow { X < -4 }"
        };
        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 3);
        REQUIRE(polyhedralSystem.totalAtoms() == 4);
        REQUIRE(!polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(!polyhedralSystem.hasClosedFlow());
    }

    SECTION("Is bounded invariant")
    {
        std::istringstream input {
            "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
            "q ( { X < 3 & Y <= 3 } { X > 10 & Y <= 2 } { Y <= 10 } { X = 3 } )"
            "r { X > 3 & Y >= 4 }"
            "v { X < 1 & Z < 1 }"
            "Inv ( { X >= 4 & X <= 8 & Y <= 4 & Y >= 0 & Z <= 4 & Z > 0} )"
            "Flow { X < -4 }"
        };
        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 3);
        REQUIRE(polyhedralSystem.totalAtoms() == 4);
        REQUIRE(!polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(polyhedralSystem.isMovementForced());
        REQUIRE(polyhedralSystem.hasBoundedInvariant());
        REQUIRE(!polyhedralSystem.hasClosedFlow());
    }

    SECTION("Building a complex PolyhedralSystem with multiple constraints and atoms (6 dimensions & 11 atoms)")
    {
        PolyhedralSystem polyhedralSystem {};
        std::istringstream input {
            "Inv ( { X + Y + Z >= 3 & Y - Z >= 4 } )"
            "Flow { X + Z <= 4 }"
            "p ( { X + 2Z < 3 & 3Y <= 3 } { X < 3 & Y <= 10 } )"
            "q ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )"
            "r { 4X > 3 & 4Y >= 4 }"
            "v { 4X < 1 & 4Z < 1 }"
            "z ( { 10X < 3 & 40Y <= 3 } { 8X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )"
            "x ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )"
            "u ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )"
            "p1 ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )"
            "p2 { 4K > 3 & 4Y >= 4 }"
            "p3 { 4L < 1 & 4Z < 1 }"
            "u1 ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2M <= 10 } { 3X = 3 } )"
        };
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 6);
        REQUIRE(polyhedralSystem.totalAtoms() == 11);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Parse a PolyhedralSystem spec then compare with the expected PolyhedralSystem")
    {
        std::istringstream input {
            "Flow { X <= 4 }"
            "Inv ( { 1*X + Y >= 3 & Y >= 4 } )"
            "p ( { X < 3 & 1Y <= 3 } { X < 3 & 1*Y <= 10 } )"
            "q { 1X > 3 & Y >= 4 }"
        };


        PolyhedralSystemSymbolTable symbolTable {};
        symbolTable
            .addVariables({ "X", "Y" })
            .addAtoms({ "p", "q" });

        PPL::Variable x { *symbolTable.getVariable("X") };
        PPL::Variable y { *symbolTable.getVariable("Y") };
        PolyhedralSystem expectedPolyhedralSystem {
            PolyhedralSystem::builder()
                .flow(poly({ x + 0*y <= 4 }))
                .denotation({
                    { ap("p"), powerset({{ x < 3, y <= 3 }, { x < 3, y <= 10 }}) },
                    { ap("q"), powerset({{ x > 3, y >= 4 }})}
                })
                .invariant(powerset({{ x + y >= 3, y >= 4 }}))
                .symbolTable(symbolTable)
                .build()
        };


        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(expectedPolyhedralSystem.totalAtoms() == polyhedralSystem.totalAtoms());
        REQUIRE(expectedPolyhedralSystem.spaceDimension() == polyhedralSystem.spaceDimension());
        REQUIRE(expectedPolyhedralSystem == polyhedralSystem);

        REQUIRE(expectedPolyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!expectedPolyhedralSystem.isMovementForced());
        REQUIRE(!expectedPolyhedralSystem.hasBoundedInvariant());
        REQUIRE(expectedPolyhedralSystem.hasClosedFlow());

        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Read a PolyhedralSystem from a file")
    {
        std::ifstream input;
        std::string testCaseFilePath { "polyhedral-system-test-1.txt" };
        input.open(testCaseFilePath);

        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(polyhedralSystem.spaceDimension() == 6);
        REQUIRE(polyhedralSystem.totalAtoms() == 11);
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());
    }

    SECTION("Parse a PolyhedralSystem spec then add new atomic proposition (stay)")
    {
        std::istringstream input {
            "Flow { X <= 4 }"
            "Inv ( { 1*X + Y >= 3 & Y >= 4 } )"
            "p ( { X < 3 & 1Y <= 3 } { X < 3 & 1*Y <= 10 } )"
            "q { 1X > 3 & Y >= 4 }"
        };


        PolyhedralSystemSymbolTable symbolTable {};
        symbolTable
            .addVariables({ "X", "Y" })
            .addAtoms({ "p", "q" });

        PPL::Variable x { *symbolTable.getVariable("X") };
        PPL::Variable y { *symbolTable.getVariable("Y") };
        PolyhedralSystem expectedPolyhedralSystem {
            PolyhedralSystem::builder()
                .flow(poly({ x + 0*y <= 4 }))
                .denotation({
                    { ap("p"), powerset({{ x < 3, y <= 3 }, { x < 3, y <= 10 }}) },
                    { ap("q"), powerset({{ x > 3, y >= 4 }})}
                })
                .invariant(powerset({{ x + y >= 3, y >= 4 }}))
                .symbolTable(symbolTable)
                .build()
        };

        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(expectedPolyhedralSystem.totalAtoms() == polyhedralSystem.totalAtoms());
        REQUIRE(expectedPolyhedralSystem.spaceDimension() == polyhedralSystem.spaceDimension());
        REQUIRE(expectedPolyhedralSystem == polyhedralSystem);

        REQUIRE(expectedPolyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!expectedPolyhedralSystem.isMovementForced());
        REQUIRE(!expectedPolyhedralSystem.hasBoundedInvariant());
        REQUIRE(expectedPolyhedralSystem.hasClosedFlow());

        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());
        REQUIRE(!polyhedralSystem.isMovementForced());
        REQUIRE(!polyhedralSystem.hasBoundedInvariant());
        REQUIRE(polyhedralSystem.hasClosedFlow());

        spot::formula stay { ap("stay") };
        Powerset stayInterpretation { powerset({{ x - y >= 3 }, { x - y <= 6 }}) };
        polyhedralSystem.addAtomInterpretation(stay, stayInterpretation);
        REQUIRE(polyhedralSystem.containsAtom(stay));
        std::optional stayAtomInterpretation { polyhedralSystem.getAtomInterpretation(stay) };
        REQUIRE(stayAtomInterpretation.has_value());
        REQUIRE((*stayAtomInterpretation)->interpretation() == *intersect(stayInterpretation, polyhedralSystem.invariant()));
    }

    SECTION("Parse GAP Experiment")
    {
        PolyhedralSystemSharedPtr polyhedralSystem {
            std::make_shared<PolyhedralSystem>(
                std::move(
                    *parsePolyhedralSystem(
                        "Inv ( { a >= 0 & b >= 0 } )"
                        "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                        "p { a >= b + 1 }"
                        "q { b >= a + 1 }"
                        "t0 { t = 0 }"
                        "t1 { t <= 10 }"
                    )
                )
            )
        };

        REQUIRE(polyhedralSystem->spaceDimension() == 3);
        REQUIRE(polyhedralSystem->totalAtoms() == 4);
        REQUIRE(!polyhedralSystem->hasOmnidirectionalFlow());
        REQUIRE(polyhedralSystem->isMovementForced());
        REQUIRE(!polyhedralSystem->hasBoundedInvariant());
        REQUIRE(polyhedralSystem->hasClosedFlow());
    }
}

TEST_CASE("Correctly report syntax errors when parsing PolyhedralSystem", "[bad]")
{
    SECTION("Non-integer coefficient [TEST 1]")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Flow { X <= 4.0 }"
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
               "q { X > 3 & Y >= 4 }"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.semanticErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 1);
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError nonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { nonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 1);
        REQUIRE(locationError.charPositionInLine() == 16);
    }

    SECTION("Non-integer coefficient [TEST 2]")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Flow { X <= 4 }\n"
               "Inv ( { X + Y >= 3 & Y >= 4 } )\n"
               "p ( { X < 3.42 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "q { X > 3 & Y >= 4 }\n"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.semanticErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 1);
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError nonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { nonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 3);
        REQUIRE(locationError.charPositionInLine() == 15);
    }

    SECTION("Non-integer coefficient [TEST 3]")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Flow { X <= 4 }\n"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "q { X > 3 & Y >= 4 }\n"
               "Inv ( { X + Y >= 3.442312 & Y >= 42312.23213 } )\n"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() == 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() == 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 2);
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError firstNonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { firstNonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 4);
        REQUIRE(locationError.charPositionInLine() == 26);
    }

    SECTION("Empty [TEST 1]")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem("   ") };

        REQUIRE(!polyhedralSystemParsingResult.ok());
    }

    SECTION("Empty [TEST 2]")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem("") };

        REQUIRE(!polyhedralSystemParsingResult.ok());
    }

    SECTION("No Sense input")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem(
            "asdfasdf FLow afa fa m42 Inv x<3\n"
            "q { X > 3 & Y >= 4 }\n"
            "best{]}"
        ) };

        REQUIRE(!polyhedralSystemParsingResult.ok());
    }

    SECTION("Invalid variable identifier")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
                "Inv ( { X + Y + Z >= 3 & Y - Z >= 4 } )\n"
                "Flow { X + Z <= 4 }\n"
                "p ( { X + 2Z < 3 & 3Y <= 3 } { X < 3 & Y <= 10 } )\n"
                "q ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )\n"
                "r { 4X > 3 & 4Y >= 4 }\n"
                "v { 4X < 1 & 4Z < 1 }\n"
                "z ( { 10X < 3 & 40Y <= 3 } { 8X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )\n"
                "x ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )\n"
                "u ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )\n"
                "p1 ( { 10X < 3 & 40YFAS <= 3 } { 99X > 10 & 21Y <= 2 } { 2Y <= 10 } { 3X = 3 } )\n" // <-- HERE (Line 10)
                "p2 { 4K > 3 & 4Y >= 4 }\n"
                "p3 { 4L < 1 & 4Z < 1 }\n"
                "u1 ( { 10X < 3 & 40Y <= 3 } { 99X > 10 & 21Y <= 2 } { 2M <= 10 } { 3X = 3 } )\n"
            )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() >= 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() >= 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() >= 2);
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError invalidVariableIdError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { invalidVariableIdError.startLocation() };
        REQUIRE(locationError.line() == 10);
        REQUIRE(locationError.charPositionInLine() == 21);
    }

    SECTION("Parsing an invalid input with >>")
    {
        std::istringstream input {
            "Flow { X <= 4 }"
            "Inv ( { 1*X + Y >= 3 & Y >= 4 } )"
            "pjfksx ( { X < 3 & 1Y <= 3 } { X < 3 & 1*Y <= 10 } )" // Invalid ID
            "q { 1X > 3 & Y >= 4 }"
        };

        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(input.fail());
        REQUIRE(polyhedralSystem.flow().is_empty());
        REQUIRE(polyhedralSystem.invariant().is_empty());
        REQUIRE(polyhedralSystem.preFlow().is_empty());
        REQUIRE(polyhedralSystem.spaceDimension() == 0);
        REQUIRE(polyhedralSystem.totalAtoms() == 0);
    }

    SECTION("Missing INV")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "Flow { X <= 4 }\n"
               "q { X > 3 & Y >= 4 }\n"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() == 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 1);
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError firstNonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { firstNonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 4);
        REQUIRE(locationError.charPositionInLine() == 0);
    }

    SECTION("Missing FLOW")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "q { X > 3 & Y >= 4 }\n"
               "Inv { X <= 4 }\n"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() >= 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() >= 1);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(!polyhedralSystemParsingResult.syntaxErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError firstNonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { firstNonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 4);
        REQUIRE(locationError.charPositionInLine() == 0);
    }

    SECTION("Duplicated atoms")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "q { X > 3 & Y >= 4 }\n"
               "Inv ( { X <= 4 } )\n"
               "Flow { X <= 4 }\n"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )\n"
               "q { X > 3 & Y >= 4 }\n"
           )
        };

        REQUIRE(!polyhedralSystemParsingResult.ok());
        REQUIRE(polyhedralSystemParsingResult.errorCount() == 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.syntaxErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.semanticErrorCount() == 2);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().empty());
        REQUIRE(!polyhedralSystemParsingResult.semanticErrors().empty());

        const ParserError p_duplicate { polyhedralSystemParsingResult.semanticErrors()[0] };
        const PositionError locationError1 { p_duplicate.startLocation() };
        REQUIRE(locationError1.line() == 5);
        REQUIRE(locationError1.charPositionInLine() == 0);

        const ParserError q_duplicate { polyhedralSystemParsingResult.semanticErrors()[1] };
        const PositionError locationError2 { q_duplicate.startLocation() };
        REQUIRE(locationError2.line() == 6);
        REQUIRE(locationError2.charPositionInLine() == 0);
    }
}