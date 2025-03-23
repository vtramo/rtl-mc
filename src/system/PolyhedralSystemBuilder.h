#pragma once

#include "AtomInterpretation.h"
#include "PolyhedralSystemSymbolTable.h"

class PolyhedralSystem;

/*!
 * \class PolyhedralSystemBuilder
 * \brief A builder class for constructing \ref PolyhedralSystem objects.
 *
 * The \c PolyhedralSystemBuilder class is responsible for constructing \ref PolyhedralSystem objects in a step-by-step manner.
 * It ensures that all required components (flow, invariant, denotation, and symbol table) are properly set and consistent
 * before building the final \ref PolyhedralSystem object.
 *
 * \note A \ref PolyhedralSystem object can only be constructed using this builder class. Direct construction of
 *       \ref PolyhedralSystem objects is not allowed.
 *
 * \see PolyhedralSystem
 */
class PolyhedralSystemBuilder {
public:
    /*!
     * \brief Sets the flow constraint for the polyhedral system.
     * \param flow The flow constraint as a \ref Poly object.
     * \return A reference to the current builder for method chaining.
     */
    PolyhedralSystemBuilder& flow(const Poly& flow);

    /*!
     * \overload
     * \see PolyhedralSystemBuilder& flow(const Poly& flow)
     */
    PolyhedralSystemBuilder& flow(Poly&& flow);

    /*!
     * \brief Sets the invariant region for the polyhedral system.
     * \param invariant The invariant region as a \ref Powerset object.
     * \return A reference to the current builder for method chaining.
     */
    PolyhedralSystemBuilder& invariant(const Powerset& invariant);

    /*!
     * \overload
     * \see PolyhedralSystemBuilder& invariant(const Powerset& invariant)
     */
    PolyhedralSystemBuilder& invariant(Powerset&& invariant);

    /*!
     * \brief Sets the denotation map for the polyhedral system.
     * \param denotation A map from atomic propositions to their interpretations.
     * \return A reference to the current builder for method chaining.
     */
    PolyhedralSystemBuilder& denotation(const std::unordered_map<spot::formula, Powerset>& denotation);

    /*!
     * \overload
     * \see PolyhedralSystemBuilder& denotation(const std::unordered_map<spot::formula, Powerset>& denotation)
     */
    PolyhedralSystemBuilder& denotation(std::unordered_map<spot::formula, Powerset>&& denotation);

    /*!
     * \brief Sets the symbol table for the polyhedral system.
     * \param polyhedralSystemSymbolTable The symbol table as a \ref PolyhedralSystemSymbolTable object.
     * \return A reference to the current builder for method chaining.
     */
    PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable);

    /*!
     * \overload
     * \see PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable)
     */
    PolyhedralSystemBuilder& symbolTable(PolyhedralSystemSymbolTable&& polyhedralSystemSymbolTable);

    /*!
     * \brief Builds the \ref PolyhedralSystem object.
     * \return The constructed \ref PolyhedralSystem object.
     * \throws std::invalid_argument If any required component (flow, invariant, denotation, or symbol table) is not set.
     * \throws std::invalid_argument If the space dimensions of the components are inconsistent.
     */
    [[nodiscard]] PolyhedralSystem build() const;

    explicit PolyhedralSystemBuilder(const PolyhedralSystem& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (const PolyhedralSystem& polyhedralSystem) = delete;

    explicit PolyhedralSystemBuilder(PolyhedralSystem&& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (PolyhedralSystem&& polyhedralSystem) = delete;

    PolyhedralSystemBuilder() = default;
    ~PolyhedralSystemBuilder() = default;

private:
    std::unique_ptr<PolyhedralSystemSymbolTable> m_symbolTable { nullptr };
    std::unique_ptr<std::unordered_map<spot::formula, Powerset>> m_denotation { nullptr };
    std::unique_ptr<Powerset> m_invariant { nullptr };
    std::unique_ptr<Poly> m_flow { nullptr };

    /*!
     * \brief Builds the denotation map with \ref AtomInterpretation objects.
     * \return A map from atomic propositions to their interpretations.
     */
    std::unordered_map<spot::formula, AtomInterpretation> buildDenotation() const;

    /*!
     * \brief Builds the \ref PolyhedralSystem object using the set components.
     * \return The constructed \ref PolyhedralSystem object.
     */
    PolyhedralSystem buildPolyhedralSystem() const;

    /*!
     * \brief Asserts that the flow constraint is set.
     * \throws std::invalid_argument If the flow constraint is not set.
     */
    void assertInvariantIsNotNull() const;

    /*!
     * \brief Asserts that the invariant region is set.
     * \throws std::invalid_argument If the invariant region is not set.
     */
    void assertSymbolTableIsNotNull() const;

    /*!
     * \brief Asserts that the symbol table is set.
     * \throws std::invalid_argument If the symbol table is not set.
     */
    void assertFlowIsNotNull() const;

    /*!
     * \brief Asserts that the denotation map is set.
     * \throws std::invalid_argument If the denotation map is not set.
     */
    void assertDenotationIsNotNull() const;

    /*!
     * \brief Asserts that all components have consistent space dimensions.
     * \throws std::invalid_argument If the space dimensions of the components are inconsistent.
     */
    void assertThatAllDimensionsAreEqual() const;
};