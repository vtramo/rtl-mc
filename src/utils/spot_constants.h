#ifndef SPOT_CONSTANTS_H
#define SPOT_CONSTANTS_H

#include "spot_utils.h"

namespace spot::constants {
    inline const formula g_nullFormula {};

    inline const formula g_alive { ap("alive") };
    inline const formula g_sing { ap("sing") };
    inline const formula g_last { // alive & X!G!alive
        And({
            g_alive,
            X(Not(G(Not(g_alive))))
        })
    };

    // sing & !(1 U (alive & !((alive & X!G!alive) | (sing <-> X!sing)))) & !(1 U (alive & !(1 U (alive & sing & X!G!alive))))
    inline const formula singOpenLastProperty {
        And({
            g_sing,
            Not(U(top(), And({ Not(Or({ g_last, Equiv(g_sing, X(Not(g_sing))) })), g_alive }))),
            Not(U(top(), And({ Not(U(top(), And({ g_last, g_sing, g_alive }))), g_alive })))
        })
    };
}

#endif //SPOT_CONSTANTS_H
