/*!
* \file McPointBuilderVisitor.h
 * \brief Visitor for parsing and building rational points (a.k.a. model-checking points) from input strings.
 *
 * This visitor handles the parsing of rational points specified in the format:
 * [var1=value1, var2=value2, ...] where values can be integers or fractions.
 * The parsed points are converted to PPL (Parma Polyhedra Library) generators.
 */
#pragma once

#include "McPointBaseVisitor.h"
#include "PolyhedralSystemSymbolTable.h"
#include "ParserError.h"

/*!
 * \class McPointBuilderVisitor
 * \brief Main visitor class for parsing rational points.
 *
 * The visitor validates and converts string representations of rational points (e.g., [x=1/3, y=-2/3, z=1])
 * into PPL generators. It checks for:
 * - Syntax errors in the input format
 * - Undefined variables
 * - Duplicate variable assignments
 * - Division by zero
 * - Missing required variables
 *
 */
class McPointBuilderVisitor final
{
public:
    /*!
     * \brief Constructs a visitor with a symbol table reference.
     * \param symbolTable The symbol table containing variable definitions.
     */
    explicit McPointBuilderVisitor(const PolyhedralSystemSymbolTable& symbolTable);

    /*!
     * \brief Parses and builds a rational point from the parse tree.
     * \param parseTree The ANTLR parse tree of the point specification.
     * \return PPL generator representing the point.
     *
     * The input should follow the format:
     * [x=1/3, y=-2/3, z=1]
     */
    PPL::Generator buildMcPoint(McPointParser::ArrayContext* parseTree);

    /*!
     * \brief Checks if any errors occurred during parsing.
     * \return \c true if errors were found, \c false otherwise.
     */
    [[nodiscard]] bool hasErrors() const;
    [[nodiscard]] const std::vector<ParserError>& errors() const;
private:

    /*!
     * \brief Computes the final point after successful parsing.
     * \return PPL generator representing the point.
     * \pre Requires that parsing was successful (\ref McPointBuilderVisitor::hasErrors() == \c false)
     * \throws McPointComputationError if called when errors are present or if computation fails
     */
    PPL::Generator computeMcPoint();

    /*!
     * \class McPointVisitor
     * \brief Internal visitor class for traversing the parse tree and constructing the model-checking point (rational point).
     */
    class McPointVisitor final : public McPointBaseVisitor
    {
    public:
        friend class McPointBuilderVisitor;

        /*!
         * \brief Constructs the internal visitor with symbol table.
         * \param symbolTable Reference to the symbol table.
         */
        explicit McPointVisitor(const PolyhedralSystemSymbolTable& symbolTable);

        /*!
         * \brief Visits a valid array node in the parse tree.
         */
        std::any visitValidArray(McPointParser::ValidArrayContext* context) override;

        /*!
         * \brief Visits a valid variable=value pair.
         */
        std::any visitValidPair(McPointParser::ValidPairContext* context) override;

        /*!
         * \brief Visits an integer value (e.g., "5" or "-3").
         */
        std::any visitValidIntegerValue(McPointParser::ValidIntegerValueContext* context) override;

        /*!
         * \brief Visits a rational value (e.g., "1/2" or "-3/4").
         */
        std::any visitValidRationalValue(McPointParser::ValidRationalValueContext* context) override;
    private:
        using Rational = mpq_class; ///< Type for rational number representation

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