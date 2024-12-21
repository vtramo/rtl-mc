
// Generated from g4/PolyhedralSystem.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedralSystemParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by PolyhedralSystemParser.
 */
class  PolyhedralSystemListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterSystem(PolyhedralSystemParser::SystemContext *ctx) = 0;
  virtual void exitSystem(PolyhedralSystemParser::SystemContext *ctx) = 0;

  virtual void enterInv(PolyhedralSystemParser::InvContext *ctx) = 0;
  virtual void exitInv(PolyhedralSystemParser::InvContext *ctx) = 0;

  virtual void enterFlow(PolyhedralSystemParser::FlowContext *ctx) = 0;
  virtual void exitFlow(PolyhedralSystemParser::FlowContext *ctx) = 0;

  virtual void enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext *ctx) = 0;
  virtual void exitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext *ctx) = 0;

  virtual void enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext *ctx) = 0;
  virtual void exitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext *ctx) = 0;

  virtual void enterAtomPoly(PolyhedralSystemParser::AtomPolyContext *ctx) = 0;
  virtual void exitAtomPoly(PolyhedralSystemParser::AtomPolyContext *ctx) = 0;

  virtual void enterPowerset(PolyhedralSystemParser::PowersetContext *ctx) = 0;
  virtual void exitPowerset(PolyhedralSystemParser::PowersetContext *ctx) = 0;

  virtual void enterPoly(PolyhedralSystemParser::PolyContext *ctx) = 0;
  virtual void exitPoly(PolyhedralSystemParser::PolyContext *ctx) = 0;

  virtual void enterConstr(PolyhedralSystemParser::ConstrContext *ctx) = 0;
  virtual void exitConstr(PolyhedralSystemParser::ConstrContext *ctx) = 0;

  virtual void enterLinearExpr(PolyhedralSystemParser::LinearExprContext *ctx) = 0;
  virtual void exitLinearExpr(PolyhedralSystemParser::LinearExprContext *ctx) = 0;

  virtual void enterSignTerm(PolyhedralSystemParser::SignTermContext *ctx) = 0;
  virtual void exitSignTerm(PolyhedralSystemParser::SignTermContext *ctx) = 0;

  virtual void enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext *ctx) = 0;
  virtual void exitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext *ctx) = 0;

  virtual void enterVar(PolyhedralSystemParser::VarContext *ctx) = 0;
  virtual void exitVar(PolyhedralSystemParser::VarContext *ctx) = 0;

  virtual void enterInt(PolyhedralSystemParser::IntContext *ctx) = 0;
  virtual void exitInt(PolyhedralSystemParser::IntContext *ctx) = 0;


};

