#pragma once

#include <spot/tl/formula.hh>
#include <spot/tl/apcollect.hh>
#include <optional>

#include "spot_utils.h"
#include "ppl_utils.h"
#include "utils.h"
#include "ppl_aliases.h"

using PPL::IO_Operators::operator<<;

/*!
 * \class Observable
 * \brief Represents an observable in a polyhedral system, consisting of a set of atomic propositions and an interpretation.
 *
 * An \c Observable is a set of atomic propositions \f$\alpha \subseteq \mathit{AP}\f$ along with its interpretation
 * \f$[\![\alpha]\!]\f$, which is a polyhedron representing the set of points where all the propositions
 * in \f$\alpha\f$ hold. The interpretation is stored as a \ref Powerset object, which can represent a union of convex polyhedra (a.k.a. patches).
 *
 * This class provides methods to query the properties of the observable, such as its atomic propositions, interpretation,
 * and geometric properties (e.g., whether it is empty or covers the entire space).
 *
 * \note Instances of \c Observable are immutable. Once constructed, their state cannot be modified.
 *
 * \see PolyhedralSystem
 */
class Observable {
public:

    /*!
     * \brief Constructs an \c Observable with the given atomic propositions and interpretation.
     * \param atoms The set of atomic propositions.
     * \param interpretation The interpretation of the observable as a \ref PowersetConstSharedPtr.
     */
    Observable(spot::atomic_prop_set atoms, PowersetConstSharedPtr interpretation)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::move(interpretation) }
    {
        computeHashCode();
    }

    /*!
     * \brief Constructs an \c Observable with the given atomic propositions and interpretation.
     * \param atoms The set of atomic propositions.
     * \param interpretation The interpretation of the observable as a const \ref Powerset& object.
     */
    Observable(spot::atomic_prop_set atoms, const Powerset& interpretation)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::make_shared<Powerset>(interpretation) }
    {
        computeHashCode();
    }

    /*!
     * \brief Constructs an \c Observable with the given atomic propositions, interpretation, and string representation.
     * \param atoms The set of atomic propositions.
     * \param interpretation The interpretation of the observable as a \ref PowersetConstSharedPtr.
     * \param interpretationToString A string representation of the interpretation.
     */
    Observable(spot::atomic_prop_set atoms, PowersetConstSharedPtr interpretation, std::string interpretationToString)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::move(interpretation) }
        , m_interpretationToString { std::move(interpretationToString) }
    {
        computeHashCode();
    }

    /*!
     * \brief Constructs an \c Observable with the given atomic propositions, interpretation, and string representation.
     * \param atoms The set of atomic propositions.
     * \param interpretation The interpretation of the observable as a const \ref Powerset& object.
     * \param interpretationToString A string representation of the interpretation.
     */
    Observable(spot::atomic_prop_set atoms, const Powerset& interpretation, std::string interpretationToString)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::make_shared<Powerset>(interpretation) }
        , m_interpretationToString { std::move(interpretationToString) }
    {
        computeHashCode();
    }

    /*!
     * \brief Default constructor.
     */
    Observable() = default;

    /*!
     * \brief Returns the set of atomic propositions in the observable.
     * \return A reference to the set of atomic propositions.
     */
    [[nodiscard]] const spot::atomic_prop_set& atoms() const { return *m_atoms; }

    /*!
     * \brief Returns the interpretation of the observable.
     * \return A shared pointer to the \ref Powerset representing the interpretation.
     */
    [[nodiscard]] PowersetConstSharedPtr interpretation() const { return m_interpretation; }

    /*!
     * \brief Checks if the interpretation covers the entire space (i.e., it is the universe).
     * \return \c true if the interpretation is the universe, \c false otherwise.
     */
    [[nodiscard]] bool isUniverse() const { return m_interpretation->is_universe(); }

    /*!
     * \brief Checks if the interpretation is empty.
     * \return \c true if the interpretation is empty, \c false otherwise.
     */
    [[nodiscard]] bool isEmpty() const { return m_interpretation->is_empty(); }

    /*!
     * \brief Returns the total number of atomic propositions in the observable.
     * \return The number of atomic propositions.
     */
    [[nodiscard]] int totalAtoms() const { return m_atoms->size(); }

    /*!
     * \brief Returns the total number of patches in the interpretation.
     * \return The number of patches in the \ref Powerset.
     */
    [[nodiscard]] int totalPatches() const { return m_interpretation->size(); }

    /*!
     * \brief Returns an iterator to the beginning of the interpretation's patches.
     * \return An iterator to the first patch in the \ref Powerset.
     */
    [[nodiscard]] Powerset::const_iterator begin() const { return m_interpretation->begin(); }

    /*!
      * \brief Returns an iterator to the end of the interpretation's patches.
      * \return An iterator to the end of the \ref Powerset.
      */
    [[nodiscard]] Powerset::const_iterator end() const { return m_interpretation->end(); }

    /*!
     * \brief Returns the dimension of the space in which the interpretation is defined.
     * \return The space dimension as a \f$\text{dimension\_type}\f$.
     */
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_interpretation->space_dimension(); }

    /*!
     * \brief Returns the hash code of the observable.
     * \return The hash code as a \c std::size_t.
     */
    [[nodiscard]] std::size_t hashcode() const { return m_hashcode; }

    /*!
     * \brief Outputs the observable to a stream.
     * \param os The output stream.
     * \param observable The observable to output.
     * \return The output stream.
     */
    friend std::ostream& operator<< (std::ostream& os, const Observable& observable)
    {
        bool first = true;
        os << "Observable: {";
        for (const auto& atom: observable.atoms())
        {
            os << (first ? "" : ", ") << atom;
            first = false;
        }
        os << "}\n";
        os << "Interpretation: " <<
            (observable.m_interpretationToString.has_value()
                ? *observable.m_interpretationToString
                : PPLUtils::toString(*observable.interpretation()));
        return os;
    }

private:
    std::shared_ptr<spot::atomic_prop_set> m_atoms {}; ///< The set of atomic propositions
    PowersetConstSharedPtr m_interpretation {}; ///< The interpretation of the observable
    std::optional<std::string> m_interpretationToString {}; ///< An optional string representation of the interpretation
    std::size_t m_hashcode {}; ///< The hash code of the observable

    /*!
     * \brief Computes the hash code of the observable.
     */
    void computeHashCode()
    {
        hashCombine(m_hashcode, SpotUtils::hashcode(*m_atoms), *m_interpretation);
    }
};

/**
 * \brief Equality comparison for \c Observable
 * \param observable1 First \c Observable to compare
 * \param observable2 Second \c Observable to compare
 * \return true if both \c Observable s have the same \ref Observable::hashcode()
 */
inline bool operator== (const Observable& observable1, const Observable& observable2) {
    return observable1.hashcode() == observable2.hashcode();
}

/**
 * \brief Inequality comparison for \c Observable
 * \param observable1 First \c Observable to compare
 * \param observable2 Second \c Observable to compare
 * \return true if the \c Observable s are different
 */
inline bool operator!= (const Observable& observable1, const Observable& observable2) {
    return !(observable1 == observable2);
}

/*!
 * \brief Specialization of \c std::hash for \c Observable objects.
 */
template<> struct std::hash<Observable>
{
     /*!
     * \brief Computes the hash code of an \c Observable object.
     * \param observable The \c Observable object.
     * \return The hash code as a \c std::size_t.
     */
    std::size_t operator() (const Observable& observable) const noexcept
    {
        return observable.hashcode();
    }
};
