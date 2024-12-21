
// Generated from g4/PolyhedralSystem.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedralSystemListener.h"


/**
 * This class provides an empty implementation of PolyhedralSystemListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  PolyhedralSystemBaseListener : public PolyhedralSystemListener {
public:

  virtual void enterSystem(PolyhedralSystemParser::SystemContext * /*ctx*/) override { }
  virtual void exitSystem(PolyhedralSystemParser::SystemContext * /*ctx*/) override { }

  virtual void enterInv(PolyhedralSystemParser::InvContext * /*ctx*/) override { }
  virtual void exitInv(PolyhedralSystemParser::InvContext * /*ctx*/) override { }

  virtual void enterFlow(PolyhedralSystemParser::FlowContext * /*ctx*/) override { }
  virtual void exitFlow(PolyhedralSystemParser::FlowContext * /*ctx*/) override { }

  virtual void enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext * /*ctx*/) override { }
  virtual void exitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext * /*ctx*/) override { }

  virtual void enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext * /*ctx*/) override { }
  virtual void exitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext * /*ctx*/) override { }

  virtual void enterAtomPoly(PolyhedralSystemParser::AtomPolyContext * /*ctx*/) override { }
  virtual void exitAtomPoly(PolyhedralSystemParser::AtomPolyContext * /*ctx*/) override { }

  virtual void enterPowerset(PolyhedralSystemParser::PowersetContext * /*ctx*/) override { }
  virtual void exitPowerset(PolyhedralSystemParser::PowersetContext * /*ctx*/) override { }

  virtual void enterPoly(PolyhedralSystemParser::PolyContext * /*ctx*/) override { }
  virtual void exitPoly(PolyhedralSystemParser::PolyContext * /*ctx*/) override { }

  virtual void enterConstr(PolyhedralSystemParser::ConstrContext * /*ctx*/) override { }
  virtual void exitConstr(PolyhedralSystemParser::ConstrContext * /*ctx*/) override { }

  virtual void enterLinearExpr(PolyhedralSystemParser::LinearExprContext * /*ctx*/) override { }
  virtual void exitLinearExpr(PolyhedralSystemParser::LinearExprContext * /*ctx*/) override { }

  virtual void enterSignTerm(PolyhedralSystemParser::SignTermContext * /*ctx*/) override { }
  virtual void exitSignTerm(PolyhedralSystemParser::SignTermContext * /*ctx*/) override { }

  virtual void enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext * /*ctx*/) override { }
  virtual void exitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext * /*ctx*/) override { }

  virtual void enterVar(PolyhedralSystemParser::VarContext * /*ctx*/) override { }
  virtual void exitVar(PolyhedralSystemParser::VarContext * /*ctx*/) override { }

  virtual void enterInt(PolyhedralSystemParser::IntContext * /*ctx*/) override { }
  virtual void exitInt(PolyhedralSystemParser::IntContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

