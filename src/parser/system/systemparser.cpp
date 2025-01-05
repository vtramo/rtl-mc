#include "systemparser.h"
#include "PolyhedralSystemBuilderVisitor.h"
#include "PolyhedralSystemLexer.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemParsingResult.h"
#include "PolyhedralSystemSymbolTableListener.h"
#include "PolyhedralSystemErrorListener.h"

PolyhedralSystemParsingResult parsePolyhedralSystem(const std::string_view input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parsePolyhedralSystem(&antlrInput);
}

PolyhedralSystemParsingResult parsePolyhedralSystem(std::istream& input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parsePolyhedralSystem(&antlrInput);
}

PolyhedralSystemParsingResult parsePolyhedralSystem(antlr4::ANTLRInputStream* input)
{
    PolyhedralSystemLexer lexer { input };
    PolyhedralSystemErrorListener* errorListener { new PolyhedralSystemErrorListener {} };
    lexer.removeErrorListeners();
    lexer.addErrorListener(errorListener);
    antlr4::CommonTokenStream tokens { &lexer };
    PolyhedralSystemParser parser { &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(errorListener);
    PolyhedralSystemParser::SystemContext* parseTree = parser.system();
    if (errorListener->hasErrors())
    {
        std::vector errors { errorListener->getErrors() };
        delete errorListener;
        return PolyhedralSystemParsingResult { errors };
    }

    delete errorListener;
    antlr4::tree::ParseTreeWalker walker {};
    PolyhedralSystemSymbolTableListener symbolTableListener {};
    walker.walk(&symbolTableListener, parseTree);
    PolyhedralSystemSymbolTable symbolTable { symbolTableListener.getSymbolTable() };
    PolyhedralSystemBuilderVisitor polyhedralSystemBuilderVisitor { symbolTable };
    return PolyhedralSystemParsingResult { polyhedralSystemBuilderVisitor.buildPolyhedralSystem(parseTree) };
}