#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "ppl_utils.h"

using namespace PPLUtils;

std::vector<Powerset> buildPowersets()
{
    PPL::Variable x { 0 };
    PPL::Variable y { 1 };
    PPL::Variable z { 2 };
    PPL::Variable k { 3 };
    PPL::Variable t { 4 };

    std::vector<Powerset> powersets {
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x > 0, x < 5, y >= 2, y <= 8, z > 1, k > 1, t > 1 },
                  { k >= 3, t > 1 }}, 5),
        powerset({{ x >= 1, x <= 3, y >= 4, y <= 9, z > 2, k > 0, t > 0 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 5, y <= 10, z > 1, k >= 1, t > 0 },
                  { k >= 4, t >= 1 }}, 5),
        powerset({{ x > 0, x <= 4, y >= 3, y <= 7, z > 2, k > 1, t > 1 },
                  { k >= 2, t > 2 }}, 5),
        powerset({{ x >= 1, x < 6, y >= 6, y <= 10, z > 0, k >= 2, t >= 1 },
                  { k >= 5, t > 2 }}, 5),
        powerset({{ x > 2, x <= 7, y >= 4, y <= 8, z > 1, k > 3, t > 1 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x >= 3, x < 8, y >= 2, y <= 6, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 0, x < 4, y >= 1, y <= 5, z > 0, k >= 1, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 3, y <= 8, z > 1, k > 2, t > 1 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x >= 2, x <= 6, y >= 5, y <= 9, z > 0, k >= 3, t >= 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 3, x < 7, y >= 4, y <= 8, z > 1, k > 3, t > 2 },
                  { k >= 4, t >= 3 }}, 5),
        powerset({{ x >= 0, x < 5, y >= 3, y <= 7, z > 2, k >= 2, t > 2 },
                  { k >= 3, t > 3 }}, 5),
        powerset({{ x > 1, x <= 6, y >= 5, y <= 10, z > 1, k > 1, t > 1 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x >= 2, x < 7, y >= 2, y <= 6, z > 0, k >= 3, t >= 2 },
                  { k >= 4, t >= 2 }}, 5),
        powerset({{ x > 3, x < 8, y >= 4, y <= 9, z > 1, k > 3, t > 2 },
                  { k >= 5, t >= 3 }}, 5),
        powerset({{ x >= 1, x <= 5, y >= 2, y <= 7, z > 1, k > 2, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x > 2, x < 6, y >= 3, y <= 8, z > 0, k >= 2, t >= 1 },
                  { k >= 4, t > 2 }}, 5),
        powerset({{ x >= 3, x <= 7, y >= 5, y <= 10, z > 1, k > 3, t > 2 },
                  { k >= 5, t > 3 }}, 5),
        powerset({{ x > 1, x < 5, y >= 2, y <= 6, z > 0, k >= 1, t > 1 },
                  { k >= 3, t >= 2 }}, 5),
        powerset({{ x >= 0, x <= 4, y >= 4, y <= 8, z > 1, k > 1, t > 2 },
                  { k >= 4, t >= 3 }}, 5)
    };

    return powersets;
}

TEST_CASE("PPL benchmarks")
{

    std::vector<Powerset> powersets { buildPowersets() };
    BENCHMARK("Intersection")
    {
        PowersetUniquePtr result { std::make_unique<Powerset>(5, PPL::UNIVERSE) };
        for (unsigned i { 0 }; i < powersets.size(); ++i)
        {
            result = intersect(*result, powersets[i]);
        }
    };

    BENCHMARK("Intersection assign")
    {
        Powerset result1 { 5, PPL::UNIVERSE };
        for (unsigned i { 0 }; i < powersets.size(); ++i)
        {
            Powerset result { result1 };
            result.intersection_assign(powersets[i]);
            result1 = result;
        }
    };
}