#ifndef SPOT_CONSTANTS_H
#define SPOT_CONSTANTS_H

#include "spot_utils.h"

namespace spot::constants {

    using namespace SpotUtils;

    inline const formula g_nullFormula {};

    inline const formula g_alive { ap("alive") };
    inline const formula g_aliveUntilGNotAlive { U(g_alive, G(Not(g_alive))) };
    inline const formula g_sing { ap("sing") };
    inline const formula g_last { // alive & XG!alive
        And({
            g_alive,
            X(G(Not(g_alive)))
        })
    };

    // sing &
    // G(((sing <-> X(!sing & alive)) | last) | !alive) &
    // F(last & sing & alive)
    inline const formula g_singOpenLastProperty {
        And({
            g_sing,
            G(Or({ Equiv(g_sing, X(And({ notSing(), g_alive }))), g_last, notAlive() })),
            F(And({ g_last, g_sing, g_alive }))
        })
    };
}

#endif //SPOT_CONSTANTS_H
