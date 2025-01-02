#include <parsertlf.h>
#include <parser-dev/parser_dev.h>
#include <spot/tl/print.hh>

int main()
{
    RtlfParsingResult rtlfParsingResult { parseRtlf("p0 U") };
    assert(!rtlfParsingResult.ok());
    std::vector<ParserError> parserErrors { rtlfParsingResult.parserErrors() };

    for (auto parserError: parserErrors)
    {
        PositionError startError { parserError.startLocation() };
        std::optional<PositionError> endError { parserError.endLocation() };

        std::cout << "Error msg: " << parserError.errorMessage() << '\n';
        std::cout << "Start line: " << startError.line() << ", Position: " << startError.charPositionInLine() << "\n\n";
        if (endError)
        {
            std::cout << "End line: " << endError->line() << ", Position: " << endError->charPositionInLine() << "\n\n";
        }
    }
    // assert(*rtlfParsingResult);
    // spot::print_psl(std::cout, *rtlfParsingResult);
    // parseFormulaRtlf();
    // parsePolySystem();
}
