#include "mcparser.h"

#include "McPointLexer.h"
#include "McPointParser.h"
#include "ErrorListener.h"
#include "McPointBuilderVisitor.h"

McPointParsingResult parseMcPoint(const std::string_view input, const PolyhedralSystemSymbolTable& symbolTable)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseMcPoint(&antlrInput, symbolTable);
}

McPointParsingResult parseMcPoint(std::istream& input, const PolyhedralSystemSymbolTable& symbolTable)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseMcPoint(&antlrInput, symbolTable);
}

McPointParsingResult parseMcPoint(antlr4::ANTLRInputStream* input, const PolyhedralSystemSymbolTable& symbolTable)
{
    McPointLexer lexer { input };
    ErrorListener* errorListener { new ErrorListener {} };
    lexer.removeErrorListeners();
    lexer.addErrorListener(errorListener);
    antlr4::CommonTokenStream tokens { &lexer };
    McPointParser parser { &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(errorListener);
    McPointParser::ArrayContext* parseTree = parser.array();
    if (errorListener->hasErrors())
    {
        std::vector errors { errorListener->errors() };
        delete errorListener;
        return errors;
    }

    delete errorListener;
    McPointBuilderVisitor pointsMcBuilderVisitor { symbolTable };
    PPL::Generator point { pointsMcBuilderVisitor.buildMcPoint(parseTree) };
    if (pointsMcBuilderVisitor.hasErrors())
        return pointsMcBuilderVisitor.errors();

    return point;
}