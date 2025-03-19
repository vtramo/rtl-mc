#pragma once

#include <spot/tl/apcollect.hh>
#include <spot/tl/formula.hh>
#include <spot/twaalgos/postproc.hh>

/*!
 *  \brief Returns the logical constant 'true' formula (tt).
 *  \return A formula representing logical constant 'true'.
 */
spot::formula top();

/*!
 *  \brief Returns the logical constant 'false' formula (ff).
 *  \return A formula representing logical constant 'false'.
 */
spot::formula bottom();

/*!
 *  \brief Converts a string into an atomic proposition formula.
 *  \param ap A string representing an atomic proposition.
 *  \return A formula representing the atomic proposition.
 */
spot::formula ap(std::string_view ap);

/*!
 *  \brief Creates an LTL "eventually" formula (F formula).
 *  \param formula The formula to be wrapped with the "eventually" operator.
 *  \return A formula wrapped with the "eventually" operator.
 */
spot::formula F(const spot::formula& formula);

/*!
 *  \brief Creates an LTL "eventually" formula (F formula).
 *  \param formula The formula to be wrapped with the "eventually" operator.
 *  \return A formula wrapped with the "eventually" operator.
 */
spot::formula F(spot::formula&& formula);

/*!
 *  \brief Creates an LTL "next" formula (X formula).
 *  \param formula The formula to be wrapped with the "next" operator.
 *  \return A formula wrapped with the "next" operator.
 */
spot::formula X(spot::formula&& formula);

/*!
 *  \brief Creates an LTL "next" formula (X formula).
 *  \param formula The formula to be wrapped with the "next" operator.
 *  \return A formula wrapped with the "next" operator.
 */
spot::formula X(const spot::formula& formula);

/*!
 *  \brief Creates an LTL strong "next" formula (X[!]formula).
 *  \param formula The formula to be wrapped with the strong "next" operator.
 *  \return A formula wrapped with the strong "next" operator.
 */
spot::formula strongX(const spot::formula& formula);

/*!
 *  \brief Creates an LTL strong "next" formula (X[!]formula).
 *  \param formula The formula to be wrapped with the strong "next" operator.
 *  \return A formula wrapped with the strong "next" operator.
 */
spot::formula strongX(spot::formula&& formula);

/*!
 *  \brief Creates an LTL "globally" formula (G formula).
 *  \param formula The formula to be wrapped with the "globally" operator.
 *  \return A formula wrapped with the "globally" operator.
 */
spot::formula G(spot::formula&& formula);

/*!
 *  \brief Creates an LTL "globally" formula (G formula).
 *  \param formula The formula to be wrapped with the "globally" operator.
 *  \return A formula wrapped with the "globally" operator.
 */
spot::formula G(const spot::formula& formula);

/*!
 *  \brief Creates an LTL "weak until" formula (formula1 W formula2).
 *  \param formula1 The first formula in the "weak until" operator.
 *  \param formula2 The second formula in the "weak until" operator.
 *  \return A formula representing the "weak until" operation.
 */
spot::formula W(spot::formula&& formula1, spot::formula&& formula2);

/*!
 *  \brief Creates an LTL "until" formula (formula1 U formula2).
 *  \param formula1 The first formula in the "until" operator.
 *  \param formula2 The second formula in the "until" operator.
 *  \return A formula representing the "until" operation.
 */
spot::formula U(spot::formula&& formula1, spot::formula&& formula2);

/*!
 *  \brief Creates an LTL "until" formula (formula1 U formula2).
 *  \param formula1 The first formula in the "until" operator.
 *  \param formula2 The second formula in the "until" operator.
 *  \return A formula representing the "until" operation.
 */
spot::formula U(const spot::formula& formula1, spot::formula&& formula2);

/*!
 *  \brief Creates an LTL "release" formula (formula1 R formula2).
 *  \param formula1 The first formula in the "release" operator.
 *  \param formula2 The second formula in the "release" operator.
 *  \return A formula representing the "release" operation.
 */
spot::formula R(spot::formula&& formula1, spot::formula&& formula2);

/*!
 *  \brief Creates an LTL equivalence formula (formula1 ≡ formula2).
 *  \param formula1 The first formula in the equivalence operator.
 *  \param formula2 The second formula in the equivalence operator.
 *  \return A formula representing the equivalence operation.
 */
spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2);

/*!
 *  \brief Creates a conjunction formula from a vector of sub-formulae.
 *  \param formulae A vector of sub-formulae to be combined in conjunction.
 *  \return A formula representing the conjunction of the given sub-formulae.
 */
spot::formula And(std::vector<spot::formula>&& formulae);

/*!
 *  \brief Creates a formula representing the conjunction of the given atomic propositions.
 *  \param atoms A set of atomic propositions to be combined in conjunction.
 *  \return A formula representing the conjunction of the given atomic propositions.
 */
spot::formula andAtoms(const spot::atomic_prop_set& atoms);

/*!
 *  \brief Negates the given formula.
 *  \param formula The formula to be negated (rvalue reference).
 *  \return A new formula representing the negation of the input formula.
 */
spot::formula Not(spot::formula&& formula);

/*!
 *  \brief Negates the given formula.
 *  \param formula The formula to be negated.
 *  \return A new formula representing the negation of the input formula.
 */
spot::formula Not(const spot::formula& formula);

/*!
 *  \brief Creates a disjunction (Or) formula from a vector of sub-formulae.
 *  \param formulae A vector of sub-formulae to be combined in disjunction.
 *  \return A formula representing the disjunction of the given sub-formulae.
 */
spot::formula Or(std::vector<spot::formula>&& formulae);

/*!
 *  \brief Returns the "sing" atomic proposition.
 *  \return The "sing" atomic proposition formula.
 */
spot::formula sing();

/*!
 *  \brief Returns the "sing" atomic proposition.
 *  \return The "sing" atomic proposition formula.
 */
spot::formula sing();

/*!
 *  \brief Combines the given formula with the "sing" atomic proposition using a logical OR.
 *  \param formula The formula to be combined with "sing".
 *  \return A formula representing the logical OR between the input formula and "sing".
 */
spot::formula singOr(spot::formula&& formula);

/*!
 *  \brief Combines the given formula with the "sing" atomic proposition using a logical OR.
 *  \param formula The formula to be combined with "sing".
 *  \return A formula representing the logical OR between the input formula and "sing".
 */
spot::formula singOr(const spot::formula& formula);

/*!
 *  \brief Returns the negation of the "sing" atomic proposition.
 *  \return The negation of the "sing" atomic proposition formula.
 */
spot::formula notSing();

/*!
 *  \brief Returns the "alive" atomic proposition.
 *  \return The "alive" atomic proposition formula.
 */
spot::formula alive();

/*!
 *  \brief Returns the negation of the "alive" atomic proposition.
 *  \return The negation of the "alive" atomic proposition formula.
 */
spot::formula notAlive();

/*!
 *  \brief Returns the "alive until globally not alive" formula.
 *  \return The formula representing "alive until globally not alive".
 */
spot::formula aliveUntilGNotAlive();

/*!
 *  \brief Creates an atomic proposition set from a set of strings.
 *  \param atoms A set of strings representing the atomic propositions.
 *  \return An atomic proposition set created from the given strings.
 */
spot::atomic_prop_set AP(std::set<std::string>&& atoms);

/*!
 *  \brief Generates an LTL formula that enforces k alternations between p and q.
 *  \param k The number of required alternations (must be greater than 0).
 *  \param p The first formula in the alternation (defaults to the atomic proposition "p").
 *  \param q The second formula in the alternation (defaults to the atomic proposition "q").
 *  \return A formula enforcing k alternations between p and q.
 *  \throws std::invalid_argument if k <= 0.
 */
spot::formula generateAlternatingFormula(int k, spot::formula p = ap("p"), spot::formula q = ap("q"));

/*!
 *  \brief Checks whether a formula is free of the Next (X) operator.
 *  \param formula The formula to be checked.
 *  \return True if the formula does not contain the Next (X) operator, false otherwise.
 */
bool isXFree(spot::formula& formula);

/*!
 *  \brief Converts a formula to a string representation.
 *  \param formula The formula to be converted.
 *  \return A string representation of the given formula.
 */
std::string toFormulaString(const spot::formula& formula);

/*!
 *  \brief Checks if the given formula is the "sing" atomic proposition.
 *  \param formula The formula to be checked.
 *  \return True if the formula is the "sing" atomic proposition, false otherwise.
 */
bool isSing(const spot::formula& formula);

/*!
 *  \brief Checks if the given formula is the negation of the "sing" atomic proposition.
 *  \param formula The formula to be checked.
 *  \return True if the formula is the negation of the "sing" atomic proposition, false otherwise.
 */
bool isNotSing(const spot::formula& formula);

/*!
 *  \brief Checks if the given formula is either the "sing" atomic proposition or its negation.
 *  \param formula The formula to be checked.
 *  \return True if the formula is "sing" or "not sing", false otherwise.
 *  \throws std::invalid_argument if the formula is not a valid ALTL formula.
 */
bool isSingOrNotSing(const spot::formula& formula);

/*!
 *  \brief Removes the "sing" atomic proposition from the given formula if present.
 *  \param formula The formula from which to remove the "sing" atomic proposition.
 *  \return A tuple containing the formula with "sing" removed and a boolean indicating if the "sing" proposition was removed.
 */
std::tuple<spot::formula, bool> removeSing(spot::formula&& formula);

/*!
 *  \brief Checks if the "sing" atomic proposition is present in the given set of atomic propositions.
 *  \param atomicPropositions The set of atomic propositions to be checked.
 *  \return True if "sing" is present in the set of labels, false otherwise.
 */
bool containsSing(const spot::atomic_prop_set& atomicPropositions);

/*!
 *  \brief Collects all atomic propositions from the formula that are not in the provided forbidden set.
 *  \param forbiddenAtoms A set of atomic propositions to exclude.
 *  \param formula The formula from which to collect the atomic propositions.
 *  \return A vector of formulas that represent atomic propositions not in the forbidden set.
 */
std::vector<spot::formula> collectAtomsNotIn(const spot::atomic_prop_set& forbiddenAtoms, spot::formula& formula);

/*!
 *  \brief Collects all positive literals (non-negated atomic propositions) from the given formula.
 *  \param formula The formula to be processed.
 *  \return A set of atomic propositions representing the positive literals in the formula.
 */
spot::atomic_prop_set collectPositiveLiterals(spot::formula&& formula);

/*!
 *  \brief Checks if the given formula is non-recurrent.
 *  \param formula The formula to be checked.
 *  \return True if the formula is non-recurrent, false otherwise.
 */
bool isNonRecurrent(spot::formula& formula);
