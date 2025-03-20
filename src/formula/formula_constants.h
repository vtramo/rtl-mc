#pragma once

#include <spot/tl/parse.hh>
#include "formula.h"

/*!
 *  \brief A constant representing the \f$sing\f$ atomic proposition.
 *
 * The \f$sing\f$ atomic proposition is used to distinguish between open and singular intervals
 * in the context of time slicing. It holds true in all and only the time points \f$t_i\f$ of the
 * time slicing \f$\tau\f$.
 *
 * \note This proposition is essential for maintaining the distinction between open intervals
 * (where \f$sing\f$ is false) and singular intervals (where \f$sing\f$ is true).
 */
inline const spot::formula g_sing { ap("sing") };


/*!
 *  \brief A constant representing the \f$alive\f$ atomic proposition.
 *
 * The \f$alive\f$ atomic proposition is used to embed the semantics of LTLf (LTL with finite semantics)
 * into standard LTL (which operates over infinite words). This is achieved by introducing \f$alive\f$
 * as a proposition that is initially true but eventually becomes false forever, effectively
 * marking the end of the finite trace.
 *
 * This trick allows Spot, which is designed for infinite-word semantics, to handle LTLf formulae
 * by rewriting them. For example, the LTLf formula \f$(a \, \texttt{U} \, b) \land \, \texttt{F} \, c\f$ is transformed
 * into the LTL formula:
 * \f[
 * alive \land (a \, \texttt{U} \, (alive \land b)) \, \land \, \texttt{F}(alive \land c) \land (alive \, \texttt{U} \, (\texttt{G} \, \lnot alive))
 * \f]
 *
 * For more details, see the Spot tutorial:
 * \see https://spot.lre.epita.fr/tut12.html
 */
inline const spot::formula g_alive { ap("alive") };


/*!
 *  \brief A constant representing the LTL formula \f$alive \, \texttt{U} \, (\texttt{G} \, \lnot alive)\f$.
 *
 * This formula is a key component in the reduction of LTLf (LTL with finite semantics) satisfiability
 * to LTL (on infinite traces) satisfiability.
 *
 * It guarantees that \f$alive\f$ stays true until it fails and then stay failed.
 */
inline const spot::formula g_aliveUntilGNotAlive { U(g_alive, G(Not(g_alive))) };

/*!
 *  \brief A constant representing the atomic proposition \f$last\f$ in the context of LTLf semantics.
 *
 * The \f$last\f$ proposition is defined as \f$last \, \equiv \, \lnot \texttt{X} \, alive\f$, where:
 * - \f$alive\f$ is the atomic proposition that marks the alive part of the trace.
 *
 * In the context of LTLf (LTL with finite semantics), \f$last\f$ holds true at the final state of the
 * finite trace. This proposition is used to identify the end of the finite trace when translating LTLf
 * formulae into LTL formulae over infinite traces.
 */
inline const spot::formula g_lastFinite { Not(X(alive())) };

/*!
 *  \brief A constant representing the LTL formula that enforces alternation between singular and open observables
 *         and ensures finite-time semantics with a right-closed signal.
 *
 * This formula combines the following requirements:
 * - Alternation between singular and open observables: \f$\texttt{G}(\lnot alive \, \lor \, ((sing \, \leftrightarrow \, \texttt{X}(alive \, \land \, \lnot sing))\, \lor\, \lnot \texttt{X} \, alive))\f$.
 * - Finite-time semantics: The trace must eventually end, marked by \f$\texttt{F}(sing \, \land \, alive \, \land \lnot \texttt{X} \, alive)\f$.
 *
 * Specifically:
 * - The first part of the formula ensures that the \f$sing\f$ proposition alternates between singular and open observables
 *   as long as the trace is active (i.e., \f$alive\f$ is true).
 * - The second part ensures that the trace is finite and ends with a right-closed signal (i.e., a singular observable).
 */
inline const spot::formula g_finiteAlternationSingOpenObservablesOneStep {
    spot::parse_infix_psl(
        "G( !alive | ((sing <-> X(alive & !sing)) | !Xalive) ) &"
        "F(sing & alive & !Xalive)"
    ).f
};


/*!
 *  \brief A constant representing the atomic proposition \f$last\f$ using the strong next operator.
 *
 * The \f$last\f$ proposition is defined as \f$last \equiv \lnot \texttt{X}[!] \, true\f$, where:
 * - \\texttt{X}[!]\f$ is the **strong next** operator, which requires that the next state exists and that the argument holds in that state.
 * - \f$true\f$ is a tautology that always holds.
 *
 * In the context of finite-time semantics (LTLf), \f$last\f$ holds true in the final state of the trace, where there is no next state.
 * The strong next operator ensures that \f$\texttt{X}[!] \, true\f$ is false in the last state, as there is no successor state.
 * Therefore, \f$\lnot \texttt{X}[!] \, true\f$ correctly identifies the final state of a finite trace.
 *
 * \note The strong next operator (\f$\texttt{X}[!]\f$) differs from the weak next operator (\f$\texttt{X}\f$):
 * - \f$\texttt{X}[!] \, a\f$ requires that a successor state exists and that \f$a\f$ holds in that state.
 * - \f$\texttt{X} \, a\f$ allows for the absence of a successor state (i.e., it is true if there is no next state).
 *
 * \see https://spot.lre.epita.fr/tut12.html
 */
inline const spot::formula g_last { spot::parse_infix_psl("!X[!] true").f };


/*!
 *  \brief A constant representing the LTL formula that enforces alternation between singular and open observables
 *         using the strong next operator and ensures finite-time semantics with a right-closed signal.
 *
 * This formula is similar to `g_finiteAlternationSingOpenObservablesOneStep`, but it uses the **strong next** operator
 * (\f$X[!]\f$) instead of the weak next operator (\f$X\f$). It combines the following requirements:
 * - Alternation between singular and open observables: \f$\texttt{G}((sing \, \leftrightarrow \, \texttt{X}[!] \, \lnot sing) \, \lor \, \lnot \texttt{X}[!] \, true)\f$.
 * - Finite-time semantics: The trace must eventually end, marked by \f$\texttt{F}(\lnot \texttt{X}[!] \, true \, \land \, sing)\f$.
 *
 * Specifically:
 * - The first part of the formula ensures that the \f$sing\f$ proposition alternates between singular and open observables
 *   as long as the trace is active (i.e., there is a next state). The strong next operator ensures that the alternation
 *   is strictly enforced when a next state exists.
 * - The second part ensures that the trace is finite and ends with a right-closed signal (i.e., a singular observable).
 *   The condition \f$\lnot X[!] \, true\f$ identifies the final state of the trace, where no next state exists.
 *
 *   Unlike `g_finiteAlternationSingOpenObservablesOneStep`, this formula does **not** use the atomic proposition \f$alive\f$.
 *   The proposition \f$\text{alive}\f$ **must** be introduced to embed the semantics of LTLf (LTL with finite semantics)
 *   into standard LTL (which operates over infinite words). The \f$\text{alive}\f$ proposition marks the active part
 *   of the trace, ensuring that the finite semantics of LTLf are correctly captured within the infinite semantics of LTL.
 */
inline const spot::formula g_finiteAlternationSingOpenObservables {
    spot::parse_infix_psl(
        "G((sing <-> X[!]!sing) | !X[!] true) &"
        "F(!X[!] true & sing)"
    ).f
};


/*!
 *  \brief A constant representing the LTL formula that enforces alternation between singular and open observables
 *         in the context of infinite-time semantics.
 *
 * This formula enforces the alternation between singular and open observables over an infinite trace.
 * It is defined as:
 * \f[
 * \texttt{G}(sing \leftrightarrow \texttt{X} \, \lnot sing)
 * \f]
 * where \f$sing\f$ is the atomic proposition representing singular observables.
 *
 * The formula ensures that the \f$sing\f$ proposition alternates between true and false at each step of the trace.
 * Specifically:
 * - If \f$sing\f$ is true in the current state, it must be false in the next state.
 * - If \f$sing\f$ is false in the current state, it must be true in the next state.
 *
 * This formula is used in the context of **infinite-time semantics**, where the trace is assumed to be infinite.
 * Unlike finite-time semantics, there is no notion of a "last" state, and the alternation continues indefinitely.
 *
 * \note The condition \f$\lnot \texttt{X} \, true \f$ (used in finite-time semantics) is not applicable here, as it would
 * be equivalent to \f$false\f$ in infinite-time semantics.
 */
inline const spot::formula g_alternationSingOpenObservables {
    spot::parse_infix_psl(
        "G((sing <-> X!sing))"
    ).f
};

// inline const formula g_alternationSingOpenObservables {
//     spot::parse_infix_psl(
//         "G((sing <-> X!sing) | !X[!] true)"
//     ).f
// };