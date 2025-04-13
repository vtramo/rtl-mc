
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2


#include "PolyhedraListener.h"
#include "PolyhedraVisitor.h"

#include "PolyhedraParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct PolyhedraParserStaticData final {
  PolyhedraParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PolyhedraParserStaticData(const PolyhedraParserStaticData&) = delete;
  PolyhedraParserStaticData(PolyhedraParserStaticData&&) = delete;
  PolyhedraParserStaticData& operator=(const PolyhedraParserStaticData&) = delete;
  PolyhedraParserStaticData& operator=(PolyhedraParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag polyhedraParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<PolyhedraParserStaticData> polyhedraParserStaticData = nullptr;

void polyhedraParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (polyhedraParserStaticData != nullptr) {
    return;
  }
#else
  assert(polyhedraParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PolyhedraParserStaticData>(
    std::vector<std::string>{
      "polyhedra", "atom", "powerset", "poly", "constr", "linearExpr", "signTerm", 
      "term"
    },
    std::vector<std::string>{
      "", "'{'", "'}'", "'('", "')'", "'true'", "'&'", "", "", "", "", "", 
      "", "'*'", "'+'", "'-'", "'<='", "'<'", "'>'", "'>='", "'='", "'0'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "LINE_COMMENT", "COMMENT", "WS", "UNSIGNED_INT", 
      "NEWLINE", "FLOAT", "TIMES", "PLUS", "MINUS", "LE", "LT", "GT", "GE", 
      "EQ", "ZERO", "FLOW", "INV", "VARID", "HEX_COLOR_CODE"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,25,363,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,1,0,4,0,18,8,0,11,0,12,0,19,1,0,5,0,23,8,0,10,0,12,0,26,9,0,1,0,
  	3,0,29,8,0,1,1,1,1,1,1,3,1,34,8,1,1,1,1,1,1,1,1,1,3,1,40,8,1,1,1,1,1,
  	1,1,3,1,45,8,1,1,1,1,1,1,1,1,1,4,1,51,8,1,11,1,12,1,52,1,1,1,1,1,1,4,
  	1,58,8,1,11,1,12,1,59,1,1,1,1,1,1,1,1,1,1,4,1,67,8,1,11,1,12,1,68,1,1,
  	1,1,1,1,4,1,74,8,1,11,1,12,1,75,1,1,1,1,1,1,4,1,81,8,1,11,1,12,1,82,1,
  	1,1,1,3,1,87,8,1,1,2,1,2,5,2,91,8,2,10,2,12,2,94,9,2,1,2,1,2,1,2,1,2,
  	1,2,1,2,5,2,102,8,2,10,2,12,2,105,9,2,1,2,1,2,4,2,109,8,2,11,2,12,2,110,
  	1,2,1,2,4,2,115,8,2,11,2,12,2,116,1,2,1,2,5,2,121,8,2,10,2,12,2,124,9,
  	2,1,2,1,2,4,2,128,8,2,11,2,12,2,129,1,2,1,2,4,2,134,8,2,11,2,12,2,135,
  	1,2,1,2,5,2,140,8,2,10,2,12,2,143,9,2,1,2,4,2,146,8,2,11,2,12,2,147,1,
  	2,1,2,1,2,5,2,153,8,2,10,2,12,2,156,9,2,1,2,1,2,5,2,160,8,2,10,2,12,2,
  	163,9,2,1,2,1,2,3,2,167,8,2,1,3,1,3,1,3,1,3,5,3,173,8,3,10,3,12,3,176,
  	9,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,187,8,3,10,3,12,3,190,9,3,
  	1,3,1,3,4,3,194,8,3,11,3,12,3,195,1,3,1,3,1,3,4,3,201,8,3,11,3,12,3,202,
  	1,3,1,3,1,3,1,3,5,3,209,8,3,10,3,12,3,212,9,3,1,3,1,3,4,3,216,8,3,11,
  	3,12,3,217,1,3,1,3,1,3,4,3,223,8,3,11,3,12,3,224,1,3,1,3,1,3,1,3,5,3,
  	231,8,3,10,3,12,3,234,9,3,1,3,4,3,237,8,3,11,3,12,3,238,1,3,1,3,1,3,1,
  	3,1,3,5,3,246,8,3,10,3,12,3,249,9,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,257,8,
  	3,10,3,12,3,260,9,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,270,8,3,10,3,
  	12,3,273,9,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,283,8,3,10,3,12,3,286,
  	9,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,297,8,3,10,3,12,3,300,9,3,
  	1,3,1,3,1,3,3,3,305,8,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,
  	4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,3,4,327,8,4,1,5,3,5,330,8,5,1,5,1,5,
  	5,5,334,8,5,10,5,12,5,337,9,5,1,6,1,6,1,6,1,7,1,7,3,7,344,8,7,1,7,1,7,
  	1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,357,8,7,1,7,1,7,3,7,361,8,7,1,
  	7,0,0,8,0,2,4,6,8,10,12,14,0,3,1,0,16,20,1,0,14,15,1,0,13,13,424,0,28,
  	1,0,0,0,2,86,1,0,0,0,4,166,1,0,0,0,6,304,1,0,0,0,8,326,1,0,0,0,10,329,
  	1,0,0,0,12,338,1,0,0,0,14,360,1,0,0,0,16,18,3,2,1,0,17,16,1,0,0,0,18,
  	19,1,0,0,0,19,17,1,0,0,0,19,20,1,0,0,0,20,29,1,0,0,0,21,23,5,9,0,0,22,
  	21,1,0,0,0,23,26,1,0,0,0,24,22,1,0,0,0,24,25,1,0,0,0,25,27,1,0,0,0,26,
  	24,1,0,0,0,27,29,6,0,-1,0,28,17,1,0,0,0,28,24,1,0,0,0,29,1,1,0,0,0,30,
  	31,5,24,0,0,31,33,3,4,2,0,32,34,5,25,0,0,33,32,1,0,0,0,33,34,1,0,0,0,
  	34,87,1,0,0,0,35,36,5,24,0,0,36,37,5,1,0,0,37,39,5,2,0,0,38,40,5,25,0,
  	0,39,38,1,0,0,0,39,40,1,0,0,0,40,87,1,0,0,0,41,42,5,24,0,0,42,44,3,6,
  	3,0,43,45,5,25,0,0,44,43,1,0,0,0,44,45,1,0,0,0,45,87,1,0,0,0,46,47,5,
  	24,0,0,47,48,5,1,0,0,48,50,5,2,0,0,49,51,5,2,0,0,50,49,1,0,0,0,51,52,
  	1,0,0,0,52,50,1,0,0,0,52,53,1,0,0,0,53,54,1,0,0,0,54,87,6,1,-1,0,55,57,
  	5,24,0,0,56,58,5,1,0,0,57,56,1,0,0,0,58,59,1,0,0,0,59,57,1,0,0,0,59,60,
  	1,0,0,0,60,61,1,0,0,0,61,62,5,1,0,0,62,63,5,2,0,0,63,87,6,1,-1,0,64,66,
  	5,24,0,0,65,67,5,1,0,0,66,65,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,
  	1,0,0,0,69,70,1,0,0,0,70,71,5,1,0,0,71,73,5,2,0,0,72,74,5,2,0,0,73,72,
  	1,0,0,0,74,75,1,0,0,0,75,73,1,0,0,0,75,76,1,0,0,0,76,77,1,0,0,0,77,87,
  	6,1,-1,0,78,80,5,24,0,0,79,81,3,6,3,0,80,79,1,0,0,0,81,82,1,0,0,0,82,
  	80,1,0,0,0,82,83,1,0,0,0,83,84,1,0,0,0,84,85,6,1,-1,0,85,87,1,0,0,0,86,
  	30,1,0,0,0,86,35,1,0,0,0,86,41,1,0,0,0,86,46,1,0,0,0,86,55,1,0,0,0,86,
  	64,1,0,0,0,86,78,1,0,0,0,87,3,1,0,0,0,88,92,5,3,0,0,89,91,3,6,3,0,90,
  	89,1,0,0,0,91,94,1,0,0,0,92,90,1,0,0,0,92,93,1,0,0,0,93,95,1,0,0,0,94,
  	92,1,0,0,0,95,167,5,4,0,0,96,97,5,3,0,0,97,98,5,5,0,0,98,167,5,4,0,0,
  	99,103,5,3,0,0,100,102,3,6,3,0,101,100,1,0,0,0,102,105,1,0,0,0,103,101,
  	1,0,0,0,103,104,1,0,0,0,104,106,1,0,0,0,105,103,1,0,0,0,106,108,5,4,0,
  	0,107,109,5,4,0,0,108,107,1,0,0,0,109,110,1,0,0,0,110,108,1,0,0,0,110,
  	111,1,0,0,0,111,112,1,0,0,0,112,167,6,2,-1,0,113,115,5,3,0,0,114,113,
  	1,0,0,0,115,116,1,0,0,0,116,114,1,0,0,0,116,117,1,0,0,0,117,118,1,0,0,
  	0,118,122,5,3,0,0,119,121,3,6,3,0,120,119,1,0,0,0,121,124,1,0,0,0,122,
  	120,1,0,0,0,122,123,1,0,0,0,123,125,1,0,0,0,124,122,1,0,0,0,125,127,5,
  	4,0,0,126,128,5,4,0,0,127,126,1,0,0,0,128,129,1,0,0,0,129,127,1,0,0,0,
  	129,130,1,0,0,0,130,131,1,0,0,0,131,167,6,2,-1,0,132,134,5,3,0,0,133,
  	132,1,0,0,0,134,135,1,0,0,0,135,133,1,0,0,0,135,136,1,0,0,0,136,137,1,
  	0,0,0,137,141,5,3,0,0,138,140,3,6,3,0,139,138,1,0,0,0,140,143,1,0,0,0,
  	141,139,1,0,0,0,141,142,1,0,0,0,142,145,1,0,0,0,143,141,1,0,0,0,144,146,
  	5,4,0,0,145,144,1,0,0,0,146,147,1,0,0,0,147,145,1,0,0,0,147,148,1,0,0,
  	0,148,149,1,0,0,0,149,167,6,2,-1,0,150,154,5,3,0,0,151,153,3,6,3,0,152,
  	151,1,0,0,0,153,156,1,0,0,0,154,152,1,0,0,0,154,155,1,0,0,0,155,157,1,
  	0,0,0,156,154,1,0,0,0,157,167,6,2,-1,0,158,160,3,6,3,0,159,158,1,0,0,
  	0,160,163,1,0,0,0,161,159,1,0,0,0,161,162,1,0,0,0,162,164,1,0,0,0,163,
  	161,1,0,0,0,164,165,5,4,0,0,165,167,6,2,-1,0,166,88,1,0,0,0,166,96,1,
  	0,0,0,166,99,1,0,0,0,166,114,1,0,0,0,166,133,1,0,0,0,166,150,1,0,0,0,
  	166,161,1,0,0,0,167,5,1,0,0,0,168,169,5,1,0,0,169,174,3,8,4,0,170,171,
  	5,6,0,0,171,173,3,8,4,0,172,170,1,0,0,0,173,176,1,0,0,0,174,172,1,0,0,
  	0,174,175,1,0,0,0,175,177,1,0,0,0,176,174,1,0,0,0,177,178,5,2,0,0,178,
  	305,1,0,0,0,179,180,5,1,0,0,180,181,5,5,0,0,181,305,5,2,0,0,182,183,5,
  	1,0,0,183,188,3,8,4,0,184,185,5,6,0,0,185,187,3,8,4,0,186,184,1,0,0,0,
  	187,190,1,0,0,0,188,186,1,0,0,0,188,189,1,0,0,0,189,191,1,0,0,0,190,188,
  	1,0,0,0,191,193,5,2,0,0,192,194,5,2,0,0,193,192,1,0,0,0,194,195,1,0,0,
  	0,195,193,1,0,0,0,195,196,1,0,0,0,196,197,1,0,0,0,197,198,6,3,-1,0,198,
  	305,1,0,0,0,199,201,5,1,0,0,200,199,1,0,0,0,201,202,1,0,0,0,202,200,1,
  	0,0,0,202,203,1,0,0,0,203,204,1,0,0,0,204,205,5,1,0,0,205,210,3,8,4,0,
  	206,207,5,6,0,0,207,209,3,8,4,0,208,206,1,0,0,0,209,212,1,0,0,0,210,208,
  	1,0,0,0,210,211,1,0,0,0,211,213,1,0,0,0,212,210,1,0,0,0,213,215,5,2,0,
  	0,214,216,5,2,0,0,215,214,1,0,0,0,216,217,1,0,0,0,217,215,1,0,0,0,217,
  	218,1,0,0,0,218,219,1,0,0,0,219,220,6,3,-1,0,220,305,1,0,0,0,221,223,
  	5,1,0,0,222,221,1,0,0,0,223,224,1,0,0,0,224,222,1,0,0,0,224,225,1,0,0,
  	0,225,226,1,0,0,0,226,227,5,1,0,0,227,232,3,8,4,0,228,229,5,6,0,0,229,
  	231,3,8,4,0,230,228,1,0,0,0,231,234,1,0,0,0,232,230,1,0,0,0,232,233,1,
  	0,0,0,233,236,1,0,0,0,234,232,1,0,0,0,235,237,5,2,0,0,236,235,1,0,0,0,
  	237,238,1,0,0,0,238,236,1,0,0,0,238,239,1,0,0,0,239,240,1,0,0,0,240,241,
  	6,3,-1,0,241,305,1,0,0,0,242,247,3,8,4,0,243,244,5,6,0,0,244,246,3,8,
  	4,0,245,243,1,0,0,0,246,249,1,0,0,0,247,245,1,0,0,0,247,248,1,0,0,0,248,
  	250,1,0,0,0,249,247,1,0,0,0,250,251,5,2,0,0,251,252,6,3,-1,0,252,305,
  	1,0,0,0,253,258,3,8,4,0,254,255,5,6,0,0,255,257,3,8,4,0,256,254,1,0,0,
  	0,257,260,1,0,0,0,258,256,1,0,0,0,258,259,1,0,0,0,259,261,1,0,0,0,260,
  	258,1,0,0,0,261,262,5,2,0,0,262,263,6,3,-1,0,263,305,1,0,0,0,264,271,
  	5,1,0,0,265,266,3,8,4,0,266,267,5,6,0,0,267,268,3,8,4,0,268,270,1,0,0,
  	0,269,265,1,0,0,0,270,273,1,0,0,0,271,269,1,0,0,0,271,272,1,0,0,0,272,
  	274,1,0,0,0,273,271,1,0,0,0,274,275,3,8,4,0,275,276,5,2,0,0,276,277,6,
  	3,-1,0,277,305,1,0,0,0,278,279,5,1,0,0,279,284,3,8,4,0,280,281,5,6,0,
  	0,281,283,3,8,4,0,282,280,1,0,0,0,283,286,1,0,0,0,284,282,1,0,0,0,284,
  	285,1,0,0,0,285,287,1,0,0,0,286,284,1,0,0,0,287,288,5,6,0,0,288,289,5,
  	2,0,0,289,290,6,3,-1,0,290,305,1,0,0,0,291,292,5,1,0,0,292,293,5,6,0,
  	0,293,298,3,8,4,0,294,295,5,6,0,0,295,297,3,8,4,0,296,294,1,0,0,0,297,
  	300,1,0,0,0,298,296,1,0,0,0,298,299,1,0,0,0,299,301,1,0,0,0,300,298,1,
  	0,0,0,301,302,5,2,0,0,302,303,6,3,-1,0,303,305,1,0,0,0,304,168,1,0,0,
  	0,304,179,1,0,0,0,304,182,1,0,0,0,304,200,1,0,0,0,304,222,1,0,0,0,304,
  	242,1,0,0,0,304,253,1,0,0,0,304,264,1,0,0,0,304,278,1,0,0,0,304,291,1,
  	0,0,0,305,7,1,0,0,0,306,307,3,10,5,0,307,308,7,0,0,0,308,309,3,10,5,0,
  	309,327,1,0,0,0,310,311,3,10,5,0,311,312,3,10,5,0,312,313,6,4,-1,0,313,
  	327,1,0,0,0,314,315,3,10,5,0,315,316,7,0,0,0,316,317,6,4,-1,0,317,327,
  	1,0,0,0,318,319,7,0,0,0,319,320,3,10,5,0,320,321,6,4,-1,0,321,327,1,0,
  	0,0,322,323,3,10,5,0,323,324,8,0,0,0,324,325,6,4,-1,0,325,327,1,0,0,0,
  	326,306,1,0,0,0,326,310,1,0,0,0,326,314,1,0,0,0,326,318,1,0,0,0,326,322,
  	1,0,0,0,327,9,1,0,0,0,328,330,7,1,0,0,329,328,1,0,0,0,329,330,1,0,0,0,
  	330,331,1,0,0,0,331,335,3,14,7,0,332,334,3,12,6,0,333,332,1,0,0,0,334,
  	337,1,0,0,0,335,333,1,0,0,0,335,336,1,0,0,0,336,11,1,0,0,0,337,335,1,
  	0,0,0,338,339,7,1,0,0,339,340,3,14,7,0,340,13,1,0,0,0,341,343,5,10,0,
  	0,342,344,5,13,0,0,343,342,1,0,0,0,343,344,1,0,0,0,344,345,1,0,0,0,345,
  	361,5,24,0,0,346,361,5,24,0,0,347,361,5,10,0,0,348,349,5,10,0,0,349,350,
  	8,2,0,0,350,351,5,24,0,0,351,361,6,7,-1,0,352,353,5,12,0,0,353,361,6,
  	7,-1,0,354,356,5,12,0,0,355,357,5,13,0,0,356,355,1,0,0,0,356,357,1,0,
  	0,0,357,358,1,0,0,0,358,359,5,24,0,0,359,361,6,7,-1,0,360,341,1,0,0,0,
  	360,346,1,0,0,0,360,347,1,0,0,0,360,348,1,0,0,0,360,352,1,0,0,0,360,354,
  	1,0,0,0,361,15,1,0,0,0,45,19,24,28,33,39,44,52,59,68,75,82,86,92,103,
  	110,116,122,129,135,141,147,154,161,166,174,188,195,202,210,217,224,232,
  	238,247,258,271,284,298,304,326,329,335,343,356,360
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  polyhedraParserStaticData = std::move(staticData);
}

}

PolyhedraParser::PolyhedraParser(TokenStream *input) : PolyhedraParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

PolyhedraParser::PolyhedraParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  PolyhedraParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *polyhedraParserStaticData->atn, polyhedraParserStaticData->decisionToDFA, polyhedraParserStaticData->sharedContextCache, options);
}

PolyhedraParser::~PolyhedraParser() {
  delete _interpreter;
}

const atn::ATN& PolyhedraParser::getATN() const {
  return *polyhedraParserStaticData->atn;
}

std::string PolyhedraParser::getGrammarFileName() const {
  return "Polyhedra.g4";
}

const std::vector<std::string>& PolyhedraParser::getRuleNames() const {
  return polyhedraParserStaticData->ruleNames;
}

const dfa::Vocabulary& PolyhedraParser::getVocabulary() const {
  return polyhedraParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView PolyhedraParser::getSerializedATN() const {
  return polyhedraParserStaticData->serializedATN;
}


//----------------- PolyhedraContext ------------------------------------------------------------------

PolyhedraParser::PolyhedraContext::PolyhedraContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PolyhedraParser::AtomContext *> PolyhedraParser::PolyhedraContext::atom() {
  return getRuleContexts<PolyhedraParser::AtomContext>();
}

PolyhedraParser::AtomContext* PolyhedraParser::PolyhedraContext::atom(size_t i) {
  return getRuleContext<PolyhedraParser::AtomContext>(i);
}

std::vector<tree::TerminalNode *> PolyhedraParser::PolyhedraContext::WS() {
  return getTokens(PolyhedraParser::WS);
}

tree::TerminalNode* PolyhedraParser::PolyhedraContext::WS(size_t i) {
  return getToken(PolyhedraParser::WS, i);
}


size_t PolyhedraParser::PolyhedraContext::getRuleIndex() const {
  return PolyhedraParser::RulePolyhedra;
}

void PolyhedraParser::PolyhedraContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPolyhedra(this);
}

void PolyhedraParser::PolyhedraContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPolyhedra(this);
}


std::any PolyhedraParser::PolyhedraContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPolyhedra(this);
  else
    return visitor->visitChildren(this);
}

PolyhedraParser::PolyhedraContext* PolyhedraParser::polyhedra() {
  PolyhedraContext *_localctx = _tracker.createInstance<PolyhedraContext>(_ctx, getState());
  enterRule(_localctx, 0, PolyhedraParser::RulePolyhedra);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(28);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PolyhedraParser::VARID: {
        enterOuterAlt(_localctx, 1);
        setState(17); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(16);
          atom();
          setState(19); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == PolyhedraParser::VARID);
        break;
      }

      case PolyhedraParser::EOF:
      case PolyhedraParser::WS: {
        enterOuterAlt(_localctx, 2);
        setState(24);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == PolyhedraParser::WS) {
          setState(21);
          match(PolyhedraParser::WS);
          setState(26);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
         notifyErrorListeners("Empty input!"); 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

PolyhedraParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedraParser::AtomContext::getRuleIndex() const {
  return PolyhedraParser::RuleAtom;
}

void PolyhedraParser::AtomContext::copyFrom(AtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AtomEmptyContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::AtomEmptyContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

tree::TerminalNode* PolyhedraParser::AtomEmptyContext::HEX_COLOR_CODE() {
  return getToken(PolyhedraParser::HEX_COLOR_CODE, 0);
}

PolyhedraParser::AtomEmptyContext::AtomEmptyContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::AtomEmptyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomEmpty(this);
}
void PolyhedraParser::AtomEmptyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomEmpty(this);
}

std::any PolyhedraParser::AtomEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitAtomEmpty(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomPolyContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::AtomPolyContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

PolyhedraParser::PolyContext* PolyhedraParser::AtomPolyContext::poly() {
  return getRuleContext<PolyhedraParser::PolyContext>(0);
}

tree::TerminalNode* PolyhedraParser::AtomPolyContext::HEX_COLOR_CODE() {
  return getToken(PolyhedraParser::HEX_COLOR_CODE, 0);
}

PolyhedraParser::AtomPolyContext::AtomPolyContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::AtomPolyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomPoly(this);
}
void PolyhedraParser::AtomPolyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomPoly(this);
}

std::any PolyhedraParser::AtomPolyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitAtomPoly(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomErrorContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::AtomErrorContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

std::vector<PolyhedraParser::PolyContext *> PolyhedraParser::AtomErrorContext::poly() {
  return getRuleContexts<PolyhedraParser::PolyContext>();
}

PolyhedraParser::PolyContext* PolyhedraParser::AtomErrorContext::poly(size_t i) {
  return getRuleContext<PolyhedraParser::PolyContext>(i);
}

PolyhedraParser::AtomErrorContext::AtomErrorContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::AtomErrorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomError(this);
}
void PolyhedraParser::AtomErrorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomError(this);
}

std::any PolyhedraParser::AtomErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitAtomError(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomPowersetContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::AtomPowersetContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

PolyhedraParser::PowersetContext* PolyhedraParser::AtomPowersetContext::powerset() {
  return getRuleContext<PolyhedraParser::PowersetContext>(0);
}

tree::TerminalNode* PolyhedraParser::AtomPowersetContext::HEX_COLOR_CODE() {
  return getToken(PolyhedraParser::HEX_COLOR_CODE, 0);
}

PolyhedraParser::AtomPowersetContext::AtomPowersetContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::AtomPowersetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomPowerset(this);
}
void PolyhedraParser::AtomPowersetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomPowerset(this);
}

std::any PolyhedraParser::AtomPowersetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitAtomPowerset(this);
  else
    return visitor->visitChildren(this);
}
PolyhedraParser::AtomContext* PolyhedraParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 2, PolyhedraParser::RuleAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(86);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomPowersetContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(30);
      match(PolyhedraParser::VARID);
      setState(31);
      powerset();
      setState(33);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedraParser::HEX_COLOR_CODE) {
        setState(32);
        match(PolyhedraParser::HEX_COLOR_CODE);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomEmptyContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(35);
      match(PolyhedraParser::VARID);
      setState(36);
      match(PolyhedraParser::T__0);
      setState(37);
      match(PolyhedraParser::T__1);
      setState(39);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedraParser::HEX_COLOR_CODE) {
        setState(38);
        match(PolyhedraParser::HEX_COLOR_CODE);
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomPolyContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(41);
      match(PolyhedraParser::VARID);
      setState(42);
      poly();
      setState(44);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedraParser::HEX_COLOR_CODE) {
        setState(43);
        match(PolyhedraParser::HEX_COLOR_CODE);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomErrorContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(46);
      match(PolyhedraParser::VARID);
      setState(47);
      match(PolyhedraParser::T__0);
      setState(48);
      match(PolyhedraParser::T__1);
      setState(50); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(49);
        match(PolyhedraParser::T__1);
        setState(52); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomErrorContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(55);
      match(PolyhedraParser::VARID);
      setState(57); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(56);
                match(PolyhedraParser::T__0);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(59); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(61);
      match(PolyhedraParser::T__0);
      setState(62);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomErrorContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(64);
      match(PolyhedraParser::VARID);
      setState(66); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(65);
                match(PolyhedraParser::T__0);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(68); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(70);
      match(PolyhedraParser::T__0);
      setState(71);
      match(PolyhedraParser::T__1);
      setState(73); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(72);
        match(PolyhedraParser::T__1);
        setState(75); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<PolyhedraParser::AtomErrorContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(78);
      match(PolyhedraParser::VARID);
      setState(80); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(79);
                poly();
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(82); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
       notifyErrorListeners("Only one polyhedron is allowed! If you want a set of polyhedra, use a powerset."); 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PowersetContext ------------------------------------------------------------------

PolyhedraParser::PowersetContext::PowersetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedraParser::PowersetContext::getRuleIndex() const {
  return PolyhedraParser::RulePowerset;
}

void PolyhedraParser::PowersetContext::copyFrom(PowersetContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PowersetTrueContext ------------------------------------------------------------------

PolyhedraParser::PowersetTrueContext::PowersetTrueContext(PowersetContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PowersetTrueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPowersetTrue(this);
}
void PolyhedraParser::PowersetTrueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPowersetTrue(this);
}

std::any PolyhedraParser::PowersetTrueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPowersetTrue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PowersetEmptyOrNotEmptyContext ------------------------------------------------------------------

std::vector<PolyhedraParser::PolyContext *> PolyhedraParser::PowersetEmptyOrNotEmptyContext::poly() {
  return getRuleContexts<PolyhedraParser::PolyContext>();
}

PolyhedraParser::PolyContext* PolyhedraParser::PowersetEmptyOrNotEmptyContext::poly(size_t i) {
  return getRuleContext<PolyhedraParser::PolyContext>(i);
}

PolyhedraParser::PowersetEmptyOrNotEmptyContext::PowersetEmptyOrNotEmptyContext(PowersetContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PowersetEmptyOrNotEmptyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPowersetEmptyOrNotEmpty(this);
}
void PolyhedraParser::PowersetEmptyOrNotEmptyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPowersetEmptyOrNotEmpty(this);
}

std::any PolyhedraParser::PowersetEmptyOrNotEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPowersetEmptyOrNotEmpty(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PowersetErrorContext ------------------------------------------------------------------

std::vector<PolyhedraParser::PolyContext *> PolyhedraParser::PowersetErrorContext::poly() {
  return getRuleContexts<PolyhedraParser::PolyContext>();
}

PolyhedraParser::PolyContext* PolyhedraParser::PowersetErrorContext::poly(size_t i) {
  return getRuleContext<PolyhedraParser::PolyContext>(i);
}

PolyhedraParser::PowersetErrorContext::PowersetErrorContext(PowersetContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PowersetErrorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPowersetError(this);
}
void PolyhedraParser::PowersetErrorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPowersetError(this);
}

std::any PolyhedraParser::PowersetErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPowersetError(this);
  else
    return visitor->visitChildren(this);
}
PolyhedraParser::PowersetContext* PolyhedraParser::powerset() {
  PowersetContext *_localctx = _tracker.createInstance<PowersetContext>(_ctx, getState());
  enterRule(_localctx, 4, PolyhedraParser::RulePowerset);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(166);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetEmptyOrNotEmptyContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(88);
      match(PolyhedraParser::T__2);
      setState(92);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18863106) != 0)) {
        setState(89);
        poly();
        setState(94);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(95);
      match(PolyhedraParser::T__3);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetTrueContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(96);
      match(PolyhedraParser::T__2);
      setState(97);
      match(PolyhedraParser::T__4);
      setState(98);
      match(PolyhedraParser::T__3);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetErrorContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(99);
      match(PolyhedraParser::T__2);
      setState(103);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18863106) != 0)) {
        setState(100);
        poly();
        setState(105);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(106);
      match(PolyhedraParser::T__3);
      setState(108); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(107);
        match(PolyhedraParser::T__3);
        setState(110); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__3);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetErrorContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(114); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(113);
                match(PolyhedraParser::T__2);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(116); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(118);
      match(PolyhedraParser::T__2);
      setState(122);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18863106) != 0)) {
        setState(119);
        poly();
        setState(124);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(125);
      match(PolyhedraParser::T__3);
      setState(127); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(126);
        match(PolyhedraParser::T__3);
        setState(129); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__3);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetErrorContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(133); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(132);
                match(PolyhedraParser::T__2);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(135); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(137);
      match(PolyhedraParser::T__2);
      setState(141);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18863106) != 0)) {
        setState(138);
        poly();
        setState(143);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(145); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(144);
        match(PolyhedraParser::T__3);
        setState(147); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__3);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetErrorContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(150);
      match(PolyhedraParser::T__2);
      setState(154);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(151);
          poly(); 
        }
        setState(156);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx);
      }
       notifyErrorListeners("Missing closing ')'");  
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<PolyhedraParser::PowersetErrorContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(161);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18863106) != 0)) {
        setState(158);
        poly();
        setState(163);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(164);
      match(PolyhedraParser::T__3);
       notifyErrorListeners("Missing opening '('");  
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PolyContext ------------------------------------------------------------------

PolyhedraParser::PolyContext::PolyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedraParser::PolyContext::getRuleIndex() const {
  return PolyhedraParser::RulePoly;
}

void PolyhedraParser::PolyContext::copyFrom(PolyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PolyAtLeastOneConstrContext ------------------------------------------------------------------

std::vector<PolyhedraParser::ConstrContext *> PolyhedraParser::PolyAtLeastOneConstrContext::constr() {
  return getRuleContexts<PolyhedraParser::ConstrContext>();
}

PolyhedraParser::ConstrContext* PolyhedraParser::PolyAtLeastOneConstrContext::constr(size_t i) {
  return getRuleContext<PolyhedraParser::ConstrContext>(i);
}

PolyhedraParser::PolyAtLeastOneConstrContext::PolyAtLeastOneConstrContext(PolyContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PolyAtLeastOneConstrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPolyAtLeastOneConstr(this);
}
void PolyhedraParser::PolyAtLeastOneConstrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPolyAtLeastOneConstr(this);
}

std::any PolyhedraParser::PolyAtLeastOneConstrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPolyAtLeastOneConstr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PolyTrueContext ------------------------------------------------------------------

PolyhedraParser::PolyTrueContext::PolyTrueContext(PolyContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PolyTrueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPolyTrue(this);
}
void PolyhedraParser::PolyTrueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPolyTrue(this);
}

std::any PolyhedraParser::PolyTrueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPolyTrue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PolyErrorContext ------------------------------------------------------------------

std::vector<PolyhedraParser::ConstrContext *> PolyhedraParser::PolyErrorContext::constr() {
  return getRuleContexts<PolyhedraParser::ConstrContext>();
}

PolyhedraParser::ConstrContext* PolyhedraParser::PolyErrorContext::constr(size_t i) {
  return getRuleContext<PolyhedraParser::ConstrContext>(i);
}

PolyhedraParser::PolyErrorContext::PolyErrorContext(PolyContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::PolyErrorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPolyError(this);
}
void PolyhedraParser::PolyErrorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPolyError(this);
}

std::any PolyhedraParser::PolyErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitPolyError(this);
  else
    return visitor->visitChildren(this);
}
PolyhedraParser::PolyContext* PolyhedraParser::poly() {
  PolyContext *_localctx = _tracker.createInstance<PolyContext>(_ctx, getState());
  enterRule(_localctx, 6, PolyhedraParser::RulePoly);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(304);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyAtLeastOneConstrContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(168);
      match(PolyhedraParser::T__0);
      setState(169);
      constr();
      setState(174);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(170);
        match(PolyhedraParser::T__5);
        setState(171);
        constr();
        setState(176);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(177);
      match(PolyhedraParser::T__1);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyTrueContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(179);
      match(PolyhedraParser::T__0);
      setState(180);
      match(PolyhedraParser::T__4);
      setState(181);
      match(PolyhedraParser::T__1);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(182);
      match(PolyhedraParser::T__0);
      setState(183);
      constr();
      setState(188);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(184);
        match(PolyhedraParser::T__5);
        setState(185);
        constr();
        setState(190);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(191);
      match(PolyhedraParser::T__1);
      setState(193); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(192);
        match(PolyhedraParser::T__1);
        setState(195); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(200); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(199);
                match(PolyhedraParser::T__0);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(202); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(204);
      match(PolyhedraParser::T__0);
      setState(205);
      constr();
      setState(210);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(206);
        match(PolyhedraParser::T__5);
        setState(207);
        constr();
        setState(212);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(213);
      match(PolyhedraParser::T__1);
      setState(215); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(214);
        match(PolyhedraParser::T__1);
        setState(217); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(222); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(221);
                match(PolyhedraParser::T__0);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(224); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(226);
      match(PolyhedraParser::T__0);
      setState(227);
      constr();
      setState(232);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(228);
        match(PolyhedraParser::T__5);
        setState(229);
        constr();
        setState(234);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(236); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(235);
        match(PolyhedraParser::T__1);
        setState(238); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedraParser::T__1);
       notifyErrorListeners("Too many parentheses"); 
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(242);
      constr();
      setState(247);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(243);
        match(PolyhedraParser::T__5);
        setState(244);
        constr();
        setState(249);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(250);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Missing closing '}'"); 
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(253);
      constr();
      setState(258);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(254);
        match(PolyhedraParser::T__5);
        setState(255);
        constr();
        setState(260);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(261);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Missing opening '{'"); 
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(264);
      match(PolyhedraParser::T__0);
      setState(271);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(265);
          constr();
          setState(266);
          match(PolyhedraParser::T__5);
          setState(267);
          constr(); 
        }
        setState(273);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
      }
      setState(274);
      constr();
      setState(275);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Constraints must be concatenated with '&' operator."); 
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 9);
      setState(278);
      match(PolyhedraParser::T__0);
      setState(279);
      constr();
      setState(284);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(280);
          match(PolyhedraParser::T__5);
          setState(281);
          constr(); 
        }
        setState(286);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
      }
      setState(287);
      match(PolyhedraParser::T__5);
      setState(288);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Missing right constraint."); 
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<PolyhedraParser::PolyErrorContext>(_localctx);
      enterOuterAlt(_localctx, 10);
      setState(291);
      match(PolyhedraParser::T__0);
      setState(292);
      match(PolyhedraParser::T__5);
      setState(293);
      constr();
      setState(298);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedraParser::T__5) {
        setState(294);
        match(PolyhedraParser::T__5);
        setState(295);
        constr();
        setState(300);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(301);
      match(PolyhedraParser::T__1);
       notifyErrorListeners("Missing left constraint."); 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstrContext ------------------------------------------------------------------

PolyhedraParser::ConstrContext::ConstrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PolyhedraParser::LinearExprContext *> PolyhedraParser::ConstrContext::linearExpr() {
  return getRuleContexts<PolyhedraParser::LinearExprContext>();
}

PolyhedraParser::LinearExprContext* PolyhedraParser::ConstrContext::linearExpr(size_t i) {
  return getRuleContext<PolyhedraParser::LinearExprContext>(i);
}

tree::TerminalNode* PolyhedraParser::ConstrContext::LE() {
  return getToken(PolyhedraParser::LE, 0);
}

tree::TerminalNode* PolyhedraParser::ConstrContext::LT() {
  return getToken(PolyhedraParser::LT, 0);
}

tree::TerminalNode* PolyhedraParser::ConstrContext::GT() {
  return getToken(PolyhedraParser::GT, 0);
}

tree::TerminalNode* PolyhedraParser::ConstrContext::GE() {
  return getToken(PolyhedraParser::GE, 0);
}

tree::TerminalNode* PolyhedraParser::ConstrContext::EQ() {
  return getToken(PolyhedraParser::EQ, 0);
}


size_t PolyhedraParser::ConstrContext::getRuleIndex() const {
  return PolyhedraParser::RuleConstr;
}

void PolyhedraParser::ConstrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstr(this);
}

void PolyhedraParser::ConstrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstr(this);
}


std::any PolyhedraParser::ConstrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitConstr(this);
  else
    return visitor->visitChildren(this);
}

PolyhedraParser::ConstrContext* PolyhedraParser::constr() {
  ConstrContext *_localctx = _tracker.createInstance<ConstrContext>(_ctx, getState());
  enterRule(_localctx, 8, PolyhedraParser::RuleConstr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(326);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(306);
      linearExpr();
      setState(307);
      antlrcpp::downCast<ConstrContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2031616) != 0))) {
        antlrcpp::downCast<ConstrContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(308);
      linearExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(310);
      linearExpr();
      setState(311);
      linearExpr();
       notifyErrorListeners("You must specify a valid operator (<=, <, >, >=, =) between the linear expressions."); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(314);
      linearExpr();
      setState(315);
      antlrcpp::downCast<ConstrContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2031616) != 0))) {
        antlrcpp::downCast<ConstrContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       notifyErrorListeners("Missing right linear expression"); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(318);
      antlrcpp::downCast<ConstrContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2031616) != 0))) {
        antlrcpp::downCast<ConstrContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(319);
      linearExpr();
       notifyErrorListeners("Missing left linear expression"); 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(322);
      linearExpr();
      setState(323);
      antlrcpp::downCast<ConstrContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (_la == 0 || _la == Token::EOF || ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2031616) != 0))) {
        antlrcpp::downCast<ConstrContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
       notifyErrorListeners("Invalid operator '" + (antlrcpp::downCast<ConstrContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<ConstrContext *>(_localctx)->op->getText() : "") + "'. Valid operators are: <=, <, >, >=, =."); 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LinearExprContext ------------------------------------------------------------------

PolyhedraParser::LinearExprContext::LinearExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PolyhedraParser::TermContext* PolyhedraParser::LinearExprContext::term() {
  return getRuleContext<PolyhedraParser::TermContext>(0);
}

std::vector<PolyhedraParser::SignTermContext *> PolyhedraParser::LinearExprContext::signTerm() {
  return getRuleContexts<PolyhedraParser::SignTermContext>();
}

PolyhedraParser::SignTermContext* PolyhedraParser::LinearExprContext::signTerm(size_t i) {
  return getRuleContext<PolyhedraParser::SignTermContext>(i);
}

tree::TerminalNode* PolyhedraParser::LinearExprContext::PLUS() {
  return getToken(PolyhedraParser::PLUS, 0);
}

tree::TerminalNode* PolyhedraParser::LinearExprContext::MINUS() {
  return getToken(PolyhedraParser::MINUS, 0);
}


size_t PolyhedraParser::LinearExprContext::getRuleIndex() const {
  return PolyhedraParser::RuleLinearExpr;
}

void PolyhedraParser::LinearExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLinearExpr(this);
}

void PolyhedraParser::LinearExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLinearExpr(this);
}


std::any PolyhedraParser::LinearExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitLinearExpr(this);
  else
    return visitor->visitChildren(this);
}

PolyhedraParser::LinearExprContext* PolyhedraParser::linearExpr() {
  LinearExprContext *_localctx = _tracker.createInstance<LinearExprContext>(_ctx, getState());
  enterRule(_localctx, 10, PolyhedraParser::RuleLinearExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(329);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PolyhedraParser::PLUS

    || _la == PolyhedraParser::MINUS) {
      setState(328);
      antlrcpp::downCast<LinearExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == PolyhedraParser::PLUS

      || _la == PolyhedraParser::MINUS)) {
        antlrcpp::downCast<LinearExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(331);
    term();
    setState(335);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(332);
        signTerm(); 
      }
      setState(337);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignTermContext ------------------------------------------------------------------

PolyhedraParser::SignTermContext::SignTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PolyhedraParser::TermContext* PolyhedraParser::SignTermContext::term() {
  return getRuleContext<PolyhedraParser::TermContext>(0);
}

tree::TerminalNode* PolyhedraParser::SignTermContext::PLUS() {
  return getToken(PolyhedraParser::PLUS, 0);
}

tree::TerminalNode* PolyhedraParser::SignTermContext::MINUS() {
  return getToken(PolyhedraParser::MINUS, 0);
}


size_t PolyhedraParser::SignTermContext::getRuleIndex() const {
  return PolyhedraParser::RuleSignTerm;
}

void PolyhedraParser::SignTermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSignTerm(this);
}

void PolyhedraParser::SignTermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSignTerm(this);
}


std::any PolyhedraParser::SignTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitSignTerm(this);
  else
    return visitor->visitChildren(this);
}

PolyhedraParser::SignTermContext* PolyhedraParser::signTerm() {
  SignTermContext *_localctx = _tracker.createInstance<SignTermContext>(_ctx, getState());
  enterRule(_localctx, 12, PolyhedraParser::RuleSignTerm);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(338);
    antlrcpp::downCast<SignTermContext *>(_localctx)->op = _input->LT(1);
    _la = _input->LA(1);
    if (!(_la == PolyhedraParser::PLUS

    || _la == PolyhedraParser::MINUS)) {
      antlrcpp::downCast<SignTermContext *>(_localctx)->op = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(339);
    term();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TermContext ------------------------------------------------------------------

PolyhedraParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedraParser::TermContext::getRuleIndex() const {
  return PolyhedraParser::RuleTerm;
}

void PolyhedraParser::TermContext::copyFrom(TermContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- IntTimesVarContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::IntTimesVarContext::UNSIGNED_INT() {
  return getToken(PolyhedraParser::UNSIGNED_INT, 0);
}

tree::TerminalNode* PolyhedraParser::IntTimesVarContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

tree::TerminalNode* PolyhedraParser::IntTimesVarContext::TIMES() {
  return getToken(PolyhedraParser::TIMES, 0);
}

PolyhedraParser::IntTimesVarContext::IntTimesVarContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::IntTimesVarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIntTimesVar(this);
}
void PolyhedraParser::IntTimesVarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIntTimesVar(this);
}

std::any PolyhedraParser::IntTimesVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitIntTimesVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::VarContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

PolyhedraParser::VarContext::VarContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::VarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar(this);
}
void PolyhedraParser::VarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar(this);
}

std::any PolyhedraParser::VarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TermErrorContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::TermErrorContext::UNSIGNED_INT() {
  return getToken(PolyhedraParser::UNSIGNED_INT, 0);
}

tree::TerminalNode* PolyhedraParser::TermErrorContext::VARID() {
  return getToken(PolyhedraParser::VARID, 0);
}

tree::TerminalNode* PolyhedraParser::TermErrorContext::TIMES() {
  return getToken(PolyhedraParser::TIMES, 0);
}

tree::TerminalNode* PolyhedraParser::TermErrorContext::FLOAT() {
  return getToken(PolyhedraParser::FLOAT, 0);
}

PolyhedraParser::TermErrorContext::TermErrorContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::TermErrorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTermError(this);
}
void PolyhedraParser::TermErrorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTermError(this);
}

std::any PolyhedraParser::TermErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitTermError(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedraParser::IntContext::UNSIGNED_INT() {
  return getToken(PolyhedraParser::UNSIGNED_INT, 0);
}

PolyhedraParser::IntContext::IntContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedraParser::IntContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInt(this);
}
void PolyhedraParser::IntContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedraListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInt(this);
}

std::any PolyhedraParser::IntContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedraVisitor*>(visitor))
    return parserVisitor->visitInt(this);
  else
    return visitor->visitChildren(this);
}
PolyhedraParser::TermContext* PolyhedraParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 14, PolyhedraParser::RuleTerm);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(360);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedraParser::IntTimesVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(341);
      match(PolyhedraParser::UNSIGNED_INT);
      setState(343);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedraParser::TIMES) {
        setState(342);
        match(PolyhedraParser::TIMES);
      }
      setState(345);
      match(PolyhedraParser::VARID);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedraParser::VarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(346);
      match(PolyhedraParser::VARID);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedraParser::IntContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(347);
      match(PolyhedraParser::UNSIGNED_INT);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<PolyhedraParser::TermErrorContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(348);
      match(PolyhedraParser::UNSIGNED_INT);
      setState(349);
      antlrcpp::downCast<TermErrorContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (_la == 0 || _la == Token::EOF || (_la == PolyhedraParser::TIMES)) {
        antlrcpp::downCast<TermErrorContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(350);
      match(PolyhedraParser::VARID);
       notifyErrorListeners("Invalid operator '" + (antlrcpp::downCast<TermErrorContext *>(_localctx)->op != nullptr ? antlrcpp::downCast<TermErrorContext *>(_localctx)->op->getText() : "") + "'. Only '*' is allowed."); 
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<PolyhedraParser::TermErrorContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(352);
      match(PolyhedraParser::FLOAT);
       notifyErrorListeners("Only integer values are allowed."); 
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<PolyhedraParser::TermErrorContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(354);
      match(PolyhedraParser::FLOAT);
      setState(356);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedraParser::TIMES) {
        setState(355);
        match(PolyhedraParser::TIMES);
      }
      setState(358);
      match(PolyhedraParser::VARID);
       notifyErrorListeners("Only integer values are allowed."); 
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void PolyhedraParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  polyhedraParserInitialize();
#else
  ::antlr4::internal::call_once(polyhedraParserOnceFlag, polyhedraParserInitialize);
#endif
}
