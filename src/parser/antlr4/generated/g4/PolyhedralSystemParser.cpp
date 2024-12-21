
// Generated from g4/PolyhedralSystem.g4 by ANTLR 4.13.2


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
  	4,1,21,204,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,1,0,4,0,22,8,0,11,0,12,0,23,1,0,1,0,5,0,28,8,0,
  	10,0,12,0,31,9,0,1,0,1,0,5,0,35,8,0,10,0,12,0,38,9,0,1,0,5,0,41,8,0,10,
  	0,12,0,44,9,0,1,0,1,0,4,0,48,8,0,11,0,12,0,49,1,0,1,0,5,0,54,8,0,10,0,
  	12,0,57,9,0,1,0,5,0,60,8,0,10,0,12,0,63,9,0,1,0,1,0,5,0,67,8,0,10,0,12,
  	0,70,9,0,1,0,1,0,4,0,74,8,0,11,0,12,0,75,1,0,4,0,79,8,0,11,0,12,0,80,
  	1,0,1,0,5,0,85,8,0,10,0,12,0,88,9,0,1,0,1,0,5,0,92,8,0,10,0,12,0,95,9,
  	0,1,0,5,0,98,8,0,10,0,12,0,101,9,0,1,0,1,0,4,0,105,8,0,11,0,12,0,106,
  	1,0,1,0,5,0,111,8,0,10,0,12,0,114,9,0,1,0,5,0,117,8,0,10,0,12,0,120,9,
  	0,1,0,1,0,5,0,124,8,0,10,0,12,0,127,9,0,1,0,1,0,4,0,131,8,0,11,0,12,0,
  	132,3,0,135,8,0,1,1,1,1,1,1,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,5,3,148,8,
  	3,10,3,12,3,151,9,3,1,3,1,3,1,3,3,3,156,8,3,1,4,1,4,5,4,160,8,4,10,4,
  	12,4,163,9,4,1,4,1,4,1,5,1,5,1,5,1,5,5,5,171,8,5,10,5,12,5,174,9,5,1,
  	5,1,5,1,6,1,6,1,6,1,6,1,7,3,7,183,8,7,1,7,1,7,5,7,187,8,7,10,7,12,7,190,
  	9,7,1,8,1,8,1,8,1,9,1,9,3,9,197,8,9,1,9,1,9,1,9,3,9,202,8,9,1,9,0,0,10,
  	0,2,4,6,8,10,12,14,16,18,0,2,1,0,14,18,1,0,12,13,226,0,134,1,0,0,0,2,
  	136,1,0,0,0,4,139,1,0,0,0,6,155,1,0,0,0,8,157,1,0,0,0,10,166,1,0,0,0,
  	12,177,1,0,0,0,14,182,1,0,0,0,16,191,1,0,0,0,18,201,1,0,0,0,20,22,3,6,
  	3,0,21,20,1,0,0,0,22,23,1,0,0,0,23,21,1,0,0,0,23,24,1,0,0,0,24,25,1,0,
  	0,0,25,29,3,4,2,0,26,28,3,6,3,0,27,26,1,0,0,0,28,31,1,0,0,0,29,27,1,0,
  	0,0,29,30,1,0,0,0,30,32,1,0,0,0,31,29,1,0,0,0,32,36,3,2,1,0,33,35,3,6,
  	3,0,34,33,1,0,0,0,35,38,1,0,0,0,36,34,1,0,0,0,36,37,1,0,0,0,37,135,1,
  	0,0,0,38,36,1,0,0,0,39,41,3,6,3,0,40,39,1,0,0,0,41,44,1,0,0,0,42,40,1,
  	0,0,0,42,43,1,0,0,0,43,45,1,0,0,0,44,42,1,0,0,0,45,47,3,4,2,0,46,48,3,
  	6,3,0,47,46,1,0,0,0,48,49,1,0,0,0,49,47,1,0,0,0,49,50,1,0,0,0,50,51,1,
  	0,0,0,51,55,3,2,1,0,52,54,3,6,3,0,53,52,1,0,0,0,54,57,1,0,0,0,55,53,1,
  	0,0,0,55,56,1,0,0,0,56,135,1,0,0,0,57,55,1,0,0,0,58,60,3,6,3,0,59,58,
  	1,0,0,0,60,63,1,0,0,0,61,59,1,0,0,0,61,62,1,0,0,0,62,64,1,0,0,0,63,61,
  	1,0,0,0,64,68,3,4,2,0,65,67,3,6,3,0,66,65,1,0,0,0,67,70,1,0,0,0,68,66,
  	1,0,0,0,68,69,1,0,0,0,69,71,1,0,0,0,70,68,1,0,0,0,71,73,3,2,1,0,72,74,
  	3,6,3,0,73,72,1,0,0,0,74,75,1,0,0,0,75,73,1,0,0,0,75,76,1,0,0,0,76,135,
  	1,0,0,0,77,79,3,6,3,0,78,77,1,0,0,0,79,80,1,0,0,0,80,78,1,0,0,0,80,81,
  	1,0,0,0,81,82,1,0,0,0,82,86,3,2,1,0,83,85,3,6,3,0,84,83,1,0,0,0,85,88,
  	1,0,0,0,86,84,1,0,0,0,86,87,1,0,0,0,87,89,1,0,0,0,88,86,1,0,0,0,89,93,
  	3,4,2,0,90,92,3,6,3,0,91,90,1,0,0,0,92,95,1,0,0,0,93,91,1,0,0,0,93,94,
  	1,0,0,0,94,135,1,0,0,0,95,93,1,0,0,0,96,98,3,6,3,0,97,96,1,0,0,0,98,101,
  	1,0,0,0,99,97,1,0,0,0,99,100,1,0,0,0,100,102,1,0,0,0,101,99,1,0,0,0,102,
  	104,3,2,1,0,103,105,3,6,3,0,104,103,1,0,0,0,105,106,1,0,0,0,106,104,1,
  	0,0,0,106,107,1,0,0,0,107,108,1,0,0,0,108,112,3,4,2,0,109,111,3,6,3,0,
  	110,109,1,0,0,0,111,114,1,0,0,0,112,110,1,0,0,0,112,113,1,0,0,0,113,135,
  	1,0,0,0,114,112,1,0,0,0,115,117,3,6,3,0,116,115,1,0,0,0,117,120,1,0,0,
  	0,118,116,1,0,0,0,118,119,1,0,0,0,119,121,1,0,0,0,120,118,1,0,0,0,121,
  	125,3,2,1,0,122,124,3,6,3,0,123,122,1,0,0,0,124,127,1,0,0,0,125,123,1,
  	0,0,0,125,126,1,0,0,0,126,128,1,0,0,0,127,125,1,0,0,0,128,130,3,4,2,0,
  	129,131,3,6,3,0,130,129,1,0,0,0,131,132,1,0,0,0,132,130,1,0,0,0,132,133,
  	1,0,0,0,133,135,1,0,0,0,134,21,1,0,0,0,134,42,1,0,0,0,134,61,1,0,0,0,
  	134,78,1,0,0,0,134,99,1,0,0,0,134,118,1,0,0,0,135,1,1,0,0,0,136,137,5,
  	20,0,0,137,138,3,8,4,0,138,3,1,0,0,0,139,140,5,19,0,0,140,141,3,10,5,
  	0,141,5,1,0,0,0,142,143,5,21,0,0,143,156,3,8,4,0,144,145,5,21,0,0,145,
  	149,5,1,0,0,146,148,5,8,0,0,147,146,1,0,0,0,148,151,1,0,0,0,149,147,1,
  	0,0,0,149,150,1,0,0,0,150,152,1,0,0,0,151,149,1,0,0,0,152,156,5,2,0,0,
  	153,154,5,21,0,0,154,156,3,10,5,0,155,142,1,0,0,0,155,144,1,0,0,0,155,
  	153,1,0,0,0,156,7,1,0,0,0,157,161,5,3,0,0,158,160,3,10,5,0,159,158,1,
  	0,0,0,160,163,1,0,0,0,161,159,1,0,0,0,161,162,1,0,0,0,162,164,1,0,0,0,
  	163,161,1,0,0,0,164,165,5,4,0,0,165,9,1,0,0,0,166,167,5,1,0,0,167,172,
  	3,12,6,0,168,169,5,5,0,0,169,171,3,12,6,0,170,168,1,0,0,0,171,174,1,0,
  	0,0,172,170,1,0,0,0,172,173,1,0,0,0,173,175,1,0,0,0,174,172,1,0,0,0,175,
  	176,5,2,0,0,176,11,1,0,0,0,177,178,3,14,7,0,178,179,7,0,0,0,179,180,3,
  	14,7,0,180,13,1,0,0,0,181,183,7,1,0,0,182,181,1,0,0,0,182,183,1,0,0,0,
  	183,184,1,0,0,0,184,188,3,18,9,0,185,187,3,16,8,0,186,185,1,0,0,0,187,
  	190,1,0,0,0,188,186,1,0,0,0,188,189,1,0,0,0,189,15,1,0,0,0,190,188,1,
  	0,0,0,191,192,7,1,0,0,192,193,3,18,9,0,193,17,1,0,0,0,194,196,5,9,0,0,
  	195,197,5,11,0,0,196,195,1,0,0,0,196,197,1,0,0,0,197,198,1,0,0,0,198,
  	202,5,21,0,0,199,202,5,21,0,0,200,202,5,9,0,0,201,194,1,0,0,0,201,199,
  	1,0,0,0,201,200,1,0,0,0,202,19,1,0,0,0,27,23,29,36,42,49,55,61,68,75,
  	80,86,93,99,106,112,118,125,132,134,149,155,161,172,182,188,196,201
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
    setState(134);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(21); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(20);
        atom();
        setState(23); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(25);
      flow();
      setState(29);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(26);
        atom();
        setState(31);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(32);
      inv();
      setState(36);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(33);
        atom();
        setState(38);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(42);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(39);
        atom();
        setState(44);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(45);
      flow();
      setState(47); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(46);
        atom();
        setState(49); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(51);
      inv();
      setState(55);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(52);
        atom();
        setState(57);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(58);
        atom();
        setState(63);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(64);
      flow();
      setState(68);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(65);
        atom();
        setState(70);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(71);
      inv();
      setState(73); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(72);
        atom();
        setState(75); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(78); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(77);
        atom();
        setState(80); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(82);
      inv();
      setState(86);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(83);
        atom();
        setState(88);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(89);
      flow();
      setState(93);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(90);
        atom();
        setState(95);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(99);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(96);
        atom();
        setState(101);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(102);
      inv();
      setState(104); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(103);
        atom();
        setState(106); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PolyhedralSystemParser::ID);
      setState(108);
      flow();
      setState(112);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(109);
        atom();
        setState(114);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(118);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(115);
        atom();
        setState(120);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(121);
      inv();
      setState(125);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::ID) {
        setState(122);
        atom();
        setState(127);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(128);
      flow();
      setState(130); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(129);
        atom();
        setState(132); 
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

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(PolyhedralSystemParser::INV);
    setState(137);
    powerset();
   
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

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    match(PolyhedralSystemParser::FLOW);
    setState(140);
    poly();
   
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
    setState(155);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomPowersetContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(142);
      match(PolyhedralSystemParser::ID);
      setState(143);
      powerset();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomEmptyContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(144);
      match(PolyhedralSystemParser::ID);
      setState(145);
      match(PolyhedralSystemParser::T__0);
      setState(149);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == PolyhedralSystemParser::WS) {
        setState(146);
        match(PolyhedralSystemParser::WS);
        setState(151);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(152);
      match(PolyhedralSystemParser::T__1);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::AtomPolyContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(153);
      match(PolyhedralSystemParser::ID);
      setState(154);
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
    setState(157);
    match(PolyhedralSystemParser::T__2);
    setState(161);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::T__0) {
      setState(158);
      poly();
      setState(163);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(164);
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
    setState(166);
    match(PolyhedralSystemParser::T__0);
    setState(167);
    constr();
    setState(172);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::T__4) {
      setState(168);
      match(PolyhedralSystemParser::T__4);
      setState(169);
      constr();
      setState(174);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(175);
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
    setState(177);
    linearExpr();
    setState(178);
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
    setState(179);
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
    setState(182);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PolyhedralSystemParser::PLUS

    || _la == PolyhedralSystemParser::MINUS) {
      setState(181);
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
    setState(184);
    term();
    setState(188);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PolyhedralSystemParser::PLUS

    || _la == PolyhedralSystemParser::MINUS) {
      setState(185);
      signTerm();
      setState(190);
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
    setState(191);
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
    setState(192);
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
    setState(201);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::IntTimesVarContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(194);
      match(PolyhedralSystemParser::UNSIGNED_INT);
      setState(196);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PolyhedralSystemParser::TIMES) {
        setState(195);
        match(PolyhedralSystemParser::TIMES);
      }
      setState(198);
      match(PolyhedralSystemParser::ID);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::VarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(199);
      match(PolyhedralSystemParser::ID);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PolyhedralSystemParser::IntContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(200);
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
