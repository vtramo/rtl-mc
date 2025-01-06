#ifndef POLYHEDRALSYSTEMBUILDERVISITOR_H
#define POLYHEDRALSYSTEMBUILDERVISITOR_H

#include "PolyhedralSystem.h"
#include "PolyhedralSystemSymbolTable.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemBaseVisitor.h"

class PolyhedralSystemBuilderVisitor final
{
public:
    explicit PolyhedralSystemBuilderVisitor(PolyhedralSystemSymbolTable& symbolTable);
    PolyhedralSystem buildPolyhedralSystem(PolyhedralSystemParser::SystemContext* parseTree);

private:
    class PolyhedralSystemVisitor final : public PolyhedralSystemBaseVisitor
    {

    public:
        explicit PolyhedralSystemVisitor(PolyhedralSystemSymbolTable& symbolTable);

        std::any visitSystem(PolyhedralSystemParser::SystemContext* ctx) override;
        std::any visitInv(PolyhedralSystemParser::InvContext* ctx) override;
        std::any visitFlow(PolyhedralSystemParser::FlowContext* ctx) override;

        std::any visitAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx) override;
        std::any visitAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx) override;
        std::any visitAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx) override;

        std::any visitPowersetEmptyOrNotEmpty(PolyhedralSystemParser::PowersetEmptyOrNotEmptyContext* ctx) override;
        std::any visitPowersetTrue(PolyhedralSystemParser::PowersetTrueContext* ctx) override;

        std::any visitPolyAtLeastOneConstr(PolyhedralSystemParser::PolyAtLeastOneConstrContext* ctx) override;
        std::any visitPolyTrue(PolyhedralSystemParser::PolyTrueContext* ctx) override;

        std::any visitConstr(PolyhedralSystemParser::ConstrContext* ctx) override;

        std::any visitLinearExpr(PolyhedralSystemParser::LinearExprContext* ctx) override;
        std::any visitIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* context) override;
        std::any visitSignTerm(PolyhedralSystemParser::SignTermContext* ctx) override;
        std::any visitVar(PolyhedralSystemParser::VarContext* context) override;
        std::any visitInt(PolyhedralSystemParser::IntContext* context) override;

        [[nodiscard]] std::unordered_map<std::string, Powerset> getDenotation() const;
        [[nodiscard]] Powerset getInvariant() const;
        [[nodiscard]] Poly getFlow() const;
        [[nodiscard]] PolyhedralSystemSymbolTable getSymbolTable() const;

    private:
        int m_visitKey {};
        std::unordered_map<int, std::unique_ptr<Powerset>> m_powersets {};
        std::unordered_map<int, std::unique_ptr<Poly>> m_polyhedra {};
        std::unordered_map<int, std::unique_ptr<PPL::Linear_Expression>> m_linearExpressions {};
        std::unordered_map<int, std::unique_ptr<PPL::Constraint>> m_constraints {};

        std::reference_wrapper<PolyhedralSystemSymbolTable> m_symbolTable;
        std::unordered_map<std::string, Powerset> m_denotation {};
        Powerset m_invariant {};
        Poly m_flow {};

        std::unique_ptr<PPL::Constraint> popConstraint(int visitKey);
        std::unique_ptr<PPL::Linear_Expression> popLinearExpression(int visitKey);
        std::unique_ptr<Poly> popPoly(int visitKey);
        std::unique_ptr<Powerset> popPowerset(int visitKey);
    };

    PolyhedralSystemVisitor m_visitor;
};

#endif //POLYHEDRALSYSTEMBUILDERVISITOR_H
