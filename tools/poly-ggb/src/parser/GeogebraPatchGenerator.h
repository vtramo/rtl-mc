#pragma once

#include "PolyhedraBaseVisitor.h"
#include "SimpleSymbolTable.h"
#include "ParserError.h"
#include "GeogebraPatch.h"

class GeogebraPatchGenerator
{
public:
    explicit GeogebraPatchGenerator(SimpleSymbolTable& symbolTable);

    /*!
     * \brief Checks if any errors were encountered during the parsing process.
     * \return \c true if errors were encountered, \c false otherwise.
     */
    [[nodiscard]] bool hasErrors() const;

    std::vector<GeogebraPatch> generateGeogebraPatches(PolyhedraParser::PolyhedraContext* parseTree);

    /*!
    * \brief Retrieves the list of errors encountered during the parsing process.
    * \return A vector of ParserError objects.
    */
    [[nodiscard]] const std::vector<ParserError>& errors() const;

private:
    class GeogebraPatchVisitor final : public PolyhedraBaseVisitor
    {
    public:
        explicit GeogebraPatchVisitor(SimpleSymbolTable& symbolTable);

        std::any visitPolyhedra(PolyhedraParser::PolyhedraContext* ctx) override;
        std::any visitAtomPowerset(PolyhedraParser::AtomPowersetContext* ctx) override;
        std::any visitAtomPoly(PolyhedraParser::AtomPolyContext* ctx) override;
        std::any visitAtomEmpty(PolyhedraParser::AtomEmptyContext* ctx) override;

        std::any visitPowersetEmptyOrNotEmpty(PolyhedraParser::PowersetEmptyOrNotEmptyContext* ctx) override;
        std::any visitPowersetTrue(PolyhedraParser::PowersetTrueContext* ctx) override;

        std::any visitPolyAtLeastOneConstr(PolyhedraParser::PolyAtLeastOneConstrContext* ctx) override;
        std::any visitPolyTrue(PolyhedraParser::PolyTrueContext* ctx) override;

        std::any visitConstr(PolyhedraParser::ConstrContext* ctx) override;

        std::any visitLinearExpr(PolyhedraParser::LinearExprContext* ctx) override;
        std::any visitIntTimesVar(PolyhedraParser::IntTimesVarContext* context) override;
        std::any visitSignTerm(PolyhedraParser::SignTermContext* ctx) override;
        std::any visitVar(PolyhedraParser::VarContext* context) override;
        std::any visitInt(PolyhedraParser::IntContext* context) override;

        friend class GeogebraPatchGenerator;

    private:
        int m_visitKey {}; ///< Key used to track visited nodes.
        std::vector<ParserError> m_errors {};
        std::reference_wrapper<SimpleSymbolTable> m_symbolTable;
        std::unordered_map<int, Powerset> m_powersets {};
        std::unordered_map<int, Poly> m_patches {};
        std::unordered_map<int, PPL::Constraint> m_constraints {};
        std::unordered_map<int, PPL::Linear_Expression> m_linearExpressions {};
        std::unordered_map<spot::formula, std::pair<Powerset, RGBColor>> m_denotation {};

        bool containsAtom(const spot::formula& atom) const;
        bool containsAtom(std::string_view atom) const;
        void addDuplicateAtomParserError(antlr4::tree::TerminalNode*);
        Poly popPatch(int visitKey);
        Powerset popPowerset(int visitKey);
        PPL::Constraint popConstraint(int visitKey);
        PPL::Linear_Expression popLinearExpression(int visitKey);
    };

    static std::string buildGeogebraPatchLabel(spot::formula ap, int countPatch);

    GeogebraPatchVisitor m_visitor;
};