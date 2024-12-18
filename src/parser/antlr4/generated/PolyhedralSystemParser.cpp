
// Generated from ./g4/PolyhedralSystem.g4 by ANTLR 4.13.2


#include "PolyhedralSystemListener.h"
#include "PolyhedralSystemVisitor.h"

#include "PolyhedralSystemParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct PolyhedralSystemParserStaticData final {
  PolyhedralSystemParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PolyhedralSystemParserStaticData(const PolyhedralSystemParserStaticData&) = delete;
  PolyhedralSystemParserStaticData(PolyhedralSystemParserStaticData&&) = delete;
  PolyhedralSystemParserStaticData& operator=(const PolyhedralSystemParserStaticData&) = delete;
  PolyhedralSystemParserStaticData& operator=(PolyhedralSystemParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag polyhedralsystemParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<PolyhedralSystemParserStaticData> polyhedralsystemParserStaticData = nullptr;

void polyhedralsystemParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (polyhedralsystemParserStaticData != nullptr) {
    return;
  }
#else
  assert(polyhedralsystemParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PolyhedralSystemParserStaticData>(
    std::vector<std::string>{
      "system", "inv", "flow", "atom", "powerset", "poly", "constr", "linearExpr", 
      "signTerm", "term"
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
  	4,1,21,260,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,1,0,1,0,1,0,4,0,24,8,0,11,0,12,0,25,1,0,1,0,1,0,
  	4,0,31,8,0,11,0,12,0,32,1,0,4,0,36,8,0,11,0,12,0,37,1,0,1,0,1,0,1,0,4,
  	0,44,8,0,11,0,12,0,45,1,0,1,0,1,0,1,0,1,0,4,0,53,8,0,11,0,12,0,54,1,0,
  	1,0,1,0,1,0,4,0,61,8,0,11,0,12,0,62,1,0,1,0,1,0,4,0,68,8,0,11,0,12,0,
  	69,1,0,1,0,5,0,74,8,0,10,0,12,0,77,9,0,1,0,1,0,5,0,81,8,0,10,0,12,0,84,
  	9,0,1,0,5,0,87,8,0,10,0,12,0,90,9,0,1,0,1,0,4,0,94,8,0,11,0,12,0,95,1,
  	0,1,0,5,0,100,8,0,10,0,12,0,103,9,0,1,0,5,0,106,8,0,10,0,12,0,109,9,0,
  	1,0,1,0,5,0,113,8,0,10,0,12,0,116,9,0,1,0,1,0,4,0,120,8,0,11,0,12,0,121,
  	1,0,4,0,125,8,0,11,0,12,0,126,1,0,1,0,5,0,131,8,0,10,0,12,0,134,9,0,1,
  	0,1,0,5,0,138,8,0,10,0,12,0,141,9,0,1,0,5,0,144,8,0,10,0,12,0,147,9,0,
  	1,0,1,0,4,0,151,8,0,11,0,12,0,152,1,0,1,0,5,0,157,8,0,10,0,12,0,160,9,
  	0,1,0,5,0,163,8,0,10,0,12,0,166,9,0,1,0,1,0,5,0,170,8,0,10,0,12,0,173,
  	9,0,1,0,1,0,4,0,177,8,0,11,0,12,0,178,3,0,181,8,0,1,1,1,1,1,1,3,1,186,
  	8,1,1,2,1,2,1,2,3,2,191,8,2,1,3,1,3,1,3,3,3,196,8,3,1,3,1,3,1,3,5,3,201,
  	8,3,10,3,12,3,204,9,3,1,3,1,3,1,3,3,3,209,8,3,1,4,1,4,5,4,213,8,4,10,
  	4,12,4,216,9,4,1,4,1,4,1,5,1,5,1,5,1,5,3,5,224,8,5,1,5,5,5,227,8,5,10,
  	5,12,5,230,9,5,1,5,1,5,1,6,1,6,1,6,1,6,1,7,3,7,239,8,7,1,7,1,7,5,7,243,
  	8,7,10,7,12,7,246,9,7,1,8,1,8,1,8,1,9,1,9,3,9,253,8,9,1,9,1,9,1,9,3,9,
  	258,8,9,1,9,0,0,10,0,2,4,6,8,10,12,14,16,18,0,2,1,0,14,18,1,0,12,13,298,
  	0,180,1,0,0,0,2,182,1,0,0,0,4,187,1,0,0,0,6,208,1,0,0,0,8,210,1,0,0,0,
  	10,219,1,0,0,0,12,233,1,0,0,0,14,238,1,0,0,0,16,247,1,0,0,0,18,257,1,
  	0,0,0,20,21,3,4,2,0,21,23,3,2,1,0,22,24,3,6,3,0,23,22,1,0,0,0,24,25,1,
  	0,0,0,25,23,1,0,0,0,25,26,1,0,0,0,26,181,1,0,0,0,27,28,3,2,1,0,28,30,
  	3,4,2,0,29,31,3,6,3,0,30,29,1,0,0,0,31,32,1,0,0,0,32,30,1,0,0,0,32,33,
  	1,0,0,0,33,181,1,0,0,0,34,36,3,6,3,0,35,34,1,0,0,0,36,37,1,0,0,0,37,35,
  	1,0,0,0,37,38,1,0,0,0,38,39,1,0,0,0,39,40,3,2,1,0,40,41,3,4,2,0,41,181,
  	1,0,0,0,42,44,3,6,3,0,43,42,1,0,0,0,44,45,1,0,0,0,45,43,1,0,0,0,45,46,
  	1,0,0,0,46,47,1,0,0,0,47,48,3,4,2,0,48,49,3,2,1,0,49,181,1,0,0,0,50,52,
  	3,2,1,0,51,53,3,6,3,0,52,51,1,0,0,0,53,54,1,0,0,0,54,52,1,0,0,0,54,55,
  	1,0,0,0,55,56,1,0,0,0,56,57,3,4,2,0,57,181,1,0,0,0,58,60,3,4,2,0,59,61,
  	3,6,3,0,60,59,1,0,0,0,61,62,1,0,0,0,62,60,1,0,0,0,62,63,1,0,0,0,63,64,
  	1,0,0,0,64,65,3,2,1,0,65,181,1,0,0,0,66,68,3,6,3,0,67,66,1,0,0,0,68,69,
  	1,0,0,0,69,67,1,0,0,0,69,70,1,0,0,0,70,71,1,0,0,0,71,75,3,4,2,0,72,74,
  	3,6,3,0,73,72,1,0,0,0,74,77,1,0,0,0,75,73,1,0,0,0,75,76,1,0,0,0,76,78,
  	1,0,0,0,77,75,1,0,0,0,78,82,3,2,1,0,79,81,3,6,3,0,80,79,1,0,0,0,81,84,
  	1,0,0,0,82,80,1,0,0,0,82,83,1,0,0,0,83,181,1,0,0,0,84,82,1,0,0,0,85,87,
  	3,6,3,0,86,85,1,0,0,0,87,90,1,0,0,0,88,86,1,0,0,0,88,89,1,0,0,0,89,91,
  	1,0,0,0,90,88,1,0,0,0,91,93,3,4,2,0,92,94,3,6,3,0,93,92,1,0,0,0,94,95,
  	1,0,0,0,95,93,1,0,0,0,95,96,1,0,0,0,96,97,1,0,0,0,97,101,3,2,1,0,98,100,
  	3,6,3,0,99,98,1,0,0,0,100,103,1,0,0,0,101,99,1,0,0,0,101,102,1,0,0,0,
  	102,181,1,0,0,0,103,101,1,0,0,0,104,106,3,6,3,0,105,104,1,0,0,0,106,109,
  	1,0,0,0,107,105,1,0,0,0,107,108,1,0,0,0,108,110,1,0,0,0,109,107,1,0,0,
  	0,110,114,3,4,2,0,111,113,3,6,3,0,112,111,1,0,0,0,113,116,1,0,0,0,114,
  	112,1,0,0,0,114,115,1,0,0,0,115,117,1,0,0,0,116,114,1,0,0,0,117,119,3,
  	2,1,0,118,120,3,6,3,0,119,118,1,0,0,0,120,121,1,0,0,0,121,119,1,0,0,0,
  	121,122,1,0,0,0,122,181,1,0,0,0,123,125,3,6,3,0,124,123,1,0,0,0,125,126,
  	1,0,0,0,126,124,1,0,0,0,126,127,1,0,0,0,127,128,1,0,0,0,128,132,3,2,1,
  	0,129,131,3,6,3,0,130,129,1,0,0,0,131,134,1,0,0,0,132,130,1,0,0,0,132,
  	133,1,0,0,0,133,135,1,0,0,0,134,132,1,0,0,0,135,139,3,4,2,0,136,138,3,
  	6,3,0,137,136,1,0,0,0,138,141,1,0,0,0,139,137,1,0,0,0,139,140,1,0,0,0,
  	140,181,1,0,0,0,141,139,1,0,0,0,142,144,3,6,3,0,143,142,1,0,0,0,144,147,
  	1,0,0,0,145,143,1,0,0,0,145,146,1,0,0,0,146,148,1,0,0,0,147,145,1,0,0,
  	0,148,150,3,2,1,0,149,151,3,6,3,0,150,149,1,0,0,0,151,152,1,0,0,0,152,
  	150,1,0,0,0,152,153,1,0,0,0,153,154,1,0,0,0,154,158,3,4,2,0,155,157,3,
  	6,3,0,156,155,1,0,0,0,157,160,1,0,0,0,158,156,1,0,0,0,158,159,1,0,0,0,
  	159,181,1,0,0,0,160,158,1,0,0,0,161,163,3,6,3,0,162,161,1,0,0,0,163,166,
  	1,0,0,0,164,162,1,0,0,0,164,165,1,0,0,0,165,167,1,0,0,0,166,164,1,0,0,
  	0,167,171,3,2,1,0,168,170,3,6,3,0,169,168,1,0,0,0,170,173,1,0,0,0,171,
  	169,1,0,0,0,171,172,1,0,0,0,172,174,1,0,0,0,173,171,1,0,0,0,174,176,3,
  	4,2,0,175,177,3,6,3,0,176,175,1,0,0,0,177,178,1,0,0,0,178,176,1,0,0,0,
  	178,179,1,0,0,0,179,181,1,0,0,0,180,20,1,0,0,0,180,27,1,0,0,0,180,35,
  	1,0,0,0,180,43,1,0,0,0,180,50,1,0,0,0,180,58,1,0,0,0,180,67,1,0,0,0,180,
  	88,1,0,0,0,180,107,1,0,0,0,180,124,1,0,0,0,180,145,1,0,0,0,180,164,1,
  	0,0,0,181,1,1,0,0,0,182,183,5,20,0,0,183,185,3,8,4,0,184,186,5,10,0,0,
  	185,184,1,0,0,0,185,186,1,0,0,0,186,3,1,0,0,0,187,188,5,19,0,0,188,190,
  	3,10,5,0,189,191,5,10,0,0,190,189,1,0,0,0,190,191,1,0,0,0,191,5,1,0,0,
  	0,192,193,5,21,0,0,193,195,3,8,4,0,194,196,5,10,0,0,195,194,1,0,0,0,195,
  	196,1,0,0,0,196,209,1,0,0,0,197,198,5,21,0,0,198,202,5,1,0,0,199,201,
  	5,8,0,0,200,199,1,0,0,0,201,204,1,0,0,0,202,200,1,0,0,0,202,203,1,0,0,
  	0,203,205,1,0,0,0,204,202,1,0,0,0,205,209,5,2,0,0,206,207,5,21,0,0,207,
  	209,3,10,5,0,208,192,1,0,0,0,208,197,1,0,0,0,208,206,1,0,0,0,209,7,1,
  	0,0,0,210,214,5,3,0,0,211,213,3,10,5,0,212,211,1,0,0,0,213,216,1,0,0,
  	0,214,212,1,0,0,0,214,215,1,0,0,0,215,217,1,0,0,0,216,214,1,0,0,0,217,
  	218,5,4,0,0,218,9,1,0,0,0,219,220,5,1,0,0,220,228,3,12,6,0,221,223,5,
  	5,0,0,222,224,5,10,0,0,223,222,1,0,0,0,223,224,1,0,0,0,224,225,1,0,0,
  	0,225,227,3,12,6,0,226,221,1,0,0,0,227,230,1,0,0,0,228,226,1,0,0,0,228,
  	229,1,0,0,0,229,231,1,0,0,0,230,228,1,0,0,0,231,232,5,2,0,0,232,11,1,
  	0,0,0,233,234,3,14,7,0,234,235,7,0,0,0,235,236,3,14,7,0,236,13,1,0,0,
  	0,237,239,7,1,0,0,238,237,1,0,0,0,238,239,1,0,0,0,239,240,1,0,0,0,240,
  	244,3,18,9,0,241,243,3,16,8,0,242,241,1,0,0,0,243,246,1,0,0,0,244,242,
  	1,0,0,0,244,245,1,0,0,0,245,15,1,0,0,0,246,244,1,0,0,0,247,248,7,1,0,
  	0,248,249,3,18,9,0,249,17,1,0,0,0,250,252,5,9,0,0,251,253,5,11,0,0,252,
  	251,1,0,0,0,252,253,1,0,0,0,253,254,1,0,0,0,254,258,5,21,0,0,255,258,
  	5,21,0,0,256,258,5,9,0,0,257,250,1,0,0,0,257,255,1,0,0,0,257,256,1,0,
  	0,0,258,19,1,0,0,0,37,25,32,37,45,54,62,69,75,82,88,95,101,107,114,121,
  	126,132,139,145,152,158,164,171,178,180,185,190,195,202,208,214,223,228,
  	238,244,252,257
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  polyhedralsystemParserStaticData = std::move(staticData);
}

}

PolyhedralSystemParser::PolyhedralSystemParser(TokenStream *input) : PolyhedralSystemParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

PolyhedralSystemParser::PolyhedralSystemParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  PolyhedralSystemParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *polyhedralsystemParserStaticData->atn, polyhedralsystemParserStaticData->decisionToDFA, polyhedralsystemParserStaticData->sharedContextCache, options);
}

PolyhedralSystemParser::~PolyhedralSystemParser() {
  delete _interpreter;
}

const atn::ATN& PolyhedralSystemParser::getATN() const {
  return *polyhedralsystemParserStaticData->atn;
}

std::string PolyhedralSystemParser::getGrammarFileName() const {
  return "PolyhedralSystem.g4";
}

const std::vector<std::string>& PolyhedralSystemParser::getRuleNames() const {
  return polyhedralsystemParserStaticData->ruleNames;
}

const dfa::Vocabulary& PolyhedralSystemParser::getVocabulary() const {
  return polyhedralsystemParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView PolyhedralSystemParser::getSerializedATN() const {
  return polyhedralsystemParserStaticData->serializedATN;
}


//----------------- SystemContext ------------------------------------------------------------------

PolyhedralSystemParser::SystemContext::SystemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PolyhedralSystemParser::FlowContext* PolyhedralSystemParser::SystemContext::flow() {
  return getRuleContext<PolyhedralSystemParser::FlowContext>(0);
}

PolyhedralSystemParser::InvContext* PolyhedralSystemParser::SystemContext::inv() {
  return getRuleContext<PolyhedralSystemParser::InvContext>(0);
}

std::vector<PolyhedralSystemParser::AtomContext *> PolyhedralSystemParser::SystemContext::atom() {
  return getRuleContexts<PolyhedralSystemParser::AtomContext>();
}

PolyhedralSystemParser::AtomContext* PolyhedralSystemParser::SystemContext::atom(size_t i) {
  return getRuleContext<PolyhedralSystemParser::AtomContext>(i);
}


size_t PolyhedralSystemParser::SystemContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleSystem;
}

void PolyhedralSystemParser::SystemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSystem(this);
}

void PolyhedralSystemParser::SystemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSystem(this);
}


std::any PolyhedralSystemParser::SystemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitSystem(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::SystemContext* PolyhedralSystemParser::system() {
  SystemContext *_localctx = _tracker.createInstance<SystemContext>(_ctx, getState());
  enterRule(_localctx, 0, PolyhedralSystemParser::RuleSystem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(180);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(20);
      flow();
      setState(21);
      inv();
      setState(23); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(22);
        atom();
        setState(25); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(27);
      inv();
      setState(28);
      flow();
      setState(30); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(29);
        atom();
        setState(32); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(35); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(34);
        atom();
        setState(37); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(39);
      inv();
      setState(40);
      flow();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(43); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(42);
        atom();
        setState(45); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(47);
      flow();
      setState(48);
      inv();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(50);
      inv();
      setState(52); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(51);
        atom();
        setState(54); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(56);
      flow();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(58);
      flow();
      setState(60); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(59);
        atom();
        setState(62); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(64);
      inv();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(67); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(66);
        atom();
        setState(69); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(71);
      flow();
      setState(75);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(72);
        atom();
        setState(77);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(78);
      inv();
      setState(82);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(79);
        atom();
        setState(84);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(88);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(85);
        atom();
        setState(90);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(91);
      flow();
      setState(93); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(92);
        atom();
        setState(95); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(97);
      inv();
      setState(101);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(98);
        atom();
        setState(103);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(107);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(104);
        atom();
        setState(109);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(110);
      flow();
      setState(114);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(111);
        atom();
        setState(116);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(117);
      inv();
      setState(119); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(118);
        atom();
        setState(121); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(124); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(123);
        atom();
        setState(126); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(128);
      inv();
      setState(132);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(129);
        atom();
        setState(134);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(135);
      flow();
      setState(139);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(136);
        atom();
        setState(141);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(145);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(142);
        atom();
        setState(147);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(148);
      inv();
      setState(150); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(149);
        atom();
        setState(152); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(154);
      flow();
      setState(158);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(155);
        atom();
        setState(160);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(164);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(161);
        atom();
        setState(166);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(167);
      inv();
      setState(171);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(168);
        atom();
        setState(173);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(174);
      flow();
      setState(176); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(175);
        atom();
        setState(178); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
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

//----------------- InvContext ------------------------------------------------------------------

PolyhedralSystemParser::InvContext::InvContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PolyhedralSystemParser::InvContext::INV() {
  return getToken(PolyhedralSystemParser::INV, 0);
}

PolyhedralSystemParser::PowersetContext* PolyhedralSystemParser::InvContext::powerset() {
  return getRuleContext<PolyhedralSystemParser::PowersetContext>(0);
}

tree::TerminalNode* PolyhedralSystemParser::InvContext::NEWLINE() {
  return getToken(PolyhedralSystemParser::NEWLINE, 0);
}


size_t PolyhedralSystemParser::InvContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleInv;
}

void PolyhedralSystemParser::InvContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInv(this);
}

void PolyhedralSystemParser::InvContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInv(this);
}


std::any PolyhedralSystemParser::InvContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitInv(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::InvContext* PolyhedralSystemParser::inv() {
  InvContext *_localctx = _tracker.createInstance<InvContext>(_ctx, getState());
  enterRule(_localctx, 2, PolyhedralSystemParser::RuleInv);
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
    setState(182);
    match(PolyhedralSystemParser::INV);
    setState(183);
    powerset();
    setState(185);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PolyhedralSystemParser::NEWLINE) {
      setState(184);
      match(PolyhedralSystemParser::NEWLINE);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FlowContext ------------------------------------------------------------------

PolyhedralSystemParser::FlowContext::FlowContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PolyhedralSystemParser::FlowContext::FLOW() {
  return getToken(PolyhedralSystemParser::FLOW, 0);
}

PolyhedralSystemParser::PolyContext* PolyhedralSystemParser::FlowContext::poly() {
  return getRuleContext<PolyhedralSystemParser::PolyContext>(0);
}

tree::TerminalNode* PolyhedralSystemParser::FlowContext::NEWLINE() {
  return getToken(PolyhedralSystemParser::NEWLINE, 0);
}


size_t PolyhedralSystemParser::FlowContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleFlow;
}

void PolyhedralSystemParser::FlowContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFlow(this);
}

void PolyhedralSystemParser::FlowContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFlow(this);
}


std::any PolyhedralSystemParser::FlowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitFlow(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::FlowContext* PolyhedralSystemParser::flow() {
  FlowContext *_localctx = _tracker.createInstance<FlowContext>(_ctx, getState());
  enterRule(_localctx, 4, PolyhedralSystemParser::RuleFlow);
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
    setState(187);
    match(PolyhedralSystemParser::FLOW);
    setState(188);
    poly();
    setState(190);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PolyhedralSystemParser::NEWLINE) {
      setState(189);
      match(PolyhedralSystemParser::NEWLINE);
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

PolyhedralSystemParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedralSystemParser::AtomContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleAtom;
}

void PolyhedralSystemParser::AtomContext::copyFrom(AtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AtomEmptyContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::AtomEmptyContext::ID() {
  return getToken(PolyhedralSystemParser::ID, 0);
}

std::vector<tree::TerminalNode *> PolyhedralSystemParser::AtomEmptyContext::WS() {
  return getTokens(PolyhedralSystemParser::WS);
}

tree::TerminalNode* PolyhedralSystemParser::AtomEmptyContext::WS(size_t i) {
  return getToken(PolyhedralSystemParser::WS, i);
}

PolyhedralSystemParser::AtomEmptyContext::AtomEmptyContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::AtomEmptyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomEmpty(this);
}
void PolyhedralSystemParser::AtomEmptyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomEmpty(this);
}

std::any PolyhedralSystemParser::AtomEmptyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitAtomEmpty(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomPolyContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::AtomPolyContext::ID() {
  return getToken(PolyhedralSystemParser::ID, 0);
}

PolyhedralSystemParser::PolyContext* PolyhedralSystemParser::AtomPolyContext::poly() {
  return getRuleContext<PolyhedralSystemParser::PolyContext>(0);
}

PolyhedralSystemParser::AtomPolyContext::AtomPolyContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::AtomPolyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomPoly(this);
}
void PolyhedralSystemParser::AtomPolyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomPoly(this);
}

std::any PolyhedralSystemParser::AtomPolyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitAtomPoly(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomPowersetContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::AtomPowersetContext::ID() {
  return getToken(PolyhedralSystemParser::ID, 0);
}

PolyhedralSystemParser::PowersetContext* PolyhedralSystemParser::AtomPowersetContext::powerset() {
  return getRuleContext<PolyhedralSystemParser::PowersetContext>(0);
}

tree::TerminalNode* PolyhedralSystemParser::AtomPowersetContext::NEWLINE() {
  return getToken(PolyhedralSystemParser::NEWLINE, 0);
}

PolyhedralSystemParser::AtomPowersetContext::AtomPowersetContext(AtomContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::AtomPowersetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomPowerset(this);
}
void PolyhedralSystemParser::AtomPowersetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomPowerset(this);
}

std::any PolyhedralSystemParser::AtomPowersetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitAtomPowerset(this);
  else
    return visitor->visitChildren(this);
}
PolyhedralSystemParser::AtomContext* PolyhedralSystemParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 6, PolyhedralSystemParser::RuleAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(208);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomPowersetContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(192);
      match(PolyhedralSystemParser::ID);
      setState(193);
      powerset();
      setState(195);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedralSystemParser::NEWLINE) {
        setState(194);
        match(PolyhedralSystemParser::NEWLINE);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomEmptyContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(197);
      match(PolyhedralSystemParser::ID);
      setState(198);
      match(PolyhedralSystemParser::T__0);
      setState(202);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::WS) {
        setState(199);
        match(PolyhedralSystemParser::WS);
        setState(204);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(205);
      match(PolyhedralSystemParser::T__1);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomPolyContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(206);
      match(PolyhedralSystemParser::ID);
      setState(207);
      poly();
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

PolyhedralSystemParser::PowersetContext::PowersetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PolyhedralSystemParser::PolyContext *> PolyhedralSystemParser::PowersetContext::poly() {
  return getRuleContexts<PolyhedralSystemParser::PolyContext>();
}

PolyhedralSystemParser::PolyContext* PolyhedralSystemParser::PowersetContext::poly(size_t i) {
  return getRuleContext<PolyhedralSystemParser::PolyContext>(i);
}


size_t PolyhedralSystemParser::PowersetContext::getRuleIndex() const {
  return PolyhedralSystemParser::RulePowerset;
}

void PolyhedralSystemParser::PowersetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPowerset(this);
}

void PolyhedralSystemParser::PowersetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPowerset(this);
}


std::any PolyhedralSystemParser::PowersetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitPowerset(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::PowersetContext* PolyhedralSystemParser::powerset() {
  PowersetContext *_localctx = _tracker.createInstance<PowersetContext>(_ctx, getState());
  enterRule(_localctx, 8, PolyhedralSystemParser::RulePowerset);
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
    setState(210);
    match(PolyhedralSystemParser::T__2);
    setState(214);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::T__0) {
      setState(211);
      poly();
      setState(216);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(217);
    match(PolyhedralSystemParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PolyContext ------------------------------------------------------------------

PolyhedralSystemParser::PolyContext::PolyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PolyhedralSystemParser::ConstrContext *> PolyhedralSystemParser::PolyContext::constr() {
  return getRuleContexts<PolyhedralSystemParser::ConstrContext>();
}

PolyhedralSystemParser::ConstrContext* PolyhedralSystemParser::PolyContext::constr(size_t i) {
  return getRuleContext<PolyhedralSystemParser::ConstrContext>(i);
}

std::vector<tree::TerminalNode *> PolyhedralSystemParser::PolyContext::NEWLINE() {
  return getTokens(PolyhedralSystemParser::NEWLINE);
}

tree::TerminalNode* PolyhedralSystemParser::PolyContext::NEWLINE(size_t i) {
  return getToken(PolyhedralSystemParser::NEWLINE, i);
}


size_t PolyhedralSystemParser::PolyContext::getRuleIndex() const {
  return PolyhedralSystemParser::RulePoly;
}

void PolyhedralSystemParser::PolyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPoly(this);
}

void PolyhedralSystemParser::PolyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPoly(this);
}


std::any PolyhedralSystemParser::PolyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitPoly(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::PolyContext* PolyhedralSystemParser::poly() {
  PolyContext *_localctx = _tracker.createInstance<PolyContext>(_ctx, getState());
  enterRule(_localctx, 10, PolyhedralSystemParser::RulePoly);
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
    setState(219);
    match(PolyhedralSystemParser::T__0);
    setState(220);
    constr();
    setState(228);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::T__4) {
      setState(221);
      match(PolyhedralSystemParser::T__4);
      setState(223);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedralSystemParser::NEWLINE) {
        setState(222);
        match(PolyhedralSystemParser::NEWLINE);
      }
      setState(225);
      constr();
      setState(230);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(231);
    match(PolyhedralSystemParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstrContext ------------------------------------------------------------------

PolyhedralSystemParser::ConstrContext::ConstrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PolyhedralSystemParser::LinearExprContext *> PolyhedralSystemParser::ConstrContext::linearExpr() {
  return getRuleContexts<PolyhedralSystemParser::LinearExprContext>();
}

PolyhedralSystemParser::LinearExprContext* PolyhedralSystemParser::ConstrContext::linearExpr(size_t i) {
  return getRuleContext<PolyhedralSystemParser::LinearExprContext>(i);
}

tree::TerminalNode* PolyhedralSystemParser::ConstrContext::LE() {
  return getToken(PolyhedralSystemParser::LE, 0);
}

tree::TerminalNode* PolyhedralSystemParser::ConstrContext::LT() {
  return getToken(PolyhedralSystemParser::LT, 0);
}

tree::TerminalNode* PolyhedralSystemParser::ConstrContext::GT() {
  return getToken(PolyhedralSystemParser::GT, 0);
}

tree::TerminalNode* PolyhedralSystemParser::ConstrContext::GE() {
  return getToken(PolyhedralSystemParser::GE, 0);
}

tree::TerminalNode* PolyhedralSystemParser::ConstrContext::EQ() {
  return getToken(PolyhedralSystemParser::EQ, 0);
}


size_t PolyhedralSystemParser::ConstrContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleConstr;
}

void PolyhedralSystemParser::ConstrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstr(this);
}

void PolyhedralSystemParser::ConstrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstr(this);
}


std::any PolyhedralSystemParser::ConstrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitConstr(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::ConstrContext* PolyhedralSystemParser::constr() {
  ConstrContext *_localctx = _tracker.createInstance<ConstrContext>(_ctx, getState());
  enterRule(_localctx, 12, PolyhedralSystemParser::RuleConstr);
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
    setState(233);
    linearExpr();
    setState(234);
    antlrcpp::downCast<ConstrContext *>(_localctx)->op = _input->LT(1);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 507904) != 0))) {
      antlrcpp::downCast<ConstrContext *>(_localctx)->op = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(235);
    linearExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LinearExprContext ------------------------------------------------------------------

PolyhedralSystemParser::LinearExprContext::LinearExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PolyhedralSystemParser::TermContext* PolyhedralSystemParser::LinearExprContext::term() {
  return getRuleContext<PolyhedralSystemParser::TermContext>(0);
}

std::vector<PolyhedralSystemParser::SignTermContext *> PolyhedralSystemParser::LinearExprContext::signTerm() {
  return getRuleContexts<PolyhedralSystemParser::SignTermContext>();
}

PolyhedralSystemParser::SignTermContext* PolyhedralSystemParser::LinearExprContext::signTerm(size_t i) {
  return getRuleContext<PolyhedralSystemParser::SignTermContext>(i);
}

tree::TerminalNode* PolyhedralSystemParser::LinearExprContext::PLUS() {
  return getToken(PolyhedralSystemParser::PLUS, 0);
}

tree::TerminalNode* PolyhedralSystemParser::LinearExprContext::MINUS() {
  return getToken(PolyhedralSystemParser::MINUS, 0);
}


size_t PolyhedralSystemParser::LinearExprContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleLinearExpr;
}

void PolyhedralSystemParser::LinearExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLinearExpr(this);
}

void PolyhedralSystemParser::LinearExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLinearExpr(this);
}


std::any PolyhedralSystemParser::LinearExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitLinearExpr(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::LinearExprContext* PolyhedralSystemParser::linearExpr() {
  LinearExprContext *_localctx = _tracker.createInstance<LinearExprContext>(_ctx, getState());
  enterRule(_localctx, 14, PolyhedralSystemParser::RuleLinearExpr);
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
    setState(238);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PolyhedralSystemParser::PLUS

    || _la == PolyhedralSystemParser::MINUS) {
      setState(237);
      antlrcpp::downCast<LinearExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == PolyhedralSystemParser::PLUS

      || _la == PolyhedralSystemParser::MINUS)) {
        antlrcpp::downCast<LinearExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(240);
    term();
    setState(244);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::PLUS

    || _la == PolyhedralSystemParser::MINUS) {
      setState(241);
      signTerm();
      setState(246);
      _errHandler->sync(this);
      _la = _input->LA(1);
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

PolyhedralSystemParser::SignTermContext::SignTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PolyhedralSystemParser::TermContext* PolyhedralSystemParser::SignTermContext::term() {
  return getRuleContext<PolyhedralSystemParser::TermContext>(0);
}

tree::TerminalNode* PolyhedralSystemParser::SignTermContext::PLUS() {
  return getToken(PolyhedralSystemParser::PLUS, 0);
}

tree::TerminalNode* PolyhedralSystemParser::SignTermContext::MINUS() {
  return getToken(PolyhedralSystemParser::MINUS, 0);
}


size_t PolyhedralSystemParser::SignTermContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleSignTerm;
}

void PolyhedralSystemParser::SignTermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSignTerm(this);
}

void PolyhedralSystemParser::SignTermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSignTerm(this);
}


std::any PolyhedralSystemParser::SignTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitSignTerm(this);
  else
    return visitor->visitChildren(this);
}

PolyhedralSystemParser::SignTermContext* PolyhedralSystemParser::signTerm() {
  SignTermContext *_localctx = _tracker.createInstance<SignTermContext>(_ctx, getState());
  enterRule(_localctx, 16, PolyhedralSystemParser::RuleSignTerm);
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
    setState(247);
    antlrcpp::downCast<SignTermContext *>(_localctx)->op = _input->LT(1);
    _la = _input->LA(1);
    if (!(_la == PolyhedralSystemParser::PLUS

    || _la == PolyhedralSystemParser::MINUS)) {
      antlrcpp::downCast<SignTermContext *>(_localctx)->op = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(248);
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

PolyhedralSystemParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t PolyhedralSystemParser::TermContext::getRuleIndex() const {
  return PolyhedralSystemParser::RuleTerm;
}

void PolyhedralSystemParser::TermContext::copyFrom(TermContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- IntTimesVarContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::IntTimesVarContext::UNSIGNED_INT() {
  return getToken(PolyhedralSystemParser::UNSIGNED_INT, 0);
}

tree::TerminalNode* PolyhedralSystemParser::IntTimesVarContext::ID() {
  return getToken(PolyhedralSystemParser::ID, 0);
}

tree::TerminalNode* PolyhedralSystemParser::IntTimesVarContext::TIMES() {
  return getToken(PolyhedralSystemParser::TIMES, 0);
}

PolyhedralSystemParser::IntTimesVarContext::IntTimesVarContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::IntTimesVarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIntTimesVar(this);
}
void PolyhedralSystemParser::IntTimesVarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIntTimesVar(this);
}

std::any PolyhedralSystemParser::IntTimesVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitIntTimesVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::VarContext::ID() {
  return getToken(PolyhedralSystemParser::ID, 0);
}

PolyhedralSystemParser::VarContext::VarContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::VarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar(this);
}
void PolyhedralSystemParser::VarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar(this);
}

std::any PolyhedralSystemParser::VarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntContext ------------------------------------------------------------------

tree::TerminalNode* PolyhedralSystemParser::IntContext::UNSIGNED_INT() {
  return getToken(PolyhedralSystemParser::UNSIGNED_INT, 0);
}

PolyhedralSystemParser::IntContext::IntContext(TermContext *ctx) { copyFrom(ctx); }

void PolyhedralSystemParser::IntContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInt(this);
}
void PolyhedralSystemParser::IntContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PolyhedralSystemListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInt(this);
}

std::any PolyhedralSystemParser::IntContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<PolyhedralSystemVisitor*>(visitor))
    return parserVisitor->visitInt(this);
  else
    return visitor->visitChildren(this);
}
PolyhedralSystemParser::TermContext* PolyhedralSystemParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 18, PolyhedralSystemParser::RuleTerm);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(257);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::IntTimesVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(250);
      match(PolyhedralSystemParser::UNSIGNED_INT);
      setState(252);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedralSystemParser::TIMES) {
        setState(251);
        match(PolyhedralSystemParser::TIMES);
      }
      setState(254);
      match(PolyhedralSystemParser::ID);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::VarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(255);
      match(PolyhedralSystemParser::ID);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::IntContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(256);
      match(PolyhedralSystemParser::UNSIGNED_INT);
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

void PolyhedralSystemParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  polyhedralsystemParserInitialize();
#else
  ::antlr4::internal::call_once(polyhedralsystemParserOnceFlag, polyhedralsystemParserInitialize);
#endif
}
