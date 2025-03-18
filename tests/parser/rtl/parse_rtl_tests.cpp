#include "parsertl.h"
#include "test_utils.h"
#include "formula.h"
#include "RtlParsingResult.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Correctly parse a RTL formula", "[good]")
{
    SECTION("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)")
    {
        std::string rtl { "G(p0 & p1) & p2 & G p3 & F(p4 & F p5)" };
        RtlParsingResult rtlParsingResult { parseRtl(rtl) };

        REQUIRE(rtlParsingResult.ok());
        REQUIRE(rtlParsingResult.isLtl());
        REQUIRE(!rtlParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlParsingResult == spot::parse_infix_psl(rtl).f);
    }

    SECTION("Parse a RTL formula read from a file (rtl-test-1.txt)")
    {
        std::string rtl { readTestFileAsString("rtl-test-1.txt") };
        RtlParsingResult rtlParsingResult { parseRtl(rtl) };

        REQUIRE(rtlParsingResult.ok());
        REQUIRE(rtlParsingResult.isLtl());
        REQUIRE(!rtlParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlParsingResult == spot::parse_infix_psl(rtl).f);
    }
}

TEST_CASE("Correctly report syntax errors when parsing a RTL formula", "[bad]")
{
    SECTION("p0 U")
    {
        std::string rtl { "p0 U" };
        RtlParsingResult rtlParsingResult { parseRtl(rtl) };

        REQUIRE(!rtlParsingResult); // == !rtlParsingResult.ok()
        REQUIRE(rtlParsingResult.hasParserErrors());
        REQUIRE(!rtlParsingResult.parserErrors().empty());

        ParserError untilParserError { rtlParsingResult.parserErrors()[1] };
        REQUIRE(untilParserError.errorMessage() == "missing right operand for \"until operator\"");
        REQUIRE(untilParserError.startLocation().line() == 1);
        REQUIRE(untilParserError.startLocation().charPositionInLine() == 3);
        REQUIRE(untilParserError.endLocation()->line() == 1);
        REQUIRE(untilParserError.endLocation()->charPositionInLine() == 4);
    }

    SECTION("Given AP={p0,p1,p2} then GF p0 & F!p1 & FGFG(p2 U !p3) should be illegal")
    {
        std::string rtl { "G(F(p0)) & F(!p1) & F(G(F(G(p2 U !p3))))" };
        spot::atomic_prop_set allowedAtomicProp { AP({ "p0", "p1", "p2" }) };
        RtlParsingResult rtlParsingResult { parseRtl(rtl, allowedAtomicProp) };

        REQUIRE(!rtlParsingResult);
        REQUIRE(!rtlParsingResult.hasParserErrors());
        REQUIRE(rtlParsingResult.isLtl());
        REQUIRE(rtlParsingResult.isUsingIllegalAtoms());
        REQUIRE(rtlParsingResult.illegalAtoms().size() == 1);
        REQUIRE(rtlParsingResult.illegalAtoms()[0] == ap("p3"));
    }
}