#include <parsertlf.h>
#include <test_utils.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Correctly parse a RTLf formula", "[good]")
{
    SECTION("G(p0 & p1) & p2 & G p3 & F(p4 & F p5)")
    {
        std::string rtlf { "G(p0 & p1) & p2 & G p3 & F(p4 & F p5)" };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };

        REQUIRE(rtlfParsingResult.ok());
        REQUIRE(rtlfParsingResult.isLtl());
        REQUIRE(!rtlfParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlfParsingResult == spot::parse_infix_psl(rtlf).f);
    }

    SECTION("p0 U p1 & p2 S p3 & GF(p0 & p4 S p5)")
    {
        std::string rtlf { "p0 U p1 & p2 S p3 & GF(p0 & p4 S p5)" };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };

        REQUIRE(rtlfParsingResult.ok());
        REQUIRE(rtlfParsingResult.isLtl());
        REQUIRE(!rtlfParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlfParsingResult == spot::parse_infix_psl(rtlf).f);
    }

    SECTION("p0 W p1 & p2 S p3 & GF(p0 & p4 R p5) | p8 W (p9 W p10) R p11")
    {
        std::string rtlf { "p0 W p1 & p2 S p3 & GF(p0 & p4 R p5) | p8 W (p9 W p10) R p11" };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };

        REQUIRE(rtlfParsingResult.ok());
        REQUIRE(rtlfParsingResult.isLtl());
        REQUIRE(!rtlfParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlfParsingResult == spot::parse_infix_psl(rtlf).f);
    }

    SECTION("Parse a RTL formula read from a file (rtlf-test-1.txt)")
    {
        std::string rtlf { readTestFileAsString("rtlf-test-1.txt") };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };

        REQUIRE(rtlfParsingResult.ok());
        REQUIRE(rtlfParsingResult.isLtl());
        REQUIRE(!rtlfParsingResult.isUsingIllegalAtoms());
        REQUIRE(*rtlfParsingResult == spot::parse_infix_psl(rtlf).f);
    }
}

TEST_CASE("Correctly report syntax errors when parsing a RTLf formula", "[bad]")
{
    SECTION("p0 U")
    {
        std::string rtlf { "p0 U" };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };

        REQUIRE(!rtlfParsingResult); // == !rtlfParsingResult.ok()
        REQUIRE(rtlfParsingResult.hasParserErrors());
        REQUIRE(!rtlfParsingResult.parserErrors().empty());

        ParserError untilParserError { rtlfParsingResult.parserErrors()[1] };
        REQUIRE(untilParserError.errorMessage() == "missing right operand for \"until operator\"");
        REQUIRE(untilParserError.startLocation().line() == 1);
        REQUIRE(untilParserError.startLocation().charPositionInLine() == 3);
        REQUIRE(untilParserError.endLocation()->line() == 1);
        REQUIRE(untilParserError.endLocation()->charPositionInLine() == 4);
    }

    SECTION("Given AP={p0,p1,p2} then GF p0 & F!p1 & FGFG(p2 U !p3) should be illegal")
    {
        std::string rtlf { "GF p0 & F!p1 & FGFG(p2 U !p3)" };
        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf, { "p0", "p1", "p2" }) };

        REQUIRE(!rtlfParsingResult);
        REQUIRE(!rtlfParsingResult.hasParserErrors());
        REQUIRE(rtlfParsingResult.isLtl());
        REQUIRE(rtlfParsingResult.isUsingIllegalAtoms());
        REQUIRE(rtlfParsingResult.illegalAtoms().size() == 1);
        REQUIRE(rtlfParsingResult.illegalAtoms()[0] == SpotUtils::ap("p3"));
    }
}