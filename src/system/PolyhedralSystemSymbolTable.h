#pragma once

#include "ppl_aliases.h"
#include <optional>
#include <spot/tl/apcollect.hh>

/*!
 * \class PolyhedralSystemSymbolTable
 * \brief Manages the mapping between user-defined variable names and their corresponding dimensions in the state space.
 *
 * The \c PolyhedralSystemSymbolTable class is responsible for maintaining the association between user-defined variable names
 * and their corresponding dimensions in the state space. It provides a bidirectional mapping between:
 * - User-defined variable names (e.g., "x", "y", "z")
 * - Internal PPL (Parma Polyhedra Library) variables
 *
 * It also manages a set of atomic propositions, which are used to define regions of interest in the state space.
 * Typically, a \c PolyhedralSystemSymbolTable is constructed after the parsing of a polyhedral system specification.
 * The \c PolyhedralSystemSymbolTableListener is responsible for collecting user-defined variable names and atomic proposition
 * names from the specification and using them to build a \c PolyhedralSystemSymbolTable. This table is then used
 * throughout the system to ensure consistent naming and interpretation of variables and propositions.
 *
 * \note The symbol table plays a crucial role in constructing and interpreting polyhedral systems, as it acts as a bridge
 *       between human-readable variable names and the internal representation used by the PPL (Parma Polyhedra Library).
 *       It enables the translation of polyhedral objects (such as \c Poly or \c Powerset) into human-readable output
 *       by providing meaningful variable names. This is particularly useful when using the \c toString functions
 *       defined in the \c PPLOutput namespace, which rely on the symbol table to generate readable representations
 *       of polyhedra and other geometric objects. Moreover, it is used to construct a \c PolyhedralSystem during
 *       its parsing process.
 *
 * \see PolyhedralSystemSymbolTableListener
 * \see PolyhedralSystemBuilderVisitor
 */
class PolyhedralSystemSymbolTable {
public:
    /*!
     * \brief Adds a new variable with a user-defined name to the symbol table.
     * \param id The user-defined name of the variable.
     * \return A reference to the current symbol table for method chaining.
     */
    PolyhedralSystemSymbolTable& addVariable(std::string_view id);

    /*!
     * \brief Adds multiple variables with user-defined names to the symbol table.
     * \param ids A list of user-defined variable names.
     * \return A reference to the current symbol table for method chaining.
     */
    PolyhedralSystemSymbolTable& addVariables(std::initializer_list<std::string_view> ids);

    /*!
     * \brief Adds an atomic proposition to the symbol table.
     * \param atom The atomic proposition to add.
     * \return A reference to the current symbol table for method chaining.
     */
    PolyhedralSystemSymbolTable& addAtom(const spot::formula& atom);

    /*!
     * \brief Adds an atomic proposition by name to the symbol table.
     * \param atom The name of the atomic proposition to add.
     * \return A reference to the current symbol table for method chaining.
     */
    PolyhedralSystemSymbolTable& addAtom(std::string_view atom);

    /*!
     * \brief Adds multiple atomic propositions by name to the symbol table.
     * \param atoms A list of atomic proposition names to add.
     * \return A reference to the current symbol table for method chaining.
     */
    PolyhedralSystemSymbolTable& addAtoms(std::initializer_list<std::string_view> atoms);

    /*!
     * \brief Checks if a variable with the given name exists in the symbol table.
     * \param id The user-defined name of the variable to check.
     * \return \c true if the variable exists, \c false otherwise.
     */
    [[nodiscard]] bool containsVariable(std::string_view id) const;

    /*!
     * \brief Checks if an atomic proposition with the given name exists in the symbol table.
     * \param atom The name of the atomic proposition to check.
     * \return \c true if the atomic proposition exists, \c false otherwise.
     */
    [[nodiscard]] bool containsAtom(std::string_view atom) const;

    /*!
     * \brief Returns the set of atomic propositions managed by the symbol table.
     * \return A reference to the set of atomic propositions.
     */
    [[nodiscard]] const spot::atomic_prop_set& atoms() const;

    /*!
     * \brief Retrieves the PPL variable corresponding to a user-defined variable name.
     * \param id The user-defined name of the variable.
     * \return An optional containing the PPL variable if the name exists, or \c std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<PPL::Variable> getVariable(std::string_view id) const;

    /*!
     * \brief Retrieves the user-defined name corresponding to a PPL variable.
     * \param variable The PPL variable.
     * \return An optional containing the user-defined name if the variable exists, or \c std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::string> getVariableName(const PPL::Variable& variable) const;

    /*!
     * \brief Returns a list of all user-defined variable names in the symbol table.
     * \return A vector of variable names.
     */
    [[nodiscard]] std::vector<std::string> getVariableNames() const;

    /*!
     * \brief Returns the total number of dimensions in the state space.
     * \return The space dimension as a \f$\text{dimension\_type}\f$.
     */
    [[nodiscard]] PPL::dimension_type getSpaceDimension() const;

    /*!
     * \brief Returns the total number of atomic propositions in the symbol table.
     * \return The number of atomic propositions.
     */
    [[nodiscard]] int getTotalAtoms() const;

    /*!
     * \brief Returns the mapping between space dimensions and user-defined variable names.
     * \return An unordered map that associates each space dimension with a corresponding variable name.
     */
    [[nodiscard]] std::unordered_map<PPL::dimension_type, std::string> getVariableNameBySpaceDimension() const
    {
        return m_idBySpaceDimension;
    }

    friend bool operator== (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);
    friend bool operator!= (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);

private:
    std::unordered_map<std::string, PPL::Variable> m_variableById {}; ///< Maps user-defined variable names to PPL variables.
    std::unordered_map<PPL::dimension_type, std::string> m_idBySpaceDimension {}; ///< Maps dimensions to user-defined variable names.
    PPL::dimension_type m_dimensions {}; ///< The total number of dimensions
    spot::atomic_prop_set m_atoms {}; ///< The set of atomic propositions managed by the symbol table.
};