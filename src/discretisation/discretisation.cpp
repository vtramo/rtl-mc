#include "discretisation.h"
#include "formula_constants.h"

/**
 * \defgroup DiscretisationRules Formal Discretisation Rules
 * \brief Mathematical definition of RTL/RTLf discretisation rules
 *
 * The discretisation follows these formal rules:
 * \f{align*}{
 *   \texttt{dsc}(p)                &\overset{\triangle}{=} p \\
 *   \texttt{dsc}(\neg \varphi)     &\overset{\triangle}{=} \neg \texttt{dsc}(\varphi) \\
 *   \texttt{dsc}(\varphi_1 \,\land\, \varphi_2) &\overset{\triangle}{=} \texttt{dsc}(\varphi_1) \land \texttt{dsc}(\varphi_2) \\
 *   \texttt{dsc}(\varphi_1\, \lor\, \varphi_2)  &\overset{\triangle}{=} \texttt{dsc}(\varphi_1) \lor \texttt{dsc}(\varphi_2) \\
 *   \texttt{dsc}(\mathbf{X}\, \varphi)       &\overset{\triangle}{=} (sing \land \mathbf{X}\,\texttt{dsc}(\varphi)) \lor (\neg sing \land \texttt{dsc}(\varphi)) \\
 *   \texttt{dsc}(\varphi_1\, \mathbf{U}\, \varphi_2) &\overset{\triangle}{=} \texttt{dsc}(\varphi_1)\, \mathbf{U}\, ((sing \lor \texttt{dsc}(\varphi_1)) \land \texttt{dsc}(\varphi_2)) \\
 *   \texttt{dsc}(\varphi_1\, \mathbf{R}\, \varphi_2) &\overset{\triangle}{=} \texttt{dsc}(\varphi_1)\, \mathbf{R}\, ((\neg sing \land \texttt{dsc}(\varphi_1)) \lor \texttt{dsc}(\varphi_2)) \\
 *   \texttt{dsc}(\mathbf{G}\, \varphi_1) &\overset{\triangle}{=} \texttt{dsc}(\bot\, \mathbf{R}\, \varphi_1) \\
 *   \texttt{dsc}(\varphi_1\,\mathbf{M}\,\varphi_2) &\overset{\triangle}{=} \texttt{dsc}(\varphi_1 \, \mathbf{R} \, \varphi_2)\,\land\,\texttt{dsc}(\mathbf{F}(\varphi_1\,\land\,\varphi_2)) \\
 *   \texttt{dsc}(\varphi_1\,\mathbf{W}\,\varphi_2) &\overset{\triangle}{=} \texttt{dsc}(\varphi_1\, \mathbf{U} \, \varphi_2)\,\lor\,\texttt{dsc}(\mathbf{G}(\varphi_1))
 * \f}
 */


/**
 * \defgroup FiniteDiscretisation Finite Semantics Discretisation (RTLf → LTL)
 * \brief Direct discretisation of finite-time RTLf formulae to LTL infinite semantics
 *
 * These functions implement the combined RTLf → LTL transformation that:
 * - Applies standard discretisation rules
 * - Incorporates the \f$alive\f$ proposition to convert to LTL
 *
 * \note More efficient than separate RTLf → LTLf → LTL transformations
 * \see DiscretisationRules
 * \see https://spot.lre.epita.fr/tut12.html
 */

namespace
{
    spot::formula dsctF(spot::formula&& formula);
    spot::formula dsctG(spot::formula&& formula);
    spot::formula dsctX(spot::formula&& formula);
    spot::formula dsctU(spot::formula&& formula);
    spot::formula dsctR(spot::formula&& formula);
    spot::formula dsctM(spot::formula&& formula);
    spot::formula dsctW(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{F}\f$ (eventually) operator discretisation
     * \param formula Input \f$\mathbf{F}\f$ formula
     * \return Discretised \f$\mathbf{F}\f$ formula according to rule:
     * \f$\texttt{dsc}(\mathbf{F}\varphi) \overset{\triangle}{=} \mathbf{F}\,\texttt{dsc}(\varphi)\f$
     */
    spot::formula dscF(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{G}\f$ (globally) operator discretisation
     * \param formula Input \f$\mathbf{G}\f$ formula
     * \return Discretised \f$\mathbf{G}\f$ formula according to rule:
     * \f$\texttt{dsc}(\mathbf{G} \varphi_1) \overset{\triangle}{=} \texttt{dsc}(false \mathbf{R} \varphi_1)\f$
     */
    spot::formula dscG(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{X}\f$ (next) operator discretisation
     * \param formula Input \f$\mathbf{X}\f$ formula
     * \return Discretized \f$\mathbf{X}\f$ formula according to rule:
     * \f$\texttt{dsc}(\mathbf{X}\varphi) \overset{\triangle}{=} (sing\land\mathbf{X}\,\texttt{dsc}(\varphi))\lor(\neg sing\land\texttt{dsc}(\varphi))\f$
     */
    spot::formula dscX(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{U}\f$ (until) operator discretisation
     * \param formula Input \f$\mathbf{U}\f$ formula
     * \return Discretised \f$\mathbf{U}\f$ formula according to rule:
     * \f$\texttt{dsc}(\varphi_1\mathbf{U}\varphi_2) \overset{\triangle}{=} \texttt{dsc}(\varphi_1)\mathbf{U}((sing\lor\texttt{dsc}(\varphi_1))\land\texttt{dsc}(\varphi_2))\f$
     */
    spot::formula dscU(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{R}\f$ (release) operator discretisation
     * \param formula Input \f$\mathbf{R}\f$ formula
     * \return Discretised \f$\mathbf{R}\f$ formula according to rule:
     * \f$\texttt{dsc}(\varphi_1\mathbf{R}\varphi_2) \overset{\triangle}{=} \texttt{dsc}(\varphi_1)\mathbf{R}((\neg sing\land\texttt{dsc}(\varphi_1))\lor\texttt{dsc}(\varphi_2))\f$
     */
    spot::formula dscR(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{M}\f$ (release) operator discretisation
     * \param formula Input \f$\mathbf{M}\f$ formula
     * \return Discretised \f$\mathbf{M}\f$ formula according to rule:
     * \f$\texttt{dsc}(\varphi_1\mathbf{M}\varphi_2) \overset{\triangle}{=} \texttt{dsc}(\varphi_1 \, \mathbf{R} \, \varphi_2)\,\land\,\texttt{dsc}(\mathbf{F}(\varphi_1\,\land\,\varphi_2))\f$
     */
    spot::formula dscM(spot::formula&& formula);

    /**
     * \brief Implements the \f$\mathbf{W}\f$ (release) operator discretisation
     * \param formula Input \f$\mathbf{W}\f$ formula
     * \return Discretised \f$\mathbf{W}\f$ formula according to rule:
     * \f$\texttt{dsc}(\varphi_1\mathbf{W}\varphi_2) \overset{\triangle}{=} \texttt{dsc}(\varphi_1\, \mathbf{U} \, \varphi_2)\,\lor\,\texttt{dsc}(\mathbf{G}(\varphi_1))\f$
     */
    spot::formula dscW(spot::formula&& formula);

    /**
     * \see g_finiteAlternationSingOpenObservablesOneStep
     */
    spot::formula finiteAlternationSingOpenObservablesOneStep();

    /**
     * \see g_finiteAlternationSingOpenObservables
     */
    spot::formula finiteAlternationSingOpenObservables();

    /**
     * \see g_alternationSingOpenObservables
     */
    spot::formula alternationSingOpenObservables();
}

spot::formula applyFiniteAlternationSingOpenObservablesOneStep(spot::formula&& formula)
{
    return andFormulae({
                std::move(formula),
                finiteAlternationSingOpenObservablesOneStep(),
                alive(),
                aliveUntilGNotAlive(),
            });

}

/**
 * This function enforces finite trace semantics by conjoining the input formula with:
 * \f[
 * \underbrace{\mathbf{G}(sing \leftrightarrow \mathbf{X}\neg sing \lor last)}_{\text{alternation}}
 * \land \underbrace{\mathbf{F}(last\, \land \, sing)}_{\text{termination}}
 * \f]
 * where \f$last\f$ identifies the final state \f$ last \, \overset{\triangle}{=} \, \neg \mathbf{X}[!]true\f$
 *
 * The combined constraints ensure:
 * 1. Proper alternation between singular and open intervals during execution
 * 2. The trace must be finite and terminate in a singular observable
 *
 * \note Compared to the infinite version \ref applyAlternationSingOpenObservables, this adds:
 *       - The termination requirement \f$\mathbf{F}(last \, \land \, sing)\f$
 *       - Uses strong next (\f$\mathbf{X}[!]\f$) for precise finite-state detection
 *
 * \see applyFiniteAlternationSingOpenObservablesOneStep() for version with alive proposition
 * \see g_finiteAlternationSingOpenObservables for the base constraint formula
 */
spot::formula applyFiniteAlternationSingOpenObservables(spot::formula&& formula)
{
    return andFormulae({
                std::move(formula),
                finiteAlternationSingOpenObservables(),
             });
}

/**
 * This function enforces the alternation pattern between singular and open intervals by
 * conjoining the input formula with:
 * \f[
 * \texttt{G}(sing \leftrightarrow \mathbf{X}\,\neg sing)
 * \f]
 *
 * \note This function is for pure infinite semantics (without finite termination).
 *
 * \see g_alternationSingOpenObservables for the base constraint formula
 * \see applyFiniteAlternationSingOpenObservables() for the finite-semantics version
 */
spot::formula applyAlternationSingOpenObservables(spot::formula&& formula)
{
    return andFormulae({
                std::move(formula),
                alternationSingOpenObservables(),
             });
}

/**
 * \ingroup FiniteDiscretisation
 */
spot::formula toDiscretisedLtlFormula(spot::formula&& rtlFinite)
{
    switch (rtlFinite.kind())
    {
    case spot::op::ap:
        return rtlFinite == ap("last")
            ? g_lastFinite
            : rtlFinite;
    case spot::op::X:
        return dsctX(std::move(rtlFinite));
    case spot::op::F:
        return dsctF(std::move(rtlFinite));
    case spot::op::G:
        return dsctG(std::move(rtlFinite));
    case spot::op::U:
        return dsctU(std::move(rtlFinite));
    case spot::op::W:
        return dsctW(std::move(rtlFinite));
    case spot::op::M:
        return dsctM(std::move(rtlFinite));
    case spot::op::R:
        return dsctR(std::move(rtlFinite));
    default:
        return rtlFinite.map([] (spot::formula child)
        {
            return toDiscretisedLtlFormula(std::move(child));
        });
    }
}

/**
 * \ingroup DiscretisationRules
 *
 * Applies the formal discretisation rules recursively to all formula components.
 * Handles all core LTL operators and atomic propositions.
 */
spot::formula toDiscretisedFormula(spot::formula&& formula)
{
    switch (formula.kind())
    {
    case spot::op::ap:
        return formula == ap("last")
            ? g_last
            : formula;
    case spot::op::X:
        return dscX(std::move(formula));
    case spot::op::F:
        return dscF(std::move(formula));
    case spot::op::G:
        return dscG(std::move(formula));
    case spot::op::U:
        return dscU(std::move(formula));
    case spot::op::W:
        return dscW(std::move(formula));
    case spot::op::M:
        return dscM(std::move(formula));
    case spot::op::R:
        return dscR(std::move(formula));
    default:
        return formula.map([] (spot::formula child)
        {
            return toDiscretisedFormula(std::move(child));
        });
    }
}

namespace
{
    spot::formula dsctX(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretisedLtlFormula(formula[0]) };
        spot::formula next { X(andFormulae({ childDsct, alive() })) };
        return Or({
            andFormulae({ sing(), std::move(next) }),
            andFormulae({ notSing(), std::move(childDsct) })
        });
    }

    spot::formula dsctF(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretisedLtlFormula(formula[0]) };
        return F(andFormulae({ std::move(childDsct), alive() }));
    }

    spot::formula dsctG(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretisedLtlFormula(formula[0]) };
        return G(Or({ std::move(childDsct), notAlive() }));
    }

    spot::formula dsctU(spot::formula&& formula)
    {
        spot::formula child1Dsct { toDiscretisedLtlFormula(formula[0]) };
        spot::formula child2Dsct { toDiscretisedLtlFormula(formula[1]) };
        spot::formula child1DsctCopy { child1Dsct };

        return U(std::move(child1DsctCopy), andFormulae({
                                            std::move(child2Dsct),
                                            singOr(std::move(child1Dsct)),
                                            alive()
                                        }));
    }

    spot::formula dsctR(spot::formula&& formula)
    {
        spot::formula child1Dsct { toDiscretisedLtlFormula(formula[0]) };
        spot::formula child2Dsct { toDiscretisedLtlFormula(formula[1]) };
        spot::formula child1DsctCopy { child1Dsct };

        return R(std::move(child1DsctCopy), Or({ std::move(child2Dsct), andFormulae({ std::move(child1Dsct), notSing() }), notAlive() }));
    }

    spot::formula dsctM(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };
        spot::formula child1AndChild2 { andFormulae({ child1, child2 }) };

        return andFormulae({
            dsctR(R(std::move(child1), std::move(child2))),
            dsctF(F(std::move(child1AndChild2)))
        });
    }

    spot::formula dsctW(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula untilDsct { dsctU(U(child1, std::move(child2))) };
        spot::formula globallyDsct { dsctG(G(Or({ std::move(child1), notAlive() }))) };

        return Or({ std::move(untilDsct), std::move(globallyDsct) });
    }



    spot::formula dscX(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretisedFormula(formula[0]) };
        spot::formula strongNext { strongX(childDsc) };
        return Or({
            andFormulae({ sing(), strongNext }),
            andFormulae({ notSing(), std::move(childDsc) })
        });
    }

    spot::formula dscF(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretisedFormula(formula[0]) };
        return F(std::move(childDsc));
    }

    spot::formula dscG(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretisedFormula(formula[0]) };
        return G(std::move(childDsc));
    }

    spot::formula dscU(spot::formula&& formula)
    {
        spot::formula child1Dsc { toDiscretisedFormula(formula[0]) };
        spot::formula child2Dsc { toDiscretisedFormula(formula[1]) };

        spot::formula child2AndSingOrChild1 { andFormulae({ std::move(child2Dsc), singOr(child1Dsc) }) };
        return U(std::move(child1Dsc), std::move(child2AndSingOrChild1));
    }

    spot::formula dscW(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula until { dscU(U(child1, std::move(child2))) };
        spot::formula globally { dscG(G(std::move(child1))) };
        return Or({ std::move(until), std::move(globally) });
    }

    spot::formula dscR(spot::formula&& formula)
    {
        spot::formula child1Dsc { toDiscretisedFormula(formula[0]) };
        spot::formula child2Dsc { toDiscretisedFormula(formula[1]) };

        spot::formula child1AndNotSing { andFormulae({ child1Dsc, notSing() }) };
        return R(std::move(child1Dsc), Or({ std::move(child2Dsc), std::move(child1AndNotSing) }));
    }

    spot::formula dscM(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula eventually { dscF(F(andFormulae({ child1, child2 }))) };
        spot::formula release { dscR(R(std::move(child1), std::move(child2))) };
        return andFormulae({ std::move(eventually), std::move(release) });
    }

    spot::formula finiteAlternationSingOpenObservablesOneStep()
    {
        return g_finiteAlternationSingOpenObservablesOneStep;
    }

    spot::formula finiteAlternationSingOpenObservables()
    {
        return g_finiteAlternationSingOpenObservables;
    }

    spot::formula alternationSingOpenObservables()
    {
        return g_alternationSingOpenObservables;
    }
}