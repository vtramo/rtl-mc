
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  PolyhedraLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, LINE_COMMENT = 7, 
    COMMENT = 8, WS = 9, UNSIGNED_INT = 10, NEWLINE = 11, FLOAT = 12, TIMES = 13, 
    PLUS = 14, MINUS = 15, LE = 16, LT = 17, GT = 18, GE = 19, EQ = 20, 
    ZERO = 21, FLOW = 22, INV = 23, VARID = 24, HEX_COLOR_CODE = 25
  };

  explicit PolyhedraLexer(antlr4::CharStream *input);

  ~PolyhedraLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

