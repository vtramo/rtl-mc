
// Generated from ./g4/PolyhedralSystem.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedralSystemParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by PolyhedralSystemParser.
 */
class  PolyhedralSystemVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by PolyhedralSystemParser.
   */
    virtual std::any visitSystem(PolyhedralSystemParser::SystemContext *context) = 0;

    virtual std::any visitInv(PolyhedralSystemParser::InvContext *context) = 0;

    virtual std::any visitFlow(PolyhedralSystemParser::FlowContext *context) = 0;

    virtual std::any visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext *context) = 0;

    virtual std::any visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext *context) = 0;

    virtual std::any visitAtomPoly(PolyhedralSystemParser::AtomPolyContext *context) = 0;

    virtual std::any visitPowerset(PolyhedralSystemParser::PowersetContext *context) = 0;

    virtual std::any visitPoly(PolyhedralSystemParser::PolyContext *context) = 0;

    virtual std::any visitConstr(PolyhedralSystemParser::ConstrContext *context) = 0;

    virtual std::any visitLinearExpr(PolyhedralSystemParser::LinearExprContext *context) = 0;

    virtual std::any visitSignTerm(PolyhedralSystemParser::SignTermContext *context) = 0;

    virtual std::any visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext *context) = 0;

    virtual std::any visitVar(PolyhedralSystemParser::VarContext *context) = 0;

    virtual std::any visitInt(PolyhedralSystemParser::IntContext *context) = 0;


};

