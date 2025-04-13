
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedraListener.h"


/**
 * This class provides an empty implementation of PolyhedraListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  PolyhedraBaseListener : public PolyhedraListener {
public:

  virtual void enterPolyhedra(PolyhedraParser::PolyhedraContext * /*ctx*/) override { }
  virtual void exitPolyhedra(PolyhedraParser::PolyhedraContext * /*ctx*/) override { }

  virtual void enterAtomPowerset(PolyhedraParser::AtomPowersetContext * /*ctx*/) override { }
  virtual void exitAtomPowerset(PolyhedraParser::AtomPowersetContext * /*ctx*/) override { }

  virtual void enterAtomEmpty(PolyhedraParser::AtomEmptyContext * /*ctx*/) override { }
  virtual void exitAtomEmpty(PolyhedraParser::AtomEmptyContext * /*ctx*/) override { }

  virtual void enterAtomPoly(PolyhedraParser::AtomPolyContext * /*ctx*/) override { }
  virtual void exitAtomPoly(PolyhedraParser::AtomPolyContext * /*ctx*/) override { }

  virtual void enterAtomError(PolyhedraParser::AtomErrorContext * /*ctx*/) override { }
  virtual void exitAtomError(PolyhedraParser::AtomErrorContext * /*ctx*/) override { }

  virtual void enterPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext * /*ctx*/) override { }
  virtual void exitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext * /*ctx*/) override { }

  virtual void enterPowersetTrue(PolyhedraParser::PowersetTrueContext * /*ctx*/) override { }
  virtual void exitPowersetTrue(PolyhedraParser::PowersetTrueContext * /*ctx*/) override { }

  virtual void enterPowersetError(PolyhedraParser::PowersetErrorContext * /*ctx*/) override { }
  virtual void exitPowersetError(PolyhedraParser::PowersetErrorContext * /*ctx*/) override { }

  virtual void enterPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext * /*ctx*/) override { }
  virtual void exitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext * /*ctx*/) override { }

  virtual void enterPolyTrue(PolyhedraParser::PolyTrueContext * /*ctx*/) override { }
  virtual void exitPolyTrue(PolyhedraParser::PolyTrueContext * /*ctx*/) override { }

  virtual void enterPolyError(PolyhedraParser::PolyErrorContext * /*ctx*/) override { }
  virtual void exitPolyError(PolyhedraParser::PolyErrorContext * /*ctx*/) override { }

  virtual void enterConstr(PolyhedraParser::ConstrContext * /*ctx*/) override { }
  virtual void exitConstr(PolyhedraParser::ConstrContext * /*ctx*/) override { }

  virtual void enterLinearExpr(PolyhedraParser::LinearExprContext * /*ctx*/) override { }
  virtual void exitLinearExpr(PolyhedraParser::LinearExprContext * /*ctx*/) override { }

  virtual void enterSignTerm(PolyhedraParser::SignTermContext * /*ctx*/) override { }
  virtual void exitSignTerm(PolyhedraParser::SignTermContext * /*ctx*/) override { }

  virtual void enterIntTimesVar(PolyhedraParser::IntTimesVarContext * /*ctx*/) override { }
  virtual void exitIntTimesVar(PolyhedraParser::IntTimesVarContext * /*ctx*/) override { }

  virtual void enterVar(PolyhedraParser::VarContext * /*ctx*/) override { }
  virtual void exitVar(PolyhedraParser::VarContext * /*ctx*/) override { }

  virtual void enterInt(PolyhedraParser::IntContext * /*ctx*/) override { }
  virtual void exitInt(PolyhedraParser::IntContext * /*ctx*/) override { }

  virtual void enterTermError(PolyhedraParser::TermErrorContext * /*ctx*/) override { }
  virtual void exitTermError(PolyhedraParser::TermErrorContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

