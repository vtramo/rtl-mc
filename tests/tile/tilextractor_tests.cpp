#include <TileExtractorFast.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "ppl_output.h"
#include "TileExtractorComplex.h"
#include "TileExtractorList.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "spot_utils.h"
#include "formula.h"

using namespace SpotUtils;
using namespace PPLUtils;

TEST_CASE("Extract Tiles from Observables")
{
    SECTION("{p, q}")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
               "Flow { X <= 4 }"
               "Inv ( { X + Y >= 3 & Y >= 4 } )"
               "p { -X > -3 & -Y >= -10 & Y >= 4 & X + Y >= 3 }"
               "q { Y >= 4 & X > 3 }"
           )
        };
        REQUIRE(polyhedralSystemParsingResult.ok());
        const PolyhedralSystem& polyhedralSystem { *polyhedralSystemParsingResult };
        REQUIRE(polyhedralSystem.hasOmnidirectionalFlow());

        const Powerset& p { (*polyhedralSystem.getAtomInterpretation("p"))->interpretation() };
        const Powerset& q { (*polyhedralSystem.getAtomInterpretation("q"))->interpretation() };
        const Powerset& notP { (*polyhedralSystem.getAtomInterpretation("p"))->notInterpretation() };
        const Powerset& notQ { (*polyhedralSystem.getAtomInterpretation("q"))->notInterpretation() };

        std::vector observables { polyhedralSystem.generateObservables() };
        REQUIRE(observables.size() == 2);

        SECTION("TileExtractorComplex")
        {
            TileExtractorComplex tileExtractor {};
            std::vector tiles { tileExtractor.extractTiles(observables) };

            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(
                    std::vector {
                        Tile {
                            Observable { AP({"p"}), intersect(p, notQ), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            p
                        },
                        Tile {
                            Observable { AP({"q"}), intersect(notP, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            q
                        },
                    }
                )
            );
        }

        SECTION("TileExtractorList")
        {
            TileExtractorList tileExtractorList {};
            std::vector tiles { tileExtractorList.extractTiles(observables) };

            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(
                    std::vector {
                        Tile {
                            Observable { AP({"p"}), intersect(p, notQ), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            p
                        },
                        Tile {
                            Observable { AP({"q"}), intersect(notP, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            q
                        },
                    }
                )
            );
        }

        SECTION("TileExtractorFast")
        {
            TileExtractorFast tileExtractorFast {};
            std::vector tiles { tileExtractorFast.extractTiles(observables) };

            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(
                    std::vector {
                        Tile {
                            Observable { AP({"p"}), intersect(p, notQ), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            p
                        },
                        Tile {
                            Observable { AP({"q"}), intersect(notP, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                            q
                        },
                    }
                )
            );
        }
    }

    SECTION(
        "(\n"
        "   { x >= -1 & x <= 1 & y >= -1 & y <= 1 }\n"
        "   { x >= 1  & x <= 2 & y >= 0  & y <= 1 }\n"
        "   { x >= 1  & x <= 3 & y >= 1  & y <= 2 }\n"

        "   { x >= 4  & x <= 5 & y >= 0  & y <= 1 }\n"
        "   { x >= 4  & x <= 6 & y >= 1  & y <= 2 }\n"
        ")"
    )
    {
        PPL::Variable x { 0 };
        PPL::Variable y { 1 };

        Powerset observableInterpretation {
            PPLUtils::powerset({
                { x >= -1, x <= 1, y >= -1, y <= 1 },
                { x >= 1 , x <= 2, y >= 0 , y <= 1 },
                { x >= 1 , x <= 3, y >= 1 , y <= 2 },
                { x >= 4 , x <= 5, y >= 0 , y <= 1 },
                { x >= 4 , x <= 6, y >= 1 , y <= 2 },
            })
        };
        REQUIRE(observableInterpretation.size() == 5);

        Observable observable { Observable { AP({"p"}), observableInterpretation } };
        Tile expectedTile1 {
            observable,
            PPLUtils::powerset({
                { x >= -1, x <= 1, y >= -1, y <= 1 },
                { x >= 1 , x <= 2, y >= 0 , y <= 1 },
                { x >= 1 , x <= 3, y >= 1 , y <= 2 },
            })
        };
        Tile expectedTile2 {
            observable,
            PPLUtils::powerset({
                { x >= 4 , x <= 5, y >= 0 , y <= 1 },
                { x >= 4 , x <= 6, y >= 1 , y <= 2 },
            })
        };

        SECTION("TileExtractorComplex")
        {
            TileExtractorComplex tileExtractor {};
            std::vector tiles { tileExtractor.extractTiles(observable) };


            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector { expectedTile1, expectedTile2 })
            );
        }

        SECTION("TileExtractorList")
        {
            TileExtractorList tileExtractorList {};
            std::vector tiles { tileExtractorList.extractTiles(observable) };


            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector { expectedTile1, expectedTile2 })
            );
        }

        SECTION("TileExtractorFast")
        {
            TileExtractorFast tileExtractorFast {};
            std::vector tiles { tileExtractorFast.extractTiles(observable) };


            REQUIRE(tiles.size() == 2);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector { expectedTile1, expectedTile2 })
            );
        }
    }

    SECTION(
        "(\n"
        "   { x > -1 & x < 1 & y > -1 & y < 1 }\n"
        "   { x > 1  & x < 2 & y > 0  & y < 1 }\n"
        "   { x > 1  & x < 3 & y > 1  & y < 2 }\n"

        "   { x > 4  & x < 5 & y > 0  & y < 1 }\n"
        "   { x > 4  & x < 6 & y > 1  & y < 2 }\n"
        ")"
    )
    {
        PPL::Variable x { 0 };
        PPL::Variable y { 1 };

        Powerset observableInterpretation {
            PPLUtils::powerset({
                { x > -1, x < 1, y > -1, y < 1 },
                { x > 1 , x < 2, y > 0 , y < 1 },
                { x > 1 , x < 3, y > 1 , y < 2 },
                { x > 4 , x < 5, y > 0 , y < 1 },
                { x > 4 , x < 6, y > 1 , y < 2 },
            })
        };
        REQUIRE(observableInterpretation.size() == 5);

        Observable observable { Observable { AP({"p"}), observableInterpretation } };
        Tile expectedTile1 {
            observable,
            PPLUtils::powerset({{ x > -1, x < 1, y > -1, y < 1 }})
        };
        Tile expectedTile2 {
            observable,
            PPLUtils::powerset({{ x > 1 , x < 2, y > 0 , y < 1 }})
        };
        Tile expectedTile3 {
            observable,
            PPLUtils::powerset({{ x > 1 , x < 3, y > 1 , y < 2 }})
        };
        Tile expectedTile4 {
            observable,
            PPLUtils::powerset({{ x > 4 , x < 5, y > 0 , y < 1 }})
        };
        Tile expectedTile5 {
            observable,
            PPLUtils::powerset({{ x > 4 , x < 6, y > 1 , y < 2 }})
        };

        SECTION("TileExtractorComplex")
        {
            TileExtractorComplex tileExtractor {};
            std::vector tiles { tileExtractor.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }

        SECTION("TileExtractorList")
        {
            TileExtractorList tileExtractorList {};
            std::vector tiles { tileExtractorList.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }

        SECTION("TileExtractorFast")
        {
            TileExtractorFast tileExtractorFast {};
            std::vector tiles { tileExtractorFast.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }
    }

    SECTION(
        "(\n"
            "{ -x >= 2, -y >= -4, y >= 3, x >= -3 },"
            "{ -x >= 2, -y >= -5, y >= 4, x >= -3 },"
            "{ -x >= -18, -y >= -5, y >= 4, x >= 17 },"
            "{ -x >= -18, -y >= -4, y >= 3, x >= 17 },"
            "{ -x >= -17, -y >= -5, y >= 4, x >= 15 },"
            "{ -x >= -16, -y >= -4, y >= 3, x >= 15 },"
            "{ -x >= -15, -y >= -5, y >= 4, x >= 10 },"
            "{ -x >= -16, -y >= -6, y >= 5, x >= 10 },"
            "{ -x >= -10, -y >= -6, y >= 5, x >= 4 },"
            "{ -x >= -8, -y >= -4, y >= 3, x >= 4 },"
            "{ -x >= -9, -y >= -5, y >= 4, x >= 4 },"
            "{ -x >= -6, -y >= -2, y >= 1, x >= 4 },"
            "{ -x >= -7, -y >= -3, y >= 2, x >= 4 },"
            "{ -x >= -5, -y >= -4, y >= 3, x >= 1 },"
            "{ -x >= -3, -y >= -2, y >= 1, x >= 1 },"
            "{ -x >= -1, -y >= -1, y >= -1, x >= -1 },"
            "{ -x >= -2, -y >= -1, y >= 0, x >= 1 },"
            "{ -x >= -4, -y >= -3, y >= 2, x >= 1 },"
            "{ -x >= -7, -y >= -6, y >= 5, x >= 1 },"
            "{ -x >= -6, -y >= -5, y >= 4, x >= 1 },"
            "{ -x >= 1, -y >= -7, y >= 6, x >= -2 },"
            "{ -x >= 0, -y >= -7, y >= 6, x >= -1 },"
            "{ -x >= -4, -y >= -10, y >= 9, x >= 3 },"
            "{ -x >= -2, -y >= -9, y >= 8, x >= 0 },"
            "{ -x >= -3, -y >= -10, y >= 9, x >= 0 },"
            "{ -x >= -1, -y >= -8, y >= 7, x >= 0 },"
            "{ -x >= -12, -y >= -9, y >= 8, x >= 4 },"
            "{ -x >= -11, -y >= -8, y >= 7, x >= 4 },"
            "{ -x >= -9, -y >= -8, y >= 7, x >= 1 },"
            "{ -x >= -8, -y >= -7, y >= 6, x >= 1 },"
            "{ -x >= -5, -y >= -1, y >= 0, x >= 4 },"
            "{ -x >= -13, -y >= -10, y >= 9, x >= 4 },"
            "{ -x >= -13, -y >= -3, y >= 2, x >= 10 },"
            "{ -x >= -11, -y >= -1, y >= 0, x >= 10 },"
            "{ -x >= -12, -y >= -2, y >= 1, x >= 10 },"
            "{ -x >= -14, -y >= -4, y >= 3, x >= 10 },"
            "{ -x >= 1, -y >= -6, y >= 5, x >= -2 },"
            "{ -x >= -18, -y >= -6, y >= 5, x >= 17 },"

            "{ -x >= -58, -y >= -57, y >= 56, x >= 50 },"
            "{ -x >= -56, -y >= -55, y >= 54, x >= 50 },"
            "{ -x >= -54, -y >= -53, y >= 52, x >= 50 },"
            "{ -x >= -53, -y >= -52, y >= 50, x >= 50 },"
            "{ -x >= -55, -y >= -54, y >= 53, x >= 50 },"
            "{ -x >= -57, -y >= -56, y >= 55, x >= 50 },"
            "{ -x >= -59, -y >= -58, y >= 57, x >= 50 },"

            "{ -x >= -78, -y >= -77, y >= 76, x >= 70 },"
            "{ -x >= -76, -y >= -75, y >= 74, x >= 70 },"
            "{ -x >= -74, -y >= -73, y >= 72, x >= 70 },"
            "{ -x >= -73, -y >= -72, y >= 70, x >= 70 },"
            "{ -x >= -75, -y >= -74, y >= 73, x >= 70 },"
            "{ -x >= -77, -y >= -76, y >= 75, x >= 70 },"
            "{ -x >= -79, -y >= -78, y >= 77, x >= 70 },"

            "{ -x >= -98, -y >= -97, y >= 96, x >= 90 },"
            "{ -x >= -96, -y >= -95, y >= 94, x >= 90 },"
            "{ -x >= -94, -y >= -93, y >= 92, x >= 90 },"
            "{ -x >= -93, -y >= -92, y >= 90, x >= 90 },"
            "{ -x >= -95, -y >= -94, y >= 93, x >= 90 },"
            "{ -x >= -97, -y >= -96, y >= 95, x >= 90 },"
            "{ -x >= -99, -y >= -98, y >= 97, x >= 90 },"

            "{ -x >= -118, -y >= -117, y >= 116, x >= 110 },"
            "{ -x >= -116, -y >= -115, y >= 114, x >= 110 },"
            "{ -x >= -114, -y >= -113, y >= 112, x >= 110 },"
            "{ -x >= -113, -y >= -112, y >= 110, x >= 110 },"
            "{ -x >= -115, -y >= -114, y >= 113, x >= 110 },"
            "{ -x >= -117, -y >= -116, y >= 115, x >= 110 },"
            "{ -x >= -119, -y >= -118, y >= 117, x >= 110 }"
        ")"
    )
    {
        PPL::Variable x { 0 };
        PPL::Variable y { 1 };

        Powerset observableInterpretation {
            PPLUtils::powerset({
                { -x >= 2, -y >= -4, y >= 3, x >= -3 },
                { -x >= 2, -y >= -5, y >= 4, x >= -3 },
                { -x >= -18, -y >= -5, y >= 4, x >= 17 },
                { -x >= -18, -y >= -4, y >= 3, x >= 17 },
                { -x >= -17, -y >= -5, y >= 4, x >= 15 },
                { -x >= -16, -y >= -4, y >= 3, x >= 15 },
                { -x >= -15, -y >= -5, y >= 4, x >= 10 },
                { -x >= -16, -y >= -6, y >= 5, x >= 10 },
                { -x >= -10, -y >= -6, y >= 5, x >= 4 },
                { -x >= -8, -y >= -4, y >= 3, x >= 4 },
                { -x >= -9, -y >= -5, y >= 4, x >= 4 },
                { -x >= -6, -y >= -2, y >= 1, x >= 4 },
                { -x >= -7, -y >= -3, y >= 2, x >= 4 },
                { -x >= -5, -y >= -4, y >= 3, x >= 1 },
                { -x >= -3, -y >= -2, y >= 1, x >= 1 },
                { -x >= -1, -y >= -1, y >= -1, x >= -1 },
                { -x >= -2, -y >= -1, y >= 0, x >= 1 },
                { -x >= -4, -y >= -3, y >= 2, x >= 1 },
                { -x >= -7, -y >= -6, y >= 5, x >= 1 },
                { -x >= -6, -y >= -5, y >= 4, x >= 1 },
                { -x >= 1, -y >= -7, y >= 6, x >= -2 },
                { -x >= 0, -y >= -7, y >= 6, x >= -1 },
                { -x >= -4, -y >= -10, y >= 9, x >= 3 },
                { -x >= -2, -y >= -9, y >= 8, x >= 0 },
                { -x >= -3, -y >= -10, y >= 9, x >= 0 },
                { -x >= -1, -y >= -8, y >= 7, x >= 0 },
                { -x >= -12, -y >= -9, y >= 8, x >= 4 },
                { -x >= -11, -y >= -8, y >= 7, x >= 4 },
                { -x >= -9, -y >= -8, y >= 7, x >= 1 },
                { -x >= -8, -y >= -7, y >= 6, x >= 1 },
                { -x >= -5, -y >= -1, y >= 0, x >= 4 },
                { -x >= -13, -y >= -10, y >= 9, x >= 4 },
                { -x >= -13, -y >= -3, y >= 2, x >= 10 },
                { -x >= -11, -y >= -1, y >= 0, x >= 10 },
                { -x >= -12, -y >= -2, y >= 1, x >= 10 },
                { -x >= -14, -y >= -4, y >= 3, x >= 10 },
                { -x >= 1, -y >= -6, y >= 5, x >= -2 },
                { -x >= -18, -y >= -6, y >= 5, x >= 17 },

                { -x >= -58, -y >= -57, y >= 56, x >= 50 },
                { -x >= -56, -y >= -55, y >= 54, x >= 50 },
                { -x >= -54, -y >= -53, y >= 52, x >= 50 },
                { -x >= -53, -y >= -52, y >= 50, x >= 50 },
                { -x >= -55, -y >= -54, y >= 53, x >= 50 },
                { -x >= -57, -y >= -56, y >= 55, x >= 50 },
                { -x >= -59, -y >= -58, y >= 57, x >= 50 },

                { -x >= -78, -y >= -77, y >= 76, x >= 70 },
                { -x >= -76, -y >= -75, y >= 74, x >= 70 },
                { -x >= -74, -y >= -73, y >= 72, x >= 70 },
                { -x >= -73, -y >= -72, y >= 70, x >= 70 },
                { -x >= -75, -y >= -74, y >= 73, x >= 70 },
                { -x >= -77, -y >= -76, y >= 75, x >= 70 },
                { -x >= -79, -y >= -78, y >= 77, x >= 70 },

                { -x >= -98, -y >= -97, y >= 96, x >= 90 },
                { -x >= -96, -y >= -95, y >= 94, x >= 90 },
                { -x >= -94, -y >= -93, y >= 92, x >= 90 },
                { -x >= -93, -y >= -92, y >= 90, x >= 90 },
                { -x >= -95, -y >= -94, y >= 93, x >= 90 },
                { -x >= -97, -y >= -96, y >= 95, x >= 90 },
                { -x >= -99, -y >= -98, y >= 97, x >= 90 },

                { -x >= -118, -y >= -117, y >= 116, x >= 110 },
                { -x >= -116, -y >= -115, y >= 114, x >= 110 },
                { -x >= -114, -y >= -113, y >= 112, x >= 110 },
                { -x >= -113, -y >= -112, y >= 110, x >= 110 },
                { -x >= -115, -y >= -114, y >= 113, x >= 110 },
                { -x >= -117, -y >= -116, y >= 115, x >= 110 },
                { -x >= -119, -y >= -118, y >= 117, x >= 110 },
            })
        };
        REQUIRE(observableInterpretation.size() == 66);

        Observable observable { Observable { AP({"p"}), observableInterpretation } };
        Tile expectedTile1 {
            observable,
            PPLUtils::powerset({
                { -x >= 2, -y >= -4, y >= 3, x >= -3 },
                { -x >= 2, -y >= -5, y >= 4, x >= -3 },
                { -x >= -18, -y >= -5, y >= 4, x >= 17 },
                { -x >= -18, -y >= -4, y >= 3, x >= 17 },
                { -x >= -17, -y >= -5, y >= 4, x >= 15 },
                { -x >= -16, -y >= -4, y >= 3, x >= 15 },
                { -x >= -15, -y >= -5, y >= 4, x >= 10 },
                { -x >= -16, -y >= -6, y >= 5, x >= 10 },
                { -x >= -10, -y >= -6, y >= 5, x >= 4 },
                { -x >= -8, -y >= -4, y >= 3, x >= 4 },
                { -x >= -9, -y >= -5, y >= 4, x >= 4 },
                { -x >= -6, -y >= -2, y >= 1, x >= 4 },
                { -x >= -7, -y >= -3, y >= 2, x >= 4 },
                { -x >= -5, -y >= -4, y >= 3, x >= 1 },
                { -x >= -3, -y >= -2, y >= 1, x >= 1 },
                { -x >= -1, -y >= -1, y >= -1, x >= -1 },
                { -x >= -2, -y >= -1, y >= 0, x >= 1 },
                { -x >= -4, -y >= -3, y >= 2, x >= 1 },
                { -x >= -7, -y >= -6, y >= 5, x >= 1 },
                { -x >= -6, -y >= -5, y >= 4, x >= 1 },
                { -x >= 1, -y >= -7, y >= 6, x >= -2 },
                { -x >= 0, -y >= -7, y >= 6, x >= -1 },
                { -x >= -4, -y >= -10, y >= 9, x >= 3 },
                { -x >= -2, -y >= -9, y >= 8, x >= 0 },
                { -x >= -3, -y >= -10, y >= 9, x >= 0 },
                { -x >= -1, -y >= -8, y >= 7, x >= 0 },
                { -x >= -12, -y >= -9, y >= 8, x >= 4 },
                { -x >= -11, -y >= -8, y >= 7, x >= 4 },
                { -x >= -9, -y >= -8, y >= 7, x >= 1 },
                { -x >= -8, -y >= -7, y >= 6, x >= 1 },
                { -x >= -5, -y >= -1, y >= 0, x >= 4 },
                { -x >= -13, -y >= -10, y >= 9, x >= 4 },
                { -x >= -13, -y >= -3, y >= 2, x >= 10 },
                { -x >= -11, -y >= -1, y >= 0, x >= 10 },
                { -x >= -12, -y >= -2, y >= 1, x >= 10 },
                { -x >= -14, -y >= -4, y >= 3, x >= 10 },
                { -x >= 1, -y >= -6, y >= 5, x >= -2 },
                { -x >= -18, -y >= -6, y >= 5, x >= 17 },
            })
        };
        Tile expectedTile2 {
            observable,
            PPLUtils::powerset({
                { -x >= -58, -y >= -57, y >= 56, x >= 50 },
                { -x >= -56, -y >= -55, y >= 54, x >= 50 },
                { -x >= -54, -y >= -53, y >= 52, x >= 50 },
                { -x >= -53, -y >= -52, y >= 50, x >= 50 },
                { -x >= -55, -y >= -54, y >= 53, x >= 50 },
                { -x >= -57, -y >= -56, y >= 55, x >= 50 },
                { -x >= -59, -y >= -58, y >= 57, x >= 50 },
            })
        };
        Tile expectedTile3 {
            observable,
            PPLUtils::powerset({
                { -x >= -78, -y >= -77, y >= 76, x >= 70 },
                { -x >= -76, -y >= -75, y >= 74, x >= 70 },
                { -x >= -74, -y >= -73, y >= 72, x >= 70 },
                { -x >= -73, -y >= -72, y >= 70, x >= 70 },
                { -x >= -75, -y >= -74, y >= 73, x >= 70 },
                { -x >= -77, -y >= -76, y >= 75, x >= 70 },
                { -x >= -79, -y >= -78, y >= 77, x >= 70 },
            })
        };
        Tile expectedTile4 {
            observable,
            PPLUtils::powerset({
                { -x >= -98, -y >= -97, y >= 96, x >= 90 },
                { -x >= -96, -y >= -95, y >= 94, x >= 90 },
                { -x >= -94, -y >= -93, y >= 92, x >= 90 },
                { -x >= -93, -y >= -92, y >= 90, x >= 90 },
                { -x >= -95, -y >= -94, y >= 93, x >= 90 },
                { -x >= -97, -y >= -96, y >= 95, x >= 90 },
                { -x >= -99, -y >= -98, y >= 97, x >= 90 },
            })
        };
        Tile expectedTile5 {
            observable,
            PPLUtils::powerset({
                { -x >= -118, -y >= -117, y >= 116, x >= 110 },
                { -x >= -116, -y >= -115, y >= 114, x >= 110 },
                { -x >= -114, -y >= -113, y >= 112, x >= 110 },
                { -x >= -113, -y >= -112, y >= 110, x >= 110 },
                { -x >= -115, -y >= -114, y >= 113, x >= 110 },
                { -x >= -117, -y >= -116, y >= 115, x >= 110 },
                { -x >= -119, -y >= -118, y >= 117, x >= 110 },
            })
        };

        SECTION("TileExtractorComplex")
        {
            TileExtractorComplex tileExtractor {};
            std::vector tiles { tileExtractor.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }

        SECTION("TileExtractorList")
        {
            TileExtractorList tileExtractorList {};
            std::vector tiles { tileExtractorList.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }

        SECTION("TileExtractorFast")
        {
            TileExtractorFast tileExtractorFast {};
            std::vector tiles { tileExtractorFast.extractTiles(observable) };


            REQUIRE(tiles.size() == 5);
            REQUIRE_THAT(
                tiles,
                Catch::Matchers::UnorderedEquals(std::vector {
                    expectedTile1,
                    expectedTile2,
                    expectedTile3,
                    expectedTile4,
                    expectedTile5
                })
            );
        }
    }
}