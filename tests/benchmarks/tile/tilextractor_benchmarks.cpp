#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "test_utils.h"
#include "spot_utils.h"
#include "formula.h"
#include "TileExtractor.h"
#include "TileExtractorComplex.h"
#include "TileExtractorFast.h"
#include "TileExtractorList.h"

using namespace SpotUtils;

TEST_CASE("TileExtractor benchmarks")
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };

    Powerset observableInterpretation {
        PPLUtils::powerset({
            { x >= -1, x <= 1, y >= -1, y <= 1 },
            { x >= 1 , x <= 2, y >= 0 , y <= 1 },
            { x >= 1 , x <= 3, y >= 1 , y <= 2 },
            { x >= 1 , x <= 4, y >= 2 , y <= 3 },
            { x >= 1 , x <= 5, y >= 3 , y <= 4 },
            { x >= 1 , x <= 6, y >= 4 , y <= 5 },
            { x >= 1 , x <= 7, y >= 5 , y <= 6 },
            { x >= 1 , x <= 8, y >= 6 , y <= 7 },
            { x >= 1 , x <= 9, y >= 7 , y <= 8 },

            { x >= 4 , x <= 5, y >= 0 , y <= 1 },
            { x >= 4 , x <= 6, y >= 1 , y <= 2 },
            { x >= 4 , x <= 7, y >= 2 , y <= 3 },
            { x >= 4 , x <= 8, y >= 3 , y <= 4 },
            { x >= 4 , x <= 9, y >= 4 , y <= 5 },
            { x >= 4 , x <= 10, y >= 5 , y <= 6 },
            { x >= 4 , x <= 11, y >= 7 , y <= 8 },
            { x >= 4 , x <= 12, y >= 8 , y <= 9 },
            { x >= 4 , x <= 13, y >= 9 , y <= 10 },

            { x >= 10 , x <= 11, y >= 0 , y <= 1 },
            { x >= 10 , x <= 12, y >= 1 , y <= 2 },
            { x >= 10 , x <= 13, y >= 2 , y <= 3 },
            { x >= 10 , x <= 14, y >= 3 , y <= 4 },
            { x >= 10 , x <= 15, y >= 4 , y <= 5 },
            { x >= 10 , x <= 16, y >= 5 , y <= 6 },

            { x >= 0 , x <= 1, y >= 7 , y <= 8 },
            { x >= 0 , x <= 1, y >= 8 , y <= 9 },
            { x >= 0 , x <= 2, y >= 8 , y <= 9 },
            { x >= 0 , x <= 3, y >= 9 , y <= 10 },
            { x >= 2 , x <= 3, y >= 9 , y <= 10 },
            { x >= 3 , x <= 4, y >= 9 , y <= 10 },

            { x >= -1 , x <= 0, y >= 6 , y <= 7 },
            { x >= -2 , x <= -1, y >= 5 , y <= 6 },
            { x >= -2 , x <= -1, y >= 6 , y <= 7 },
            { x >= 15 , x <= 16, y >= 3 , y <= 4 },
            { x >= 15 , x <= 17, y >= 4 , y <= 5 },

            { x >= 17 , x <= 18, y >= 5 , y <= 6 },
            { x >= 17 , x <= 18, y >= 4 , y <= 5 },
            { x >= 17 , x <= 18, y >= 3 , y <= 4 },
            { x >= -3 , x <= -2, y >= 3 , y <= 4 },
            { x >= -3 , x <= -2, y >= 4 , y <= 5 },

            { x >= 50 , x <= 52, y >= 50 , y <= 50 },
            { x >= 50 , x <= 53, y >= 50 , y <= 52 },
            { x >= 50 , x <= 54, y >= 52 , y <= 53 },
            { x >= 50 , x <= 55, y >= 53 , y <= 54 },
            { x >= 50 , x <= 56, y >= 54 , y <= 55 },
            { x >= 50 , x <= 57, y >= 55 , y <= 56 },
            { x >= 50 , x <= 58, y >= 56 , y <= 57 },
            { x >= 50 , x <= 59, y >= 57 , y <= 58 },

            { x >= 70 , x <= 72, y >= 70 , y <= 70 },
            { x >= 70 , x <= 73, y >= 70 , y <= 72 },
            { x >= 70 , x <= 74, y >= 72 , y <= 73 },
            { x >= 70 , x <= 75, y >= 73 , y <= 74 },
            { x >= 70 , x <= 76, y >= 74 , y <= 75 },
            { x >= 70 , x <= 77, y >= 75 , y <= 76 },
            { x >= 70 , x <= 78, y >= 76 , y <= 77 },
            { x >= 70 , x <= 79, y >= 77 , y <= 78 },

            { x >= 90 , x <= 92, y >= 90 , y <= 90 },
            { x >= 90 , x <= 93, y >= 90 , y <= 92 },
            { x >= 90 , x <= 94, y >= 92 , y <= 93 },
            { x >= 90 , x <= 95, y >= 93 , y <= 94 },
            { x >= 90 , x <= 96, y >= 94 , y <= 95 },
            { x >= 90 , x <= 97, y >= 95 , y <= 96 },
            { x >= 90 , x <= 98, y >= 96 , y <= 97 },
            { x >= 90 , x <= 99, y >= 97 , y <= 98 },

            { x >= 110 , x <= 112, y >= 110 , y <= 110 },
            { x >= 110 , x <= 113, y >= 110 , y <= 112 },
            { x >= 110 , x <= 114, y >= 112 , y <= 113 },
            { x >= 110 , x <= 115, y >= 113 , y <= 114 },
            { x >= 110 , x <= 116, y >= 114 , y <= 115 },
            { x >= 110 , x <= 117, y >= 115 , y <= 116 },
            { x >= 110 , x <= 118, y >= 116 , y <= 117 },
            { x >= 110 , x <= 119, y >= 117 , y <= 118 },
        })
    };

    Observable observable { Observable { AP({"p"}), observableInterpretation } };


    std::unique_ptr<TileExtractor> tilextractor {};
    SECTION("TileExtractorList")
    {
        tilextractor = std::make_unique<TileExtractorList>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorList benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 5);
    }

    SECTION("TileExtractorComplex")
    {
        tilextractor = std::make_unique<TileExtractorComplex>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorComplex benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 5);
    }

    SECTION("TileExtractorFast")
    {
        tilextractor = std::make_unique<TileExtractorFast>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorFast benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 5);
    }
}