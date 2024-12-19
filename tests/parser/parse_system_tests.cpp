#include <ppl_utils.h>
#include <catch2/catch_test_macros.hpp>
#include "PolyhedralSystem.h"

TEST_CASE("Correctly builds PolyhedralSystem from a string", "[parser]")
{
    SECTION("Building PolyhedralSystem with 'Flow' constraint first")
    {
        const PolyhedralSystem polyhedralSystem {
            buildPolyhedralSystem(
               "Flow { X <= 4 }"
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
               "q { X > 3 & Y >= 4 }"
           )
       };

        REQUIRE(polyhedralSystem.getSpaceDimension() == 2);
        REQUIRE(polyhedralSystem.getTotalAtoms() == 2);
    }

    SECTION("Building PolyhedralSystem with 'Inv' constraint first")
    {
        const PolyhedralSystem polyhedralSystem {
            buildPolyhedralSystem(
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "Flow { X <= 4 }"
               "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
               "q { X > 3 & Y >= 4 }"
           )
       };

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

    // SECTION("a")
    // {
    //     std::istringstream input {
    //         "Flow { X <= 4 }"
    //         "Inv ( { X + Y >= 3 & Y >= 4 } )"
    //         "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
    //         "q { X > 3 & Y >= 4 }"
    //     };
    //
    //     PolyhedralSystem polyhedralSystem {};
    //     input >> polyhedralSystem;
    //
    //     PolyhedralSystemSymbolTable symbolTable {};
    //     symbolTable
    //         .addVariables({ "x", "y" })
    //         .addAtoms({ "p", "q" });
    //
    //     PPL::Variable x { *symbolTable.getVariable("x") };
    //     PPL::Variable y { *symbolTable.getVariable("y") };
    //     PolyhedralSystem expectedPolyhedralSystem {
    //         PolyhedralSystem::builder()
    //             .flow(PPLUtils::nnc({ x + 0*y <= 4 }))
    //             .denotation({
    //                 { "p", PPLUtils::powerset({{ x < 3, y <= 3 }, { x < 3, y <= 10 }}) },
    //                 { "q", PPLUtils::powerset({{ x > 3, y >= 4 }})}
    //             })
    //             .invariant(PPLUtils::powerset({{ x + y >= 3, y >= 4 }}))
    //             .symbolTable(symbolTable)
    //             .build()
    //     };
    //
    //     REQUIRE(expectedPolyhedralSystem == polyhedralSystem);
    // }
}