#pragma once

#include <spot/tl/parse.hh>
#include "formula.h"

inline const spot::formula g_nullFormula {};

inline const spot::formula g_sing { ap("sing") };
inline const spot::formula g_alive { ap("alive") };
inline const spot::formula g_aliveUntilGNotAlive { U(g_alive, G(Not(g_alive))) };

inline const spot::formula g_lastFinite { Not(X(alive())) };

// Finite-time semantics with alive property.
inline const spot::formula g_finiteAlternationSingOpenObservablesOneStep {
    spot::parse_infix_psl(
        "G( !alive | ((sing <-> X(alive & !sing)) | !Xalive) ) &"
        "F(sing & alive & !Xalive)"
    ).f
};

inline const spot::formula g_last { spot::parse_infix_psl("!X[!] true").f }; // !X[!] true (finite-time semantics)

// Finite-time semantics.
inline const spot::formula g_finiteAlternationSingOpenObservables {
    spot::parse_infix_psl(
        "G((sing <-> X[!]!sing) | !X[!] true) &"
        "F(!X[!] true & sing)"
    ).f
};


// inline const formula g_alternationSingOpenObservables {
//     spot::parse_infix_psl(
//         "G((sing <-> X!sing) | !X[!] true)"
//     ).f
// };

// Infinite-time semantics. N.B. We cannot use !Xtrue because it is equivalent to false.
inline const spot::formula g_alternationSingOpenObservables {
    spot::parse_infix_psl(
        "G((sing <-> X!sing))"
    ).f
};