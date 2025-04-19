#include <variant>

#include "pointparser.h"
#include "RationalPointLexer.h"
#include "RationalPointParser.h"
#include "ErrorListener.h"
#include "RationalPointBuilderVisitor.h"

RationalPointParsingResult parseRationalPoint(const std::string_view input, const SymbolTable& symbolTable)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseRationalPoint(&antlrInput, symbolTable);
}

RationalPointParsingResult parseRationalPoint(std::istream& input, const SymbolTable& symbolTable)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseRationalPoint(&antlrInput, symbolTable);
}

RationalPointParsingResult parseRationalPoint(antlr4::ANTLRInputStream* input, const SymbolTable& symbolTable)
{
    RationalPointLexer lexer { input };
    ErrorListener* errorListener { new ErrorListener {} };
    lexer.removeErrorListeners();
    lexer.addErrorListener(errorListener);
    antlr4::CommonTokenStream tokens { &lexer };
    RationalPointParser parser { &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(errorListener);
    RationalPointParser::ArrayContext* parseTree = parser.array();
    if (errorListener->hasErrors())
    {
        std::vector errors { errorListener->errors() };
        delete errorListener;
        return errors;
    }

    delete errorListener;
    RationalPointBuilderVisitor pointsMcBuilderVisitor { symbolTable };
    RationalPoint point { pointsMcBuilderVisitor.buildRationalPoint(parseTree) };
    if (pointsMcBuilderVisitor.hasErrors())
    {
        return pointsMcBuilderVisitor.errors();
    }

    return point;
}