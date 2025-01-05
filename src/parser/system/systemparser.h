#ifndef SYSTEMPARSER_H
#define SYSTEMPARSER_H

#include "PolyhedralSystemParsingResult.h"
#include <antlr4-runtime.h>

PolyhedralSystemParsingResult parsePolyhedralSystem(const std::string_view input);
PolyhedralSystemParsingResult parsePolyhedralSystem(std::istream& input);
PolyhedralSystemParsingResult parsePolyhedralSystem(antlr4::ANTLRInputStream* input);

#endif //SYSTEMPARSER_H
