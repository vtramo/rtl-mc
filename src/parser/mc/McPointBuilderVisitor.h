#pragma once

#include "McPointBaseVisitor.h"
#include "PolyhedralSystemSymbolTable.h"
#include "ParserError.h"

class McPointBuilderVisitor final
{
public:
    explicit McPointBuilderVisitor(const PolyhedralSystemSymbolTable& symbolTable);
    PPL::Generator buildMcPoint(McPointParser::ArrayContext* parseTree);

    [[nodiscard]] bool hasErrors() const;
    PPL::Generator computeMcPoint();
    [[nodiscard]] const std::vector<ParserError>& errors() const;
private:
    class McPointVisitor final : public McPointBaseVisitor
    {
    public:
        friend class McPointBuilderVisitor;
        explicit McPointVisitor(const PolyhedralSystemSymbolTable& symbolTable);

        std::any visitValidArray(McPointParser::ValidArrayContext* context) override;
        std::any visitValidPair(McPointParser::ValidPairContext* context) override;
        std::any visitValidIntegerValue(McPointParser::ValidIntegerValueContext* context) override;
        std::any visitValidRationalValue(McPointParser::ValidRationalValueContext* context) override;
    private:
        using Rational = mpq_class;

        const PolyhedralSystemSymbolTable& m_symbolTable {};
        std::unordered_map<std::string, Rational> m_valueByVariable {};
        std::vector<ParserError> m_errors {};

        bool symbolTableHasVariable(std::string_view variable) const;
        void addDuplicateVariableParserError(antlr4::tree::TerminalNode* ctx);
        void addVariableNotPresentInPolySystemError(antlr4::tree::TerminalNode* ctx);
        void addMissingVariablesError(const McPointParser::ArrayContext* parseTree, std::vector<std::string>&& missingVariables);
        void addDivisionByZeroError(const McPointParser::ValidRationalValueContext* ctx);
    };

    McPointVisitor m_visitor;
};