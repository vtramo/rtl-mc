#pragma once

#include "PolyhedralSystemParsingResult.h"
#include <antlr4-runtime.h>

PolyhedralSystemParsingResult parsePolyhedralSystem(std::string_view input);
PolyhedralSystemParsingResult parsePolyhedralSystem(std::istream& input);
PolyhedralSystemParsingResult parsePolyhedralSystem(antlr4::ANTLRInputStream* input);