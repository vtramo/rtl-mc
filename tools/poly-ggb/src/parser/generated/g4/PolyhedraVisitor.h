
// Generated from g4/Polyhedra.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "PolyhedraParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by PolyhedraParser.
 */
class  PolyhedraVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by PolyhedraParser.
   */
    virtual std::any visitPolyhedra(PolyhedraParser::PolyhedraContext *context) = 0;

    virtual std::any visitAtomPowerset(PolyhedraParser::AtomPowersetContext *context) = 0;

    virtual std::any visitAtomEmpty(PolyhedraParser::AtomEmptyContext *context) = 0;

    virtual std::any visitAtomPoly(PolyhedraParser::AtomPolyContext *context) = 0;

    virtual std::any visitAtomError(PolyhedraParser::AtomErrorContext *context) = 0;

    virtual std::any visitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext *context) = 0;

    virtual std::any visitPowersetTrue(PolyhedraParser::PowersetTrueContext *context) = 0;

    virtual std::any visitPowersetError(PolyhedraParser::PowersetErrorContext *context) = 0;

    virtual std::any visitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext *context) = 0;

    virtual std::any visitPolyTrue(PolyhedraParser::PolyTrueContext *context) = 0;

    virtual std::any visitPolyError(PolyhedraParser::PolyErrorContext *context) = 0;

    virtual std::any visitConstr(PolyhedraParser::ConstrContext *context) = 0;

    virtual std::any visitLinearExpr(PolyhedraParser::LinearExprContext *context) = 0;

    virtual std::any visitSignTerm(PolyhedraParser::SignTermContext *context) = 0;

    virtual std::any visitIntTimesVar(PolyhedraParser::IntTimesVarContext *context) = 0;

    virtual std::any visitVar(PolyhedraParser::VarContext *context) = 0;

    virtual std::any visitInt(PolyhedraParser::IntContext *context) = 0;

    virtual std::any visitTermError(PolyhedraParser::TermErrorContext *context) = 0;


};

