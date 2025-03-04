#include <ObservablePatchSequence.h>
#include <observable_patch_cartesian_product.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <subprojects/spdlog-1.15.0/include/spdlog/fmt/bundled/format.h>
#include <subprojects/spdlog-1.15.0/include/spdlog/fmt/bundled/ranges.h>

#include "ppl_output.h"
#include "PolyhedralSystemParsingResult.h"
#include "spot_utils.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "systemparser.h"

using namespace PPLUtils;
using namespace SpotUtils;

bool allObservablesAreDisjoint(const std::vector<Observable>& observables);

TEST_CASE("Generate observables")
{
    SECTION("{p, q}")
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
        const Powerset& p { (*polyhedralSystem.getAtomInterpretation("p"))->interpretation() };
        const Powerset& q { (*polyhedralSystem.getAtomInterpretation("q"))->interpretation() };
        const Powerset& notP { (*polyhedralSystem.getAtomInterpretation("p"))->notInterpretation() };
        const Powerset& notQ { (*polyhedralSystem.getAtomInterpretation("q"))->notInterpretation() };

        SECTION("Without filtering empty observables")
        {
            constexpr bool filterEmptyObservables { false };
            std::vector observables { polyhedralSystem.generateObservables(filterEmptyObservables) };

            REQUIRE(observables.size() == 3);
            REQUIRE(allObservablesAreDisjoint(observables));
            REQUIRE_THAT(
                observables,
                Catch::Matchers::UnorderedEquals(
                    std::vector {
                        Observable { AP({"p"}), intersect(p, notQ), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q"}), intersect(notP, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q"}), intersect(p, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                    }
                )
            );
        }

        SECTION("Filtering empty observables")
        {
            constexpr bool filterEmptyObservables { true };
            std::vector observables { polyhedralSystem.generateObservables(filterEmptyObservables) };

            REQUIRE(observables.size() == 2);
            REQUIRE(allObservablesAreDisjoint(observables));
            REQUIRE_THAT(
                observables,
                Catch::Matchers::UnorderedEquals(
                    std::vector {
                        Observable { AP({"p"}), intersect(p, notQ), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q"}), intersect(notP, q), PPLOutput::toString(*intersect(p, notQ), polyhedralSystem.symbolTable()) },
                    }
                )
            );
        }
    }

    SECTION("{p, q, r, v}")
    {
        const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
            parsePolyhedralSystem(
                "p ( { X < 3 & Y <= 3 } { X < 3 & Y <= 10 } )"
                "q ( { X < 3 & Y <= 3 } { X > 10 & Y <= 2 } { Y <= 10 } { X = 3 } )"
                "r { X > 3 & Y >= 4 }"
                "v { X < 1 & Z < 1 }"
                "Inv ( { X + Y >= 3 & Y >= 4 } )"
                "Flow { X <= 4 }"
           )
        };
        REQUIRE(polyhedralSystemParsingResult.ok());
        const PolyhedralSystem& polyhedralSystem { *polyhedralSystemParsingResult };
        const Powerset& p { (*polyhedralSystem.getAtomInterpretation("p"))->interpretation() };
        const Powerset& q { (*polyhedralSystem.getAtomInterpretation("q"))->interpretation() };
        const Powerset& r { (*polyhedralSystem.getAtomInterpretation("r"))->interpretation() };
        const Powerset& v { (*polyhedralSystem.getAtomInterpretation("v"))->interpretation() };
        const Powerset& notP { (*polyhedralSystem.getAtomInterpretation("p"))->notInterpretation() };
        const Powerset& notQ { (*polyhedralSystem.getAtomInterpretation("q"))->notInterpretation() };
        const Powerset& notR { (*polyhedralSystem.getAtomInterpretation("r"))->notInterpretation() };
        const Powerset& notV { (*polyhedralSystem.getAtomInterpretation("v"))->notInterpretation() };

        SECTION("Without filtering empty observables")
        {
            constexpr bool filterEmptyObservables { false };
            std::vector observables { polyhedralSystem.generateObservables(filterEmptyObservables) };

            REQUIRE(observables.size() == 15);
            REQUIRE(allObservablesAreDisjoint(observables));
            REQUIRE_THAT(
                observables,
                Catch::Matchers::UnorderedEquals(
                    std::vector{
                        Observable { AP({"p"}), intersect({ p, notQ, notR, notV }),         PPLOutput::toString(*intersect({ p, notQ, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q"}), intersect({ notP, q, notR, notV }),         PPLOutput::toString(*intersect({ notP, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"r"}), intersect({ notP, notQ, r, notV }),         PPLOutput::toString(*intersect({ notP, notQ, r, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"v"}), intersect({ notP, notQ, notR, v }),         PPLOutput::toString(*intersect({ notP, notQ, notR, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q"}), intersect({ p, q, notR, notV }),       PPLOutput::toString(*intersect({ p, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "r"}), intersect({ p, notQ, r, notV }),       PPLOutput::toString(*intersect({ p, notQ, r, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "v"}), intersect({ p, notQ, notR, v }),       PPLOutput::toString(*intersect({ p, notQ, notR, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q", "v"}), intersect({ notP, q, notR, v }),       PPLOutput::toString(*intersect({ notP, q, notR, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q", "r"}), intersect({ r, q, notP, notV }),       PPLOutput::toString(*intersect({ r, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"r", "v"}), intersect({ notP, notQ, r, v }),       PPLOutput::toString(*intersect({ notP, notQ, r, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q", "r"}), intersect({ p, q, r, notV }),     PPLOutput::toString(*intersect({ p, q, r, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q", "v"}), intersect({ p, q, notR, v }),     PPLOutput::toString(*intersect({ p, q, notR, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "r", "v"}), intersect({ p, notQ, r, v }),     PPLOutput::toString(*intersect({ p, notQ, r, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q", "r", "v"}), intersect({ notP, q, r, v }),     PPLOutput::toString(*intersect({ notP, q, r, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q", "r", "v"}), intersect({ p, q, r, v }),   PPLOutput::toString(*intersect({ p, q, r, v }), polyhedralSystem.symbolTable()) },
                    }
                )
            );
        }

        SECTION("Filtering empty observables")
        {
            constexpr bool filterEmptyObservables { true };
            std::vector observables { polyhedralSystem.generateObservables(filterEmptyObservables) };

            REQUIRE(observables.size() == 6);
            REQUIRE(allObservablesAreDisjoint(observables));
            REQUIRE_THAT(
                observables,
                Catch::Matchers::UnorderedEquals(
                    std::vector{
                        Observable { AP({"q"}), intersect({ notP, q, notR, notV }),         PPLOutput::toString(*intersect({ notP, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"r"}), intersect({ notP, notQ, r, notV }),         PPLOutput::toString(*intersect({ notP, notQ, r, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"v"}), intersect({ notP, notQ, notR, v }),         PPLOutput::toString(*intersect({ notP, notQ, notR, v }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q"}), intersect({ p, q, notR, notV }),       PPLOutput::toString(*intersect({ p, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"q", "r"}), intersect({ r, q, notP, notV }),       PPLOutput::toString(*intersect({ r, q, notR, notV }), polyhedralSystem.symbolTable()) },
                        Observable { AP({"p", "q", "v"}), intersect({ p, q, notR, v }),     PPLOutput::toString(*intersect({ p, q, notR, v }), polyhedralSystem.symbolTable()) },
                    }
                )
            );
        }
    }
}

bool allObservablesAreDisjoint(const std::vector<Observable>& observables)
{
    for (const Observable& currentObservable: observables)
    {
        for (const Observable& otherObservable: observables)
        {
            if (currentObservable == otherObservable) continue;
            PowersetConstSharedPtr currentObservableInterpretation { currentObservable.interpretation() };
            PowersetConstSharedPtr otherObservableInterpretation { otherObservable.interpretation() };
            if (!currentObservableInterpretation->is_disjoint_from(*otherObservableInterpretation)) return false;
        }
    }

    return true;
}


TEST_CASE("Extract observable sequences (cartesian product)")
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
    std::vector observables { observable1, observable2, observable3 };

    std::vector sequences { observablePatchesCartesianProduct(observables) };
    REQUIRE(sequences.size() == 8);

    REQUIRE_THAT(
        sequences,
        Catch::Matchers::UnorderedEquals(
            std::vector<std::vector<ObservablePatch>> {
                { { observable1, observable1Patch1 }, { observable2, observable2Patch1 }, { observable3, observable3Patch1 } },
                { { observable1, observable1Patch1 }, { observable2, observable2Patch1 }, { observable3, observable3Patch2 } },
                { { observable1, observable1Patch1 }, { observable2, observable2Patch2 }, { observable3, observable3Patch1 } },
                { { observable1, observable1Patch1 }, { observable2, observable2Patch2 }, { observable3, observable3Patch2 } },
                { { observable1, observable1Patch2 }, { observable2, observable2Patch1 }, { observable3, observable3Patch1 } },
                { { observable1, observable1Patch2 }, { observable2, observable2Patch1 }, { observable3, observable3Patch2 } },
                { { observable1, observable1Patch2 }, { observable2, observable2Patch2 }, { observable3, observable3Patch1 } },
                { { observable1, observable1Patch2 }, { observable2, observable2Patch2 }, { observable3, observable3Patch2 } }
            }
        )
    );
}