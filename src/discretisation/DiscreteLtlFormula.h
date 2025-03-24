/**
* \file DiscreteLtlFormula.h
 * \brief Represents an LTL formula obtained by discretising RTL/RTLf formulae
 */

#pragma once

#include <spot/tl/formula.hh>
#include "discretisation.h"

/**
 * \class DiscreteLtlFormula
 * \brief Represents an LTL formula obtained through discretisation of RTL/RTLf formulae
 *
 * This class encapsulates a discrete LTL formula obtained through one of two transformation paths:
 * 1. discretisation of infinite-semantics RTL formulae to LTL (RTL → LTL)
 * 2. Optimized conversion of finite-semantics RTLf formulae to LTL (RTLf → LTLf → LTL in one step)
 *
 * @note Construction constraints:
 * - The only valid construction methods are the static factory methods:
 *   - \ref discretiseRtl (for direct RTL → LTL conversion)
 *   - \ref discretiseRtlFinite (for optimized RTLf → LTL conversion)
 * - The default constructor creates an empty formula
 *
 *
 */
class DiscreteLtlFormula
{
public:
    /**
     * \brief Discretises an infinite-semantics RTL formula to LTL
     * \param formula The RTL formula to discretise (rvalue reference)
     * \return DiscreteLtlFormula containing the discretised LTL formula
     *
     * Transformation process:
     * 1. Applies basic discretisation via \ref toDiscretisedFormula
     * 2. Adds infinite-semantics constraints via \ref applyAlternationSingOpenObservables
     *
     * Suitable for properties over infinite traces.
     */
    static DiscreteLtlFormula discretiseRtl(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyAlternationSingOpenObservables(std::move(discretisedLtlFormula)) };
    }

    /**
     * \brief Discretises a finite-semantics RTLf directly to LTL (RTLf -> LTLf -> LTL)
     * \param formula The RTLf formula to discretise (rvalue reference)
     * \return DiscreteLtlFormula containing the converted LTL formula
     *
     * Transformation process:
     * 1. Converts RTLf to LTL via \ref toDiscretisedLtlFormula
     * 2. Applies finite-semantics constraints via \ref applyFiniteAlternationSingOpenObservablesOneStep
     *
     * \note Optimization advantage:
     *       This method provides a more efficient path than separate transformations:
     *       - Combined path (optimized): RTLf → LTL (single step via \ref discretiseRtlFinite)
     *       - Old path (inefficient): RTLf → LTLf (via \ref DiscreteFiniteLtlFormula) → LTL (via \ref DiscreteFiniteLtlFormula::toLtl())
     *       The optimized path avoids intermediate object creation and performs transformations in a single pass.
     *
     * Suitable for properties over finite traces that need infinite-semantics treatment.
     */
    static DiscreteLtlFormula discretiseRtlFinite(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedLtlFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyFiniteAlternationSingOpenObservablesOneStep(std::move(discretisedLtlFormula)) };
    }

    /**
     * \brief Access the underlying LTL formula
     * \return Const reference to the internal LTL spot::formula
     */
    const spot::formula& formula() const { return m_discreteLtlFormula; }

    DiscreteLtlFormula() = default;
private:
    spot::formula m_discreteLtlFormula {};

    friend class DiscreteFiniteLtlFormula;

    /**
     * \brief Private constructor that takes ownership of a discretised LTL formula
     * \param formula The discretised LTL formula (rvalue reference)
     *
     * \pre The input formula must be a valid LTL formula (asserted)
     */
    explicit DiscreteLtlFormula(spot::formula&& formula)
        : m_discreteLtlFormula { std::move(formula) }
    {
        assert(m_discreteLtlFormula.is_ltl_formula() && "Must be a valid LTL formula!");
    }
};

/**
 * \brief Equality comparison between \ref DiscreteLtlFormula and spot::formula
 * \param discreteLtlFormula The discretised formula wrapper
 * \param formula The raw SPOT formula to compare against
 * \return \c true if the underlying formulae are equivalent
 */
inline bool operator== (const DiscreteLtlFormula& discreteLtlFormula, const spot::formula& formula)
{
    return discreteLtlFormula.formula() == formula;
}

/**
 * \brief Stream output operator for \ref DiscreteLtlFormula
 * \param os Output stream
 * \param formula The formula to output
 * \return Reference to the output stream
 *
 * Outputs the string representation of the underlying spot formula
 */
inline std::ostream& operator<< (std::ostream& os, const DiscreteLtlFormula& formula)
{
    return os << formula.formula();
}
