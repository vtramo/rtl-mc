#pragma once

#include <antlr4-runtime.h>
#include <variant>

#include "PolyhedralSystemSymbolTable.h"
#include "ParserError.h"

using McPointParsingResult = std::variant<PPL::Generator, std::vector<ParserError>>;

McPointParsingResult parseMcPoint(std::string_view input, const PolyhedralSystemSymbolTable& symbolTable);
McPointParsingResult parseMcPoint(std::istream& input, const PolyhedralSystemSymbolTable& symbolTable);
McPointParsingResult parseMcPoint(antlr4::ANTLRInputStream* input, const PolyhedralSystemSymbolTable& symbolTable);