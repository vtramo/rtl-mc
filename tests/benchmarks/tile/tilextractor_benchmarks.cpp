#include <pairwise_reduce.h>

#include "TileExtractorGraph.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "test_utils.h"
#include "formula.h"
#include "TileExtractor.h"
#include "TileExtractorDoublyLinkedList.h"
#include "tests/tile/tilextractor_observable_interpretation_fixtures.h"

using namespace SpotUtils;

TEST_CASE(
    "TILEXTRACTOR BENCHMARK 1 (1 observable with 66 patches and 5 tiles).\n"
    "The input for this benchmark corresponds to that of TILEXTRACTOR TEST CASE 4\n"
    "(in tests/tile/tilextractor_tests.cpp)."
)
{
    Powerset observableInterpretation { observableInterpretationTestCase4Fixture() };
    REQUIRE(observableInterpretation.size() == 66);
    Observable observable { Observable { AP({"p"}), observableInterpretation } };

    std::unique_ptr<TileExtractor> tilextractor {};

    SECTION("TileExtractorDoublyLinkedList")
    {
        tilextractor = std::make_unique<TileExtractorDoublyLinkedList>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorDoublyLinkedList benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 5);
    }

    SECTION("TileExtractorGraph")
    {
        tilextractor = std::make_unique<TileExtractorGraph>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorGraph benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 5);
    }
}

TEST_CASE(
    "TILEXTRACTOR BENCHMARK 2 (1 observable with 72 patches and 8 tiles)\n"
    "The input for this benchmark corresponds to that of TILEXTRACTOR TEST CASE 5\n"
    "(in tests/tile/tilextractor_tests.cpp)."
)
{
    Powerset observableInterpretation { observableInterpretationTestCase5Fixture() };
    REQUIRE(observableInterpretation.size() == 72);
    Observable observable { Observable { AP({"p"}), observableInterpretation } };

    std::unique_ptr<TileExtractor> tilextractor {};

    SECTION("TileExtractorDoublyLinkedList")
    {
        tilextractor = std::make_unique<TileExtractorDoublyLinkedList>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorDoublyLinkedList benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 8);
    }

    SECTION("TileExtractorGraph")
    {
        tilextractor = std::make_unique<TileExtractorGraph>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorGraph benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 8);
    }
}

TEST_CASE(
    "TILEXTRACTOR BENCHMARK 3 (1 observable with 216 patches and 20 tiles)\n"
    "The input for this benchmark corresponds to that of TILEXTRACTOR TEST CASE 6\n"
    "(in tests/tile/tilextractor_tests.cpp)."
)
{
    Powerset observableInterpretation { observableInterpretationTestCase6Fixture() };
    REQUIRE(observableInterpretation.size() == 216);
    Observable observable { Observable { AP({"p"}), observableInterpretation } };

    std::unique_ptr<TileExtractor> tilextractor {};

    SECTION("TileExtractorDoublyLinkedList")
    {
        tilextractor = std::make_unique<TileExtractorDoublyLinkedList>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorDoublyLinkedList benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 20);
    }

    SECTION("TileExtractorGraph")
    {
        tilextractor = std::make_unique<TileExtractorGraph>();
        std::vector<Tile> tiles {};

        BENCHMARK("TileExtractorGraph benchmark")
        {
            tiles = tilextractor->extractTiles(observable);
        };

        REQUIRE(tiles.size() == 20);
    }
}