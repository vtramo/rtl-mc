/**
 * \file DiscreteFiniteLtlFormula.h
 * \brief Represents a finite LTL (LTLf) formula obtained by discretising an RTLf formula
 */

#pragma once

#include <spot/tl/formula.hh>
#include <spot/tl/ltlf.hh>
#include "DiscreteLtlFormula.h"


/**
 * \class DiscreteFiniteLtlFormula
 * \brief Represents a finite LTL (LTLf) formula obtained by discretising an RTLf formula
 *
 * This class encapsulates a discrete LTLf formula obtained by transforming an RTLf formula
 * through a two-step process:
 * 1. Initial discretization via \ref toDiscretisedFormula
 * 2. Application of finite semantics constraints via \ref applyFiniteAlternationSingOpenObservables
 *
 * If the RTLf formula is \f$\varphi\f$, then the formula contained in \ref DiscreteFiniteLtlFormula
 * (which we call \f$\widehat{\varphi}^{\text{fin}}\f$) will be:
 * \f[
 *    \widehat{\varphi}^{\text{fin}} \overset{\triangle}{=} \text{dsc}(\varphi)\, \land \,
 *    \texttt{G}\left((sing \leftrightarrow \texttt{X}\,\neg sing)\, \lor\, last \right)\,\land\,
 *    \texttt{F}(last \, \land\, sing)
 * \f]
 * where \f$last \, \overset{\triangle}{=} \, !\texttt{X}[!]\,true \f$
 */
class DiscreteFiniteLtlFormula {
public:
    /**
     * \brief Discretises an RTLf formula into an equivalent LTLf formula
     * \param formula The RTLf formula to discretise (rvalue reference)
     * \return DiscreteFiniteLtlFormula containing the discretised LTLf formula
     *
     * Applies the discretisation rules to transform the RTL formula into LTLf:
     * 1. First converts to basic discretised LTL form \ref toDiscretisedFormula
     * 2. Then applies finite semantics with alternation between singular and open observables \ref applyFiniteAlternationSingOpenObservables
     */
    static DiscreteFiniteLtlFormula discretiseRtlFinite(spot::formula&& formula) {
        spot::formula discreteLtlFormula { toDiscretisedFormula(std::move(formula)) };
        return DiscreteFiniteLtlFormula { applyFiniteAlternationSingOpenObservables(std::move(discreteLtlFormula)) };
    }

    /**
     * \brief Access the underlying LTLf formula
     * \return Const reference to the internal LTLf spot::formula
     */
    const spot::formula& formula() const { return m_discreteFiniteLtlFormula; }

    /**
     * \brief Converts the LTLf formula to infinite-semantics LTL
     * \return DiscreteLtlFormula with infinite semantics
     *
     * Uses SPOT's from_ltlf() to transform the finite LTL formula to infinite
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
     * \param formula The discretised LTLf formula (rvalue reference)
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