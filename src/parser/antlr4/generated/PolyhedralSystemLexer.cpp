
// Generated from ./g4/PolyhedralSystem.g4 by ANTLR 4.13.2


#include "PolyhedralSystemLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct PolyhedralSystemLexerStaticData final {
  PolyhedralSystemLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PolyhedralSystemLexerStaticData(const PolyhedralSystemLexerStaticData&) = delete;
  PolyhedralSystemLexerStaticData(PolyhedralSystemLexerStaticData&&) = delete;
  PolyhedralSystemLexerStaticData& operator=(const PolyhedralSystemLexerStaticData&) = delete;
  PolyhedralSystemLexerStaticData& operator=(PolyhedralSystemLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag polyhedralsystemlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<PolyhedralSystemLexerStaticData> polyhedralsystemlexerLexerStaticData = nullptr;

void polyhedralsystemlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (polyhedralsystemlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(polyhedralsystemlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PolyhedralSystemLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "DIGIT", "LETTER", "A", "B", 
      "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", 
      "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "LINE_COMMENT", 
      "COMMENT", "WS", "UNSIGNED_INT", "NEWLINE", "TIMES", "PLUS", "MINUS", 
      "LE", "LT", "GT", "GE", "EQ", "FLOW", "INV", "ID"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'{'", "'}'", "'('", "')'", "'&'", "", "", "", "", "", "'*'", 
      "'+'", "'-'", "'<='", "'<'", "'>'", "'>='", "'='"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "LINE_COMMENT", "COMMENT", "WS", "UNSIGNED_INT", 
      "NEWLINE", "TIMES", "PLUS", "MINUS", "LE", "LT", "GT", "GE", "EQ", 
      "FLOW", "INV", "ID"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,21,245,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,1,0,
  	1,0,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,
  	9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,13,1,13,1,14,1,14,1,15,1,15,1,16,
  	1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,
  	1,23,1,24,1,24,1,25,1,25,1,26,1,26,1,27,1,27,1,28,1,28,1,29,1,29,1,30,
  	1,30,1,31,1,31,1,32,1,32,1,33,1,33,1,33,1,33,5,33,170,8,33,10,33,12,33,
  	173,9,33,1,33,1,33,1,33,1,33,1,34,1,34,1,34,1,34,5,34,183,8,34,10,34,
  	12,34,186,9,34,1,34,1,34,1,34,1,34,1,34,1,35,4,35,194,8,35,11,35,12,35,
  	195,1,35,1,35,1,36,1,36,1,36,5,36,203,8,36,10,36,12,36,206,9,36,3,36,
  	208,8,36,1,37,3,37,211,8,37,1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,
  	41,1,41,1,41,1,42,1,42,1,43,1,43,1,44,1,44,1,44,1,45,1,45,1,46,1,46,1,
  	46,1,46,1,46,1,47,1,47,1,47,1,47,1,48,1,48,3,48,244,8,48,2,171,184,0,
  	49,1,1,3,2,5,3,7,4,9,5,11,0,13,0,15,0,17,0,19,0,21,0,23,0,25,0,27,0,29,
  	0,31,0,33,0,35,0,37,0,39,0,41,0,43,0,45,0,47,0,49,0,51,0,53,0,55,0,57,
  	0,59,0,61,0,63,0,65,0,67,6,69,7,71,8,73,9,75,10,77,11,79,12,81,13,83,
  	14,85,15,87,16,89,17,91,18,93,19,95,20,97,21,1,0,30,1,0,48,57,2,0,65,
  	90,97,122,2,0,65,65,97,97,2,0,66,66,98,98,2,0,67,67,99,99,2,0,68,68,100,
  	100,2,0,69,69,101,101,2,0,70,70,102,102,2,0,71,71,103,103,2,0,72,72,104,
  	104,2,0,73,73,105,105,2,0,74,74,106,106,2,0,75,75,107,107,2,0,76,76,108,
  	108,2,0,77,77,109,109,2,0,78,78,110,110,2,0,79,79,111,111,2,0,80,80,112,
  	112,2,0,81,81,113,113,2,0,82,82,114,114,2,0,83,83,115,115,2,0,84,84,116,
  	116,2,0,85,85,117,117,2,0,86,86,118,118,2,0,87,87,119,119,2,0,88,88,120,
  	120,2,0,89,89,121,121,2,0,90,90,122,122,3,0,9,10,13,13,32,32,1,0,49,57,
  	223,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,67,
  	1,0,0,0,0,69,1,0,0,0,0,71,1,0,0,0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,1,0,
  	0,0,0,79,1,0,0,0,0,81,1,0,0,0,0,83,1,0,0,0,0,85,1,0,0,0,0,87,1,0,0,0,
  	0,89,1,0,0,0,0,91,1,0,0,0,0,93,1,0,0,0,0,95,1,0,0,0,0,97,1,0,0,0,1,99,
  	1,0,0,0,3,101,1,0,0,0,5,103,1,0,0,0,7,105,1,0,0,0,9,107,1,0,0,0,11,109,
  	1,0,0,0,13,111,1,0,0,0,15,113,1,0,0,0,17,115,1,0,0,0,19,117,1,0,0,0,21,
  	119,1,0,0,0,23,121,1,0,0,0,25,123,1,0,0,0,27,125,1,0,0,0,29,127,1,0,0,
  	0,31,129,1,0,0,0,33,131,1,0,0,0,35,133,1,0,0,0,37,135,1,0,0,0,39,137,
  	1,0,0,0,41,139,1,0,0,0,43,141,1,0,0,0,45,143,1,0,0,0,47,145,1,0,0,0,49,
  	147,1,0,0,0,51,149,1,0,0,0,53,151,1,0,0,0,55,153,1,0,0,0,57,155,1,0,0,
  	0,59,157,1,0,0,0,61,159,1,0,0,0,63,161,1,0,0,0,65,163,1,0,0,0,67,165,
  	1,0,0,0,69,178,1,0,0,0,71,193,1,0,0,0,73,207,1,0,0,0,75,210,1,0,0,0,77,
  	214,1,0,0,0,79,216,1,0,0,0,81,218,1,0,0,0,83,220,1,0,0,0,85,223,1,0,0,
  	0,87,225,1,0,0,0,89,227,1,0,0,0,91,230,1,0,0,0,93,232,1,0,0,0,95,237,
  	1,0,0,0,97,241,1,0,0,0,99,100,5,123,0,0,100,2,1,0,0,0,101,102,5,125,0,
  	0,102,4,1,0,0,0,103,104,5,40,0,0,104,6,1,0,0,0,105,106,5,41,0,0,106,8,
  	1,0,0,0,107,108,5,38,0,0,108,10,1,0,0,0,109,110,7,0,0,0,110,12,1,0,0,
  	0,111,112,7,1,0,0,112,14,1,0,0,0,113,114,7,2,0,0,114,16,1,0,0,0,115,116,
  	7,3,0,0,116,18,1,0,0,0,117,118,7,4,0,0,118,20,1,0,0,0,119,120,7,5,0,0,
  	120,22,1,0,0,0,121,122,7,6,0,0,122,24,1,0,0,0,123,124,7,7,0,0,124,26,
  	1,0,0,0,125,126,7,8,0,0,126,28,1,0,0,0,127,128,7,9,0,0,128,30,1,0,0,0,
  	129,130,7,10,0,0,130,32,1,0,0,0,131,132,7,11,0,0,132,34,1,0,0,0,133,134,
  	7,12,0,0,134,36,1,0,0,0,135,136,7,13,0,0,136,38,1,0,0,0,137,138,7,14,
  	0,0,138,40,1,0,0,0,139,140,7,15,0,0,140,42,1,0,0,0,141,142,7,16,0,0,142,
  	44,1,0,0,0,143,144,7,17,0,0,144,46,1,0,0,0,145,146,7,18,0,0,146,48,1,
  	0,0,0,147,148,7,19,0,0,148,50,1,0,0,0,149,150,7,20,0,0,150,52,1,0,0,0,
  	151,152,7,21,0,0,152,54,1,0,0,0,153,154,7,22,0,0,154,56,1,0,0,0,155,156,
  	7,23,0,0,156,58,1,0,0,0,157,158,7,24,0,0,158,60,1,0,0,0,159,160,7,25,
  	0,0,160,62,1,0,0,0,161,162,7,26,0,0,162,64,1,0,0,0,163,164,7,27,0,0,164,
  	66,1,0,0,0,165,166,5,47,0,0,166,167,5,47,0,0,167,171,1,0,0,0,168,170,
  	9,0,0,0,169,168,1,0,0,0,170,173,1,0,0,0,171,172,1,0,0,0,171,169,1,0,0,
  	0,172,174,1,0,0,0,173,171,1,0,0,0,174,175,3,75,37,0,175,176,1,0,0,0,176,
  	177,6,33,0,0,177,68,1,0,0,0,178,179,5,47,0,0,179,180,5,42,0,0,180,184,
  	1,0,0,0,181,183,9,0,0,0,182,181,1,0,0,0,183,186,1,0,0,0,184,185,1,0,0,
  	0,184,182,1,0,0,0,185,187,1,0,0,0,186,184,1,0,0,0,187,188,5,42,0,0,188,
  	189,5,47,0,0,189,190,1,0,0,0,190,191,6,34,0,0,191,70,1,0,0,0,192,194,
  	7,28,0,0,193,192,1,0,0,0,194,195,1,0,0,0,195,193,1,0,0,0,195,196,1,0,
  	0,0,196,197,1,0,0,0,197,198,6,35,0,0,198,72,1,0,0,0,199,208,3,11,5,0,
  	200,204,7,29,0,0,201,203,3,11,5,0,202,201,1,0,0,0,203,206,1,0,0,0,204,
  	202,1,0,0,0,204,205,1,0,0,0,205,208,1,0,0,0,206,204,1,0,0,0,207,199,1,
  	0,0,0,207,200,1,0,0,0,208,74,1,0,0,0,209,211,5,13,0,0,210,209,1,0,0,0,
  	210,211,1,0,0,0,211,212,1,0,0,0,212,213,5,10,0,0,213,76,1,0,0,0,214,215,
  	5,42,0,0,215,78,1,0,0,0,216,217,5,43,0,0,217,80,1,0,0,0,218,219,5,45,
  	0,0,219,82,1,0,0,0,220,221,5,60,0,0,221,222,5,61,0,0,222,84,1,0,0,0,223,
  	224,5,60,0,0,224,86,1,0,0,0,225,226,5,62,0,0,226,88,1,0,0,0,227,228,5,
  	62,0,0,228,229,5,61,0,0,229,90,1,0,0,0,230,231,5,61,0,0,231,92,1,0,0,
  	0,232,233,3,25,12,0,233,234,3,37,18,0,234,235,3,43,21,0,235,236,3,59,
  	29,0,236,94,1,0,0,0,237,238,3,31,15,0,238,239,3,41,20,0,239,240,3,57,
  	28,0,240,96,1,0,0,0,241,243,3,13,6,0,242,244,3,11,5,0,243,242,1,0,0,0,
  	243,244,1,0,0,0,244,98,1,0,0,0,8,0,171,184,195,204,207,210,243,1,6,0,
  	0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  polyhedralsystemlexerLexerStaticData = std::move(staticData);
}

}

PolyhedralSystemLexer::PolyhedralSystemLexer(CharStream *input) : Lexer(input) {
  PolyhedralSystemLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *polyhedralsystemlexerLexerStaticData->atn, polyhedralsystemlexerLexerStaticData->decisionToDFA, polyhedralsystemlexerLexerStaticData->sharedContextCache);
}

PolyhedralSystemLexer::~PolyhedralSystemLexer() {
  delete _interpreter;
}

std::string PolyhedralSystemLexer::getGrammarFileName() const {
  return "PolyhedralSystem.g4";
}

const std::vector<std::string>& PolyhedralSystemLexer::getRuleNames() const {
  return polyhedralsystemlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& PolyhedralSystemLexer::getChannelNames() const {
  return polyhedralsystemlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& PolyhedralSystemLexer::getModeNames() const {
  return polyhedralsystemlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& PolyhedralSystemLexer::getVocabulary() const {
  return polyhedralsystemlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView PolyhedralSystemLexer::getSerializedATN() const {
  return polyhedralsystemlexerLexerStaticData->serializedATN;
}

const atn::ATN& PolyhedralSystemLexer::getATN() const {
  return *polyhedralsystemlexerLexerStaticData->atn;
}




void PolyhedralSystemLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  polyhedralsystemlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(polyhedralsystemlexerLexerOnceFlag, polyhedralsystemlexerLexerInitialize);
#endif
}
