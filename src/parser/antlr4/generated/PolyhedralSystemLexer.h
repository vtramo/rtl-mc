
// Generated from ./g4/PolyhedralSystem.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  PolyhedralSystemLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, LINE_COMMENT = 6, 
    COMMENT = 7, WS = 8, UNSIGNED_INT = 9, NEWLINE = 10, TIMES = 11, PLUS = 12, 
    MINUS = 13, LE = 14, LT = 15, GT = 16, GE = 17, EQ = 18, FLOW = 19, 
    INV = 20, ID = 21
  };

  explicit PolyhedralSystemLexer(antlr4::CharStream *input);

  ~PolyhedralSystemLexer() override;


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

