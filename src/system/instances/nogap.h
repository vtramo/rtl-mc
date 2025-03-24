#pragma once

#include "PolyhedralSystem.h"
#include "formula.h"
#include "systemparser.h"

/*!
 * \brief Constructs a polyhedral system and an RTLf formula for the \f$\varphi^{\text{nogap}}_k\f$ family.
 *
 * This function constructs a polyhedral system and an RTLf formula that enforces a trajectory to alternate
 * \f$k\f$ times between two adjacent half-spaces. The system is based on the two-tank model,
 * where the invariant and flow constraints are defined as follows:
 *
 * - **Invariant**: \f$\{a \geq 0 \land b \geq 0\}\f$
 * - **Flow**: \f$\{a + b \geq -2 \land a + b \leq 2 \land a \geq -1 \land a \leq 1 \land b \geq -2 \land b \leq 2 \land t = 1\}\f$
 *
 * The atomic propositions \f$p\f$ and \f$q\f$ are interpreted as:
 * - \f$[p] = \{a > b\}\f$
 * - \f$[q] = \{b > a\}\f$
 *
 * The RTLf formula is of the form:
 * \f[
 * \varphi^{\text{nogap}}_k \triangleq G(\mathit{Inv}) \land t_0 \land G(t_{\mathit{max}}) \land \texttt{F}\,\big(p \, \land \, \texttt{F}(q \, \land \, \texttt{F}(p \, \land \, \texttt{F}(q \, \land \, \dots )))\big)
 * \f]
 * where \f$t_0\f$ represents the initial time (\f$t = 0\f$) and \f$t_{\mathit{max}}\f$ represents the maximum time (\f$t \leq t_{\mathit{max}}\f$).
 *
 * \param k The number of alternations between \f$p\f$ and \f$q\f$.
 * \param t The maximum time \f$t_{\mathit{max}}\f$.
 * \return A tuple containing the constructed \ref PolyhedralSystem and the RTLf formula.
 * \throws std::invalid_argument If \f$k\f$ or \f$t\f$ is negative.
 */
inline std::tuple<PolyhedralSystemSharedPtr, spot::formula> nogap(const int k, const int t)
{
    if (k < 0)
    {
        throw std::invalid_argument("k must be non-negative!");
    }

    if (t < 0)
    {
        throw std::invalid_argument("t must be non-negative!");
    }

    PolyhedralSystemSharedPtr polyhedralSystem{
        std::make_shared<PolyhedralSystem>(
            std::move(
                *parsePolyhedralSystem(
                    "Inv ( { a >= 0 & b >= 0 } )"
                    "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                    "p { a > b }"
                    "q { b > a }"
                    "t0 { t = 0 }"
                    "t1 { t <= " + std::to_string(t) + " }"
                )
            )
        )
    };

    spot::formula rtlf{andFormulae({ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q"))})};

    return {std::move(polyhedralSystem), std::move(rtlf)};
}