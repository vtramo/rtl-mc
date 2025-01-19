#include <catch2/catch_test_macros.hpp>
#include "PolyhedralSystemParsingResult.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"

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
        REQUIRE(polyhedralSystem.getSpaceDimension() == 2);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 2);
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
        REQUIRE(polyhedralSystem.getSpaceDimension() == 2);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 2);
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

        REQUIRE(polyhedralSystem.getSpaceDimension() == 2);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 2);
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

        REQUIRE(polyhedralSystem.getSpaceDimension() == 2);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 3);
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

        REQUIRE(polyhedralSystem.getSpaceDimension() == 3);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 4);
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

        REQUIRE(polyhedralSystem.getSpaceDimension() == 6);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 11);
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
                .flow(PPLUtils::poly({ x + 0*y <= 4 }))
                .denotation({
                    { "p", PPLUtils::powerset({{ x < 3, y <= 3 }, { x < 3, y <= 10 }}) },
                    { "q", PPLUtils::powerset({{ x > 3, y >= 4 }})}
                })
                .invariant(PPLUtils::powerset({{ x + y >= 3, y >= 4 }}))
                .symbolTable(symbolTable)
                .build()
        };


        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;

        REQUIRE(expectedPolyhedralSystem.getTotalAtoms() == polyhedralSystem.getTotalAtoms());
        REQUIRE(expectedPolyhedralSystem.getSpaceDimension() == polyhedralSystem.getSpaceDimension());
        REQUIRE(expectedPolyhedralSystem == polyhedralSystem);
    }

    SECTION("Read a PolyhedralSystem from a file, then parse, then compare with the expected PolyhedralSystem")
    {
        std::ifstream input;
        std::string testCaseFilePath { "polyhedral-system-test-1.txt" };
        input.open(testCaseFilePath);

        PolyhedralSystem polyhedralSystem {};
        input >> polyhedralSystem;
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
        REQUIRE(polyhedralSystemParsingResult.unknownErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 1);
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

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
        REQUIRE(polyhedralSystemParsingResult.unknownErrorCount() == 0);
        REQUIRE(polyhedralSystemParsingResult.lexicalErrors().empty());
        REQUIRE(polyhedralSystemParsingResult.syntaxErrors().size() == 1);
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

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
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

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
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

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
        REQUIRE(polyhedralSystem.getFlow().is_empty());
        REQUIRE(polyhedralSystem.getInvariant().is_empty());
        REQUIRE(polyhedralSystem.getPreFlow().is_empty());
        REQUIRE(polyhedralSystem.getSpaceDimension() == 0);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 0);
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
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

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
        REQUIRE(polyhedralSystemParsingResult.unknownErrors().empty());

        const ParserError firstNonIntegerError { polyhedralSystemParsingResult.syntaxErrors()[0] };
        const PositionError locationError { firstNonIntegerError.startLocation() };
        REQUIRE(locationError.line() == 4);
        REQUIRE(locationError.charPositionInLine() == 0);
    }
}