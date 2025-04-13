
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  PolyhedraParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, LINE_COMMENT = 7, 
    COMMENT = 8, WS = 9, UNSIGNED_INT = 10, NEWLINE = 11, FLOAT = 12, TIMES = 13, 
    PLUS = 14, MINUS = 15, LE = 16, LT = 17, GT = 18, GE = 19, EQ = 20, 
    ZERO = 21, FLOW = 22, INV = 23, VARID = 24, HEX_COLOR_CODE = 25
  };

  enum {
    RulePolyhedra = 0, RuleAtom = 1, RulePowerset = 2, RulePoly = 3, RuleConstr = 4, 
    RuleLinearExpr = 5, RuleSignTerm = 6, RuleTerm = 7
  };

  explicit PolyhedraParser(antlr4::TokenStream *input);

  PolyhedraParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~PolyhedraParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class PolyhedraContext;
  class AtomContext;
  class PowersetContext;
  class PolyContext;
  class ConstrContext;
  class LinearExprContext;
  class SignTermContext;
  class TermContext; 

  class  PolyhedraContext : public antlr4::ParserRuleContext {
  public:
    PolyhedraContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AtomContext *> atom();
    AtomContext* atom(size_t i);
    std::vector<antlr4::tree::TerminalNode *> WS();
    antlr4::tree::TerminalNode* WS(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PolyhedraContext* polyhedra();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtomContext() = default;
    void copyFrom(AtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AtomEmptyContext : public AtomContext {
  public:
    AtomEmptyContext(AtomContext *ctx);

    antlr4::tree::TerminalNode *VARID();
    antlr4::tree::TerminalNode *HEX_COLOR_CODE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomPolyContext : public AtomContext {
  public:
    AtomPolyContext(AtomContext *ctx);

    antlr4::tree::TerminalNode *VARID();
    PolyContext *poly();
    antlr4::tree::TerminalNode *HEX_COLOR_CODE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomErrorContext : public AtomContext {
  public:
    AtomErrorContext(AtomContext *ctx);

    antlr4::tree::TerminalNode *VARID();
    std::vector<PolyContext *> poly();
    PolyContext* poly(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomPowersetContext : public AtomContext {
  public:
    AtomPowersetContext(AtomContext *ctx);

    antlr4::tree::TerminalNode *VARID();
    PowersetContext *powerset();
    antlr4::tree::TerminalNode *HEX_COLOR_CODE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtomContext* atom();

  class  PowersetContext : public antlr4::ParserRuleContext {
  public:
    PowersetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PowersetContext() = default;
    void copyFrom(PowersetContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  PowersetTrueContext : public PowersetContext {
  public:
    PowersetTrueContext(PowersetContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PowersetEmptyOrNotEmptyContext : public PowersetContext {
  public:
    PowersetEmptyOrNotEmptyContext(PowersetContext *ctx);

    std::vector<PolyContext *> poly();
    PolyContext* poly(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PowersetErrorContext : public PowersetContext {
  public:
    PowersetErrorContext(PowersetContext *ctx);

    std::vector<PolyContext *> poly();
    PolyContext* poly(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PowersetContext* powerset();

  class  PolyContext : public antlr4::ParserRuleContext {
  public:
    PolyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PolyContext() = default;
    void copyFrom(PolyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  PolyAtLeastOneConstrContext : public PolyContext {
  public:
    PolyAtLeastOneConstrContext(PolyContext *ctx);

    std::vector<ConstrContext *> constr();
    ConstrContext* constr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PolyTrueContext : public PolyContext {
  public:
    PolyTrueContext(PolyContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PolyErrorContext : public PolyContext {
  public:
    PolyErrorContext(PolyContext *ctx);

    std::vector<ConstrContext *> constr();
    ConstrContext* constr(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PolyContext* poly();

  class  ConstrContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    ConstrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LinearExprContext *> linearExpr();
    LinearExprContext* linearExpr(size_t i);
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *GE();
    antlr4::tree::TerminalNode *EQ();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstrContext* constr();

  class  LinearExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    LinearExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TermContext *term();
    std::vector<SignTermContext *> signTerm();
    SignTermContext* signTerm(size_t i);
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearExprContext* linearExpr();

  class  SignTermContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    SignTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TermContext *term();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignTermContext* signTerm();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    TermContext() = default;
    void copyFrom(TermContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  IntTimesVarContext : public TermContext {
  public:
    IntTimesVarContext(TermContext *ctx);

    antlr4::tree::TerminalNode *UNSIGNED_INT();
    antlr4::tree::TerminalNode *VARID();
    antlr4::tree::TerminalNode *TIMES();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarContext : public TermContext {
  public:
    VarContext(TermContext *ctx);

    antlr4::tree::TerminalNode *VARID();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TermErrorContext : public TermContext {
  public:
    TermErrorContext(TermContext *ctx);

    antlr4::Token *op = nullptr;
    antlr4::tree::TerminalNode *UNSIGNED_INT();
    antlr4::tree::TerminalNode *VARID();
    antlr4::tree::TerminalNode *TIMES();
    antlr4::tree::TerminalNode *FLOAT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntContext : public TermContext {
  public:
    IntContext(TermContext *ctx);

    antlr4::tree::TerminalNode *UNSIGNED_INT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TermContext* term();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

