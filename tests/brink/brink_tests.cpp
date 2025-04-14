#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "ppl_output.h"
#include "spot_utils.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"
#include "formula.h"
#include "brink_stay_atoms.h"

using namespace PPLUtils;
using namespace SpotUtils;

bool allObservablesAreDisjoint(const std::vector<Observable>& observables);

TEST_CASE("Generate observables")
{
    SECTION("brink may")
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

        const auto& symbolTable { polyhedralSystem->symbolTable() };
        PPL::Variable a { *symbolTable.getVariable("a") };
        PPL::Variable b { *symbolTable.getVariable("b") };

        Powerset expectedBrinkInterpretation { PPLUtils::powerset({{ b == 0, a >= 0 }, { a == 0, b >= 0 }}, 3) };

        auto [brink, brinkInterpretation] { brinkMay(polyhedralSystem) };

        INFO(PPLOutput::toString(*brinkInterpretation, polyhedralSystem->symbolTable()));
        REQUIRE(*brinkInterpretation == expectedBrinkInterpretation);
    }

    SECTION("Extend Poly system with brink")
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

        PolyhedralSystemSymbolTable symbolTable { polyhedralSystem->symbolTable() };
        PPL::Variable a { *symbolTable.getVariable("a") };
        PPL::Variable b { *symbolTable.getVariable("b") };
        Powerset brinkInterpretation { PPLUtils::powerset({{ b == 0, a >= 0 }, { a == 0, b >= 0 }}, 3) };
        polyhedralSystem = polyhedralSystem->extend(ap("brink"), brinkInterpretation);

        Powerset expectedBrinkInterpretation { PPLUtils::powerset({{ b == 0, a >= 0 }, { a == 0, b >= 0 }}, 3) };
        Powerset expectedNotBrinkInterpretation { PPLUtils::powerset({{ a > 0, b > 0 }}, 3) };

        const Powerset& brink { (*polyhedralSystem->getAtomInterpretation("brink"))->interpretation() };
        const Powerset& not_brink { (*polyhedralSystem->getAtomInterpretation("brink"))->notInterpretation() };

        REQUIRE(brink == expectedBrinkInterpretation);
        REQUIRE(not_brink == expectedNotBrinkInterpretation);
    }
}