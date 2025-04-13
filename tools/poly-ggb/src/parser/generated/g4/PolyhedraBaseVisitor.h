
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedraVisitor.h"


/**
 * This class provides an empty implementation of PolyhedraVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  PolyhedraBaseVisitor : public PolyhedraVisitor {
public:

  virtual std::any visitPolyhedra(PolyhedraParser::PolyhedraContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomPowerset(PolyhedraParser::AtomPowersetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomEmpty(PolyhedraParser::AtomEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomPoly(PolyhedraParser::AtomPolyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomError(PolyhedraParser::AtomErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPowersetTrue(PolyhedraParser::PowersetTrueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPowersetError(PolyhedraParser::PowersetErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPolyTrue(PolyhedraParser::PolyTrueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPolyError(PolyhedraParser::PolyErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstr(PolyhedraParser::ConstrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLinearExpr(PolyhedraParser::LinearExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignTerm(PolyhedraParser::SignTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntTimesVar(PolyhedraParser::IntTimesVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar(PolyhedraParser::VarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInt(PolyhedraParser::IntContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTermError(PolyhedraParser::TermErrorContext *ctx) override {
    return visitChildren(ctx);
  }


};

