#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "ppl_output.h"
#include "TileExtractor.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "spot_utils.h"

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
        REQUIRE(polyhedralSystem.isOmnidirectionalFlow());

        const Powerset& p { (*polyhedralSystem.getAtomInterpretation("p"))->interpretation() };
        const Powerset& q { (*polyhedralSystem.getAtomInterpretation("q"))->interpretation() };
        const Powerset& notP { (*polyhedralSystem.getAtomInterpretation("p"))->notInterpretation() };
        const Powerset& notQ { (*polyhedralSystem.getAtomInterpretation("q"))->notInterpretation() };

        std::vector observables { polyhedralSystem.generateObservables() };
        REQUIRE(observables.size() == 3);


        TileExtractor tileExtractor {};
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


        TileExtractor tileExtractor {};
        std::vector tiles { tileExtractor.extractTiles(observable) };


        REQUIRE(tiles.size() == 2);
        REQUIRE_THAT(
            tiles,
            Catch::Matchers::UnorderedEquals(std::vector { expectedTile1, expectedTile2 })
        );
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


        TileExtractor tileExtractor {};
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
}