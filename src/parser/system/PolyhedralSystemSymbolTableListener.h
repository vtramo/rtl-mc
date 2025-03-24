#pragma once

#include "PolyhedralSystemBaseListener.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemSymbolTable.h"

/*!
 * \class PolyhedralSystemSymbolTableListener
 * \brief A listener for building the \c PolyhedralSystemSymbolTable during the parse tree visit by collecting user-defined variables and atomic propositions.
 *
 * The \c PolyhedralSystemSymbolTableListener class extends \c PolyhedralSystemBaseListener and is used during the parse tree visit
 * to collect user-defined variables and atomic propositions. These are stored in a \c PolyhedralSystemSymbolTable.
 *
 * \note This listener is typically used in conjunction with an ANTLR4 parser to traverse the parse tree and extract symbols.
 *
 * \see PolyhedralSystemSymbolTable
 */
class PolyhedralSystemSymbolTableListener final : public PolyhedralSystemBaseListener
{
public:
    /*!
     * \brief Called when entering an \c IntTimesVarContext node in the parse tree.
     *
     * This method is invoked when encountering a term like \c 3*x or \c 3x in a linear expression.
     * It extracts the variable identifier and adds it to the symbol table.
     *
     * \param ctx The \c IntTimesVarContext node.
     */
    void enterIntTimesVar(PolyhedralSystemParser::IntTimesVarContext* ctx) override;

    /*!
     * \brief Called when entering a \c VarContext node in the parse tree.
     *
     * This method is invoked when encountering a variable (e.g., \c x, \c y, \c z) in the input.
     * It extracts the variable identifier and adds it to the symbol table.
     *
     * \param ctx The \c VarContext node.
     */
    void enterVar(PolyhedralSystemParser::VarContext* ctx) override;

    /*!
     * \brief Called when entering an \c AtomPowersetContext node in the parse tree.
     *
     * This method is invoked when encountering the definition of an atomic proposition
     * whose interpretation is defined as a \c Powerset (a union of polyhedra).
     * It extracts the atomic proposition identifier and adds it to the symbol table.
     *
     * \param ctx The \c AtomPowersetContext node.
     */
    void enterAtomPowerset(PolyhedralSystemParser::AtomPowersetContext* ctx) override;

    /*!
     * \brief Called when entering an \c AtomEmptyContext node in the parse tree.
     *
     * This method is invoked when encountering the definition of an atomic proposition
     * whose interpretation is defined as empty (i.e., no points satisfy the proposition).
     * It extracts the atomic proposition identifier and adds it to the symbol table.
     *
     * \param ctx The \c AtomEmptyContext node.
     */
    void enterAtomEmpty(PolyhedralSystemParser::AtomEmptyContext* ctx) override;

    /*!
     * \brief Called when entering an \c AtomPolyContext node in the parse tree.
     *
     * This method is invoked when encountering the definition of an atomic proposition
     * whose interpretation is defined as a single polyhedron.
     * It extracts the atomic proposition identifier and adds it to the symbol table.
     *
     * \param ctx The \c AtomPolyContext node.
     */
    void enterAtomPoly(PolyhedralSystemParser::AtomPolyContext* ctx) override;

    /*!
     * \brief Returns the constructed symbol table.
     * \return The \c PolyhedralSystemSymbolTable containing the collected variables and atomic propositions.
     */
    [[nodiscard]] PolyhedralSystemSymbolTable getSymbolTable() const;

private:
    PolyhedralSystemSymbolTable m_symbolTable {}; ///< The symbol table being constructed.

    /*!
     * \brief Adds a variable to the symbol table.
     * \param varId The identifier of the variable to add.
     */
    void addVariable(std::string_view varId);

    /*!
     * \brief Adds an atomic proposition to the symbol table.
     * \param atomId The identifier of the atomic proposition to add.
     */
    void addAtom(std::string_view atomId);
};