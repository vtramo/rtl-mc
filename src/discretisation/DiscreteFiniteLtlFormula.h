/**
 * \file DiscreteFiniteLtlFormula.h
 * \brief Represents a finite \f$\text{LTL}_f\f$ formula obtained by discretising an \f$\text{RTL}_f\f$ formula
 */

#pragma once

#include <spot/tl/formula.hh>
#include <spot/tl/ltlf.hh>
#include "DiscreteLtlFormula.h"


/**
 * \class DiscreteFiniteLtlFormula
 * \brief Represents a finite \f$\text{LTL}_f\f$ formula obtained by discretising an \f$\text{RTL}_f\f$ formula
 *
 * This class encapsulates a discrete \f$\text{LTL}_f\f$ formula obtained by transforming an \f$\text{RTL}_f\f$ formula
 * through a two-step process:
 * 1. Initial discretization via \ref toDiscretisedFormula
 * 2. Application of finite semantics constraints via \ref applyFiniteAlternationSingOpenObservables
 *
 * If the \f$\text{RTL}_f\f$ formula is \f$\varphi\f$, then the formula contained in \ref DiscreteFiniteLtlFormula
 * (which we call \f$\widehat{\varphi}^{\text{fin}}\f$) will be:
 * \f[
 *    \widehat{\varphi}^{\text{fin}} \overset{\triangle}{=} \text{dsc}(\varphi)\, \land \,
 *    \texttt{G}\left((sing \leftrightarrow \texttt{X}\,\neg sing)\, \lor\, last \right)\,\land\,
 *    \texttt{F}(last \, \land\, sing)
 * \f]
 * where \f$last \, \overset{\triangle}{=} \, !\texttt{X}[!]\,true \f$
 *
 * \see g_sing
 */
class DiscreteFiniteLtlFormula {
public:
    /**
     * \brief Discretises an \f$\text{RTL}_f\f$ formula into an equivalent \f$\text{LTL}_f\f$ formula
     * \param formula The \f$\text{RTL}_f\f$ formula to discretise (rvalue reference)
     * \return DiscreteFiniteLtlFormula containing the discretised \f$\text{LTL}_f\f$ formula
     *
     * Applies the discretisation rules to transform the \f$\text{RTL}_f\f$ formula into \f$\text{LTL}_f\f$:
     * 1. First converts to basic discretised LTL form \ref toDiscretisedFormula
     * 2. Then applies finite semantics with alternation between singular and open observables \ref applyFiniteAlternationSingOpenObservables
     */
    static DiscreteFiniteLtlFormula discretiseRtlFinite(spot::formula&& formula) {
        spot::formula discreteLtlFormula { toDiscretisedFormula(std::move(formula)) };
        return DiscreteFiniteLtlFormula { applyFiniteAlternationSingOpenObservables(std::move(discreteLtlFormula)) };
    }

    /**
     * \brief Access the underlying \f$\text{LTL}_f\f$ formula
     * \return Const reference to the internal \f$\text{LTL}_f\f$ spot::formula
     */
    const spot::formula& formula() const { return m_discreteFiniteLtlFormula; }

    /**
     * \brief Converts the \f$\text{LTL}_f\f$ formula to infinite-semantics LTL
     * \return DiscreteLtlFormula with infinite semantics
     *
     * Uses SPOT's from_ltlf() to transform the finite \f$\text{LTL}_f\f$ formula to infinite
     * semantics by adding the 'alive' atomic proposition.
     *
     * @note This class can only be constructed through its static factory method \ref discretiseRtlFinite.
     *
     * \see https://spot.lre.epita.fr/tut12.html
     */
    DiscreteLtlFormula toLtl() const { return DiscreteLtlFormula { spot::from_ltlf(m_discreteFiniteLtlFormula) }; }

private:
    spot::formula m_discreteFiniteLtlFormula {};

    DiscreteFiniteLtlFormula() = default;

    /**
     * \brief Private constructor that takes ownership of a spot formula
     * \param formula The discretised \f$\text{LTL}_f\f$ formula (rvalue reference)
     *
     * \pre The input formula must be a valid LTL formula (asserted)
     */
    explicit DiscreteFiniteLtlFormula(spot::formula&& formula)
        : m_discreteFiniteLtlFormula { std::move(formula) }
    {
        assert(m_discreteFiniteLtlFormula.is_ltl_formula() && "Must be a valid LTL formula!");
    }
};

/**
 * \brief Equality comparison between \ref DiscreteFiniteLtlFormula and spot::formula
 * \param discreteFormula The discretised formula wrapper
 * \param formula The raw SPOT formula to compare against
 * \return \c true if the underlying formulae are equivalent
 */
inline bool operator== (const DiscreteFiniteLtlFormula& discreteFormula, const spot::formula& formula)
{
    return discreteFormula.formula() == formula;
}

/**
 * \brief Stream output operator for \ref DiscreteFiniteLtlFormula
 * \param os Output stream
 * \param formula The formula to output
 * \return Reference to the output stream
 *
 * Outputs the string representation of the underlying spot formula
 */
inline std::ostream& operator<< (std::ostream& os, const DiscreteFiniteLtlFormula& formula)
{
    return os << formula.formula();;
}