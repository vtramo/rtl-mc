
// Generated from g4/PolyhedralSystem.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedralSystemVisitor.h"


/**
 * This class provides an empty implementation of PolyhedralSystemVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  PolyhedralSystemBaseVisitor : public PolyhedralSystemVisitor {
public:

  virtual std::any visitSystem(PolyhedralSystemParser::SystemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInv(PolyhedralSystemParser::InvContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFlow(PolyhedralSystemParser::FlowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtomPoly(PolyhedralSystemParser::AtomPolyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPowerset(PolyhedralSystemParser::PowersetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPoly(PolyhedralSystemParser::PolyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstr(PolyhedralSystemParser::ConstrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLinearExpr(PolyhedralSystemParser::LinearExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignTerm(PolyhedralSystemParser::SignTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar(PolyhedralSystemParser::VarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInt(PolyhedralSystemParser::IntContext *ctx) override {
    return visitChildren(ctx);
  }


};

