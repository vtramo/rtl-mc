#pragma once

#include <variant>
#include <vector>
#include <antlr4-runtime.h>
#include "GeogebraPatch.h"
#include "PolyhedraLexer.h"
#include "SymbolTableListener.h"
#include "SimpleSymbolTable.h"
#include "ErrorListener.h"
#include "GeogebraPatchGenerator.h"
#include "ParserError.h"

using GeogebraParsingSuccess = std::pair<std::vector<GeogebraPatch>, SimpleSymbolTable>;
using GeogebraPolyhedraParsingResult =
    std::variant<
        std::pair<std::vector<GeogebraPatch>, SimpleSymbolTable>,
        std::vector<ParserError>
    >;

GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(const std::string_view input);
GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(std::istream& input);
GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(antlr4::ANTLRInputStream* input);

inline GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(const std::string_view input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseGeogebraPolyhedra(&antlrInput);
}

inline GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(std::istream& input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parseGeogebraPolyhedra(&antlrInput);
}

inline GeogebraPolyhedraParsingResult parseGeogebraPolyhedra(antlr4::ANTLRInputStream* input)
{
    PolyhedraLexer lexer { input };
    ErrorListener* errorListener { new ErrorListener {} };
    lexer.removeErrorListeners();
    lexer.addErrorListener(errorListener);
    antlr4::CommonTokenStream tokens { &lexer };
    PolyhedraParser parser { &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(errorListener);
    PolyhedraParser::PolyhedraContext* parseTree = parser.polyhedra();
    if (errorListener->hasErrors())
    {
        std::vector<ParserError> errors { errorListener->errors() };
        delete errorListener;
        return errors;
    }

    delete errorListener;
    antlr4::tree::ParseTreeWalker walker {};
    SymbolTableListener symbolTableListener {};
    walker.walk(&symbolTableListener, parseTree);

    SimpleSymbolTable symbolTable { symbolTableListener.symbolTable() };
    GeogebraPatchGenerator geogebraPatchGenerator { symbolTable };
    std::vector<GeogebraPatch> geogebraPatches { geogebraPatchGenerator.generateGeogebraPatches(parseTree) };
    if (geogebraPatchGenerator.hasErrors())
    {
        return geogebraPatchGenerator.errors();
    }

    return std::make_pair<std::vector<GeogebraPatch>, SimpleSymbolTable>(std::move(geogebraPatches), std::move(symbolTable));
}