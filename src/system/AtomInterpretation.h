#pragma once

#include "ppl_aliases.h"

/*!
 * \class AtomInterpretation
 * \brief Represents the interpretation of an atomic proposition in a polyhedral system.
 *
 * The \c AtomInterpretation class encapsulates the interpretation of an atomic proposition as a polyhedron.
 * It stores both the interpretation (the set of points where the proposition holds)
 * and its complement (the set of points where the proposition does not hold).
 *
 * The interpretation of the atomic proposition can optionally be intersected with the system's invariant region.
 * If \f$[p]\f$ is the \p interpretation of the atomic proposition and if \p invariant \f$ (\mathit{Inv})\f$ is provided, then:
 * \f[
 * [p]_{\mathit{Inv}} = [p]\,\cap\,\mathit{Inv}
 * \f]
 * \f[
 * \overline{[p]} = \mathit{Inv} \, \setminus \, [p]_{\mathit{Inv}}
 * \f]
 *
 * \see PolyhedralSystem
 */
class AtomInterpretation {
public:
    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation and a default invariant (universe).
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     */
    explicit AtomInterpretation(const Powerset& interpretation);

    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation (using move semantics) and a default invariant (universe).
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     */
    explicit AtomInterpretation(Powerset&& interpretation);

    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation and invariant.
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     * \param invariant The invariant region of the polyhedral system.
     */
    explicit AtomInterpretation(const Powerset& interpretation, const Powerset& invariant);

    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation and invariant (using move semantics for both).
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     * \param invariant The invariant region of the polyhedral system.
     */
    explicit AtomInterpretation(Powerset&& interpretation, Powerset&& invariant);

    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation (using move semantics) and invariant.
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     * \param invariant The invariant region of the polyhedral system.
     */
    explicit AtomInterpretation(Powerset&& interpretation, const Powerset& invariant);

    /*!
     * \brief Constructs an \c AtomInterpretation with the given interpretation and invariant (using move semantics for the invariant).
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     * \param invariant The invariant region of the polyhedral system.
     */
    explicit AtomInterpretation(const Powerset& interpretation, Powerset&& invariant);

    AtomInterpretation(const AtomInterpretation& interpretation) = default;
    AtomInterpretation& operator= (const AtomInterpretation& interpretation) = delete;

    AtomInterpretation(AtomInterpretation&& interpretation) noexcept;
    AtomInterpretation& operator= (AtomInterpretation&& interpretation) = delete;

    /*!
     * \brief Returns the interpretation of the atomic proposition.
     * \return A reference to the polyhedron representing the interpretation.
     */
    [[nodiscard]] const Powerset& interpretation() const;

    /*!
     * \brief Returns the complement of the interpretation (the set of points where the proposition does not hold).
     * \return A reference to the polyhedron representing the complement of the interpretation.
     */
    [[nodiscard]] const Powerset& notInterpretation() const;
private:
    Powerset m_interpretation { PPL::EMPTY }; ///< The polyhedron representing the interpretation of the atomic proposition.
    Powerset m_notInterpretation { PPL::EMPTY }; ///< The polyhedron representing the complement of the interpretation.
};

bool operator== (const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2);
bool operator!= (const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2);
std::ostream& operator<< (std::ostream& out, const AtomInterpretation& interpretation);