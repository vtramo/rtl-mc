#include "formula.h"
#include "formula_constants.h"

#include <spot/tl/apcollect.hh>
#include <spot/tl/randomltl.hh>
#include <spot/twa/twagraph.hh>
#include <spot/tl/nenoform.hh>

/*!
 *  Returns the logical constant 'true' formula (tt).
 */
spot::formula top()
{
    return spot::formula::tt();
}

/*!
 *  Returns the logical constant 'false' formula (ff).
 */
spot::formula bottom()
{
    return spot::formula::ff();
}

/*!
 *  Converts a string into an atomic proposition formula.
 */
spot::formula ap(const std::string_view ap)
{
    return spot::formula::ap(std::string { ap });
}

/*!
 *  Creates an LTL "eventually" formula (F formula).
 */
spot::formula F(const spot::formula& formula)
{
    return spot::formula::F(formula);
}

/*!
 *  Creates an LTL "eventually" formula (F formula).
 */
spot::formula F(spot::formula&& formula)
{
    return spot::formula::F(std::move(formula));
}

/*!
 *  Creates an LTL "next" formula (X formula).
 */
spot::formula X(spot::formula&& formula)
{
    return spot::formula::X(std::move(formula));
}

/*!
 *  Creates an LTL "next" formula (X formula).
 */
spot::formula X(const spot::formula& formula)
{
    return spot::formula::X(formula);
}

/*!
 *  Creates an LTL strong "next" formula (X[!]formula).
 */
spot::formula strongX(const spot::formula& formula)
{
    return spot::formula::unop(spot::op::strong_X, formula);
}

/*!
 *  Creates an LTL strong "next" formula (X[!]formula).
 */
spot::formula strongX(spot::formula&& formula)
{
    return spot::formula::unop(spot::op::strong_X, std::move(formula));
}

/*!
 *  Creates an LTL "globally" formula (G formula).
 */
spot::formula G(spot::formula&& formula)
{
    return spot::formula::G(std::move(formula));
}

/*!
 *  Creates an LTL "globally" formula (G formula).
 */
spot::formula G(const spot::formula& formula)
{
    return spot::formula::G(formula);
}

/*!
 *  Creates an LTL "weak until" formula (formula1 W formula2).
 */
spot::formula W(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::W(std::move(formula1), std::move(formula2));
}

/*!
 *  Creates an LTL "until" formula (formula1 U formula2).
 */
spot::formula U(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::U(std::move(formula1), std::move(formula2));
}

/*!
 *  Creates an LTL "until" formula (formula1 U formula2).
 */
spot::formula U(const spot::formula& formula1, spot::formula&& formula2)
{
    return spot::formula::U(formula1, std::move(formula2));
}

/*!
 *  Creates an LTL "release" formula (formula1 R formula2).
 */
spot::formula R(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::R(std::move(formula1), std::move(formula2));
}

/*!
 *  Creates an LTL equivalence formula (formula1 ≡ formula2).
 */
spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2)
{
    return spot::formula::Equiv(formula1, formula2);
}

/*!
 *  Creates a conjunction formula from a vector of sub-formulae.
 */
spot::formula And(std::vector<spot::formula>&& formulae)
{
    return spot::formula::And(std::move(formulae));
}

/*!
 * Creates a conjunction formula from a set of formulae.
 */
spot::formula andFormulae(const std::set<spot::formula>& formulae)
{
    spot::formula result {};
    for (spot::formula atom: formulae)
        result = And(std::vector { std::move(atom), std::move(result) });
    return result;
}

spot::formula Not(const spot::formula& formula)
{
    return spot::formula::Not(formula);
}

spot::formula Not(spot::formula&& formula)
{
    return spot::formula::Not(std::move(formula));
}

/*!
 *  Creates a disjunction (Or) formula from a vector of sub-formulae.
 */
spot::formula Or(std::vector<spot::formula>&& formulae)
{
    return spot::formula::Or(std::move(formulae));
}

/*!
 *  Returns the "sing" atomic proposition.
 */
spot::formula sing()
{
    return g_sing;
}

/*!
 *  Returns the negation of the "alive" atomic proposition.
 */
spot::formula notSing()
{
    return Not(g_sing);
}

/*!
 * This function creates a new formula that represents the logical disjunction (OR) between
 * the provided formula and the "sing" atomic proposition.
 */
spot::formula singOr(spot::formula&& formula)
{
    return Or({ g_sing, std::move(formula) });
}

/*!
 * This function creates a new formula that represents the logical disjunction (OR) between
 * the provided formula and the "sing" atomic proposition.
 */
spot::formula singOr(const spot::formula& formula)
{
    return Or({ g_sing, formula });
}


/*!
 *  Returns the "alive" atomic proposition.
 */
spot::formula alive()
{
    return g_alive;
}

/*!
 *  Returns the negation of the "alive" atomic proposition.
 */
spot::formula notAlive()
{
    return Not(g_alive);
}

/*!
 *  Returns the "alive until globally not alive" formula.
 */
spot::formula aliveUntilGNotAlive()
{
    return g_aliveUntilGNotAlive;
}

/*!
 * This function constructs an atomic proposition set from a set of string
 * values representing individual atomic propositions.
 */
spot::atomic_prop_set AP(std::set<std::string>&& atoms)
{
    spot::atomic_prop_set atomicPropVector {};
    for (const auto& atom : atoms)
        atomicPropVector.insert(ap(atom));
    return atomicPropVector;
}

/*!
 * Generates an LTL formula that enforces k alternations between p and q using "eventually" LTL operator.
 *
 * The generated formula follows the pattern:
 *
 * \f$
 * \textbf{F}\,\big(p \, \land \, \textbf{F}(q \, \land \, \textbf{F}(p \, \land \, \textbf{F}(q \, \land \, \dots )))\big)
 * \f$
 *
 * This ensures that in order to satisfy the formula, one must alternate
 * between \( p \) and \( q \) exactly \( k \) times.
 */
spot::formula generateAlternatingFormula(int k, spot::formula p, spot::formula q)
{
    if (k < 0)
    {
        throw std::invalid_argument("k must be greater than 0!");
    }

    spot::formula result { F(andFormulae({ p, F(q) })) };
    if (k == 1)
        return result;

    bool pTurn = false;
    while (--k > 0)
    {
        result = pTurn ? F(andFormulae({ p, result })) : F(andFormulae({ q  , result }));
        pTurn = !pTurn;
    }

    return result;
}

/*!
 * Checks whether a formula is free of the Next (X) operator.
 *
 * The function traverses the given formula and returns `true` if it does not contain
 * any occurrence of the temporal operator \( X \) (Next). Otherwise, it returns `false`.
 */
bool isXFree(spot::formula& formula)
{
    bool hasX { false };

    formula.traverse([&hasX] (const spot::formula& child)
                    {
                        if (child.is(spot::op::X))
                        {
                            hasX = true;
                        }

                        return hasX;
                    });

    return !hasX;
}

/*!
* Traverses the formula and collects all atomic propositions that are not part of the forbidden set.
*/
std::vector<spot::formula> collectAtomsNotIn(const spot::atomic_prop_set& forbiddenAtoms, spot::formula& formula)
{
    std::vector<spot::formula> result {};
    result.reserve(forbiddenAtoms.size());

    formula.traverse([&] (const spot::formula& child)
    {
        if (child.is(spot::op::ap))
        {
            if (!forbiddenAtoms.count(child))
            {
                result.push_back(child);
            }
        }

        return false;
    });

    return result;
}

/*!
* Converts a `spot::formula` object into its string representation.
*/
std::string toFormulaString(const spot::formula& formula)
{
    std::ostringstream oss {};
    oss << formula;
    return { oss.str() };
}

/*!
 *
 * This function processes the formula recursively, checking for the presence of the "sing" atomic proposition
 * or its negation. If found, it replaces them with appropriate constants (\f$\top\f$ or \f$\bot\f$) depending on
 * the context (logical AND or OR). The function also tracks whether any "sing" proposition was removed.
 *
 * The function handles literals, constants (`tt` and `ff`), and composite formulas (logical AND or OR).
 * It returns a tuple containing the modified formula and a boolean indicating if any "sing" proposition was removed.
 */
std::tuple<spot::formula, bool> removeSing(spot::formula&& formula)
{
    if (isSingOrNotSing(formula))
        return std::make_tuple(
            spot::formula::ff(),
            true
        );

    if (formula.is_literal() || formula.is_tt() || formula.is_ff())
        return std::make_tuple(
            formula,
            false
        );

    bool removedAtLeastOneSing {};
    auto removeAllSingAtoms = [&](spot::formula&& child, auto&& self) -> spot::formula
    {
        const bool isAnd { child.is(spot::op::And) };
        std::vector<spot::formula> children {};
        children.resize(child.size());

        for (auto&& childChild: child)
        {
            if (isSing(childChild) || isNotSing(childChild))
            {
                if (isSing(childChild)) removedAtLeastOneSing = true;
                children.push_back(isAnd ? top() : bottom());
            } else if (childChild.is_literal())
            {
                children.push_back(childChild);
            } else
            {
                children.push_back(self(std::move(childChild), self));
            }
        }

        return isAnd ? spot::formula::And(std::move(children)) : spot::formula::Or(std::move(children));
    };

    return std::make_tuple(
        removeAllSingAtoms(std::move(formula), removeAllSingAtoms),
        removedAtLeastOneSing
    );
}

/*!
 *
 * This function combines the results of `isSing` and `isNotSing` to determine if the formula
 * represents "sing" or its negation.
 */
bool isSingOrNotSing(const spot::formula& formula)
{
    return isSing(formula) || (formula.is(spot::op::Not) && isSing(formula[0]));
}

/*!
 *
 * This function verifies if the formula is a logical NOT operation and if its child is the "sing" atomic proposition.
 */
bool isNotSing(const spot::formula& formula)
{
    return formula.is(spot::op::Not) && isSing(formula[0]);
}

/*!
 * This function compares the formula with the global "sing" atomic proposition (`g_sing`).
 */
bool isSing(const spot::formula& formula)
{
    return g_sing == formula;
}

/*!
 * This function searches for the "sing" atomic proposition in the provided set of atomic propositions.
 */
bool containsSing(const spot::atomic_prop_set& atomicPropositions)
{
    return atomicPropositions.find(sing()) != atomicPropositions.end();
}

/*!
 * This function traverses the formula recursively and identifies atomic propositions that are not negated.
 * Negated atomic propositions (e.g., `!p`) are skipped, while positive literals (e.g., `p`) are added to the result set.
 */
spot::atomic_prop_set collectPositiveLiterals(spot::formula&& formula)
{
    spot::atomic_prop_set result {};

    formula.traverse([&] (const spot::formula& child)
    {
        if (child.is(spot::op::Not) && child[0].is(spot::op::ap))
        {
            return true;
        }

        if (child.is(spot::op::ap))
        {
            result.insert(child);
        }

        return false;
    });

    return result;
}

/*!
 * This function verifies whether the formula belongs to the non-recurrent fragment of ALTL.
 * A formula is considered non-recurrent if, in every release subformula of the form
 * \f$\varphi_1 \, \textbf{R} \, \varphi_2\f$,
 * the second argument \f$ \varphi_2 \f$ is a propositional formula (i.e., a boolean expression).
 *
 * Intuitively, this fragment disallows temporal properties to be required continuously, except for plain boolean ones.
 * For example:
 * - The formula \f$\bot \, \textbf{R} \, (p \land q)\f$, which is equivalent to \f$\textbf{G} (p \land q)\f$, belongs to the non-recurrent fragment.
 * - The formula \f$\bot \, \textbf{R} \, (\textbf{F} \, p)\f$, which is equivalent to \f$\textbf{G} \, \textbf{F} \, p\f$, does **not** belong to the non-recurrent fragment,
 *   because the truth of the temporal formula \f$\textbf{F} \, p\f$ is required continuously.
 *
 * The function first ensures that the formula is in negative normal form (NNF) and then traverses it to check for the presence
 * of release operators (\f$\textbf{R}\f$) or globally operators (\f$\textbf{G}\f$) that violate the non-recurrent condition.
 */
bool isNonRecurrent(spot::formula& formula)
{
    if (!formula.is_ltl_formula())
        throw std::invalid_argument("Spot formula is not a LTL formula");

    if (!formula.is_in_nenoform())
        formula = spot::negative_normal_form(formula);

    bool isNonRecurrent { true };

    formula.traverse([&isNonRecurrent] (const spot::formula& child)
                    {
                        if (!isNonRecurrent) return true;

                        if (child.is(spot::op::G))
                        {
                            isNonRecurrent = child[0].is_boolean();
                        }
                        else if (child.is(spot::op::R))
                        {
                            isNonRecurrent = child[1].is_boolean();
                        }

                        return !isNonRecurrent;
                    });

    return isNonRecurrent;
}