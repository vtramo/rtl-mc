
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedraParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by PolyhedraParser.
 */
class  PolyhedraListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterPolyhedra(PolyhedraParser::PolyhedraContext *ctx) = 0;
  virtual void exitPolyhedra(PolyhedraParser::PolyhedraContext *ctx) = 0;

  virtual void enterAtomPowerset(PolyhedraParser::AtomPowersetContext *ctx) = 0;
  virtual void exitAtomPowerset(PolyhedraParser::AtomPowersetContext *ctx) = 0;

  virtual void enterAtomEmpty(PolyhedraParser::AtomEmptyContext *ctx) = 0;
  virtual void exitAtomEmpty(PolyhedraParser::AtomEmptyContext *ctx) = 0;

  virtual void enterAtomPoly(PolyhedraParser::AtomPolyContext *ctx) = 0;
  virtual void exitAtomPoly(PolyhedraParser::AtomPolyContext *ctx) = 0;

  virtual void enterAtomError(PolyhedraParser::AtomErrorContext *ctx) = 0;
  virtual void exitAtomError(PolyhedraParser::AtomErrorContext *ctx) = 0;

  virtual void enterPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext *ctx) = 0;
  virtual void exitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext *ctx) = 0;

  virtual void enterPowersetTrue(PolyhedraParser::PowersetTrueContext *ctx) = 0;
  virtual void exitPowersetTrue(PolyhedraParser::PowersetTrueContext *ctx) = 0;

  virtual void enterPowersetError(PolyhedraParser::PowersetErrorContext *ctx) = 0;
  virtual void exitPowersetError(PolyhedraParser::PowersetErrorContext *ctx) = 0;

  virtual void enterPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext *ctx) = 0;
  virtual void exitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext *ctx) = 0;

  virtual void enterPolyTrue(PolyhedraParser::PolyTrueContext *ctx) = 0;
  virtual void exitPolyTrue(PolyhedraParser::PolyTrueContext *ctx) = 0;

  virtual void enterPolyError(PolyhedraParser::PolyErrorContext *ctx) = 0;
  virtual void exitPolyError(PolyhedraParser::PolyErrorContext *ctx) = 0;

  virtual void enterConstr(PolyhedraParser::ConstrContext *ctx) = 0;
  virtual void exitConstr(PolyhedraParser::ConstrContext *ctx) = 0;

  virtual void enterLinearExpr(PolyhedraParser::LinearExprContext *ctx) = 0;
  virtual void exitLinearExpr(PolyhedraParser::LinearExprContext *ctx) = 0;

  virtual void enterSignTerm(PolyhedraParser::SignTermContext *ctx) = 0;
  virtual void exitSignTerm(PolyhedraParser::SignTermContext *ctx) = 0;

  virtual void enterIntTimesVar(PolyhedraParser::IntTimesVarContext *ctx) = 0;
  virtual void exitIntTimesVar(PolyhedraParser::IntTimesVarContext *ctx) = 0;

  virtual void enterVar(PolyhedraParser::VarContext *ctx) = 0;
  virtual void exitVar(PolyhedraParser::VarContext *ctx) = 0;

  virtual void enterInt(PolyhedraParser::IntContext *ctx) = 0;
  virtual void exitInt(PolyhedraParser::IntContext *ctx) = 0;

  virtual void enterTermError(PolyhedraParser::TermErrorContext *ctx) = 0;
  virtual void exitTermError(PolyhedraParser::TermErrorContext *ctx) = 0;


};

