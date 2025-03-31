/**
 * \file discretisation.h
 * \brief RTL/\f$\text{RTL}_f\f$ to LTL/\f$\text{LTL}_f\f$ discretisation rules implementation
 */

#pragma once
#include <spot/tl/formula.hh>

/**
 * \brief Main entry point for \f$\text{RTL}_f\f$ → LTL discretisation
 * \param rtlFinite Input \f$\text{RTL}_f\f$ formula with finite semantics
 * \return Equivalent LTL formula with infinite semantics
 */
spot::formula toDiscretisedLtlFormula(spot::formula&& rtlFinite);


spot::formula applyFiniteAlternationSingOpenObservablesOneStep(spot::formula&& formula);

/**
 * \brief Main discretisation function converting RTL/\f$\text{RTL}_f\f$ to LTL/\f$\text{LTL}_f\f$
 * \param formula Input RTL/\f$\text{RTL}_f\f$ formula to discretise
 * \return Equivalent discrete LTL/\f$\text{LTL}_f\f$ formula
 */
spot::formula toDiscretisedFormula(spot::formula&& formula);

/**
 * \brief Applies finite semantics constraints with alternation and right-closed termination
 * \param formula The input formula to be constrained
 * \return The conjunction of the input formula with finite alternation constraints
 */
spot::formula applyFiniteAlternationSingOpenObservables(spot::formula&& formula);

/**
 * \brief Applies the alternation constraint between singular and open observables to a formula
 * \param formula The input formula to be constrained
 * \return The conjunction of the input formula with the alternation constraint
 */
spot::formula applyAlternationSingOpenObservables(spot::formula&& formula);