#ifndef SPOT_CONSTANTS_H
#define SPOT_CONSTANTS_H

#include <spot/tl/parse.hh>
#include "spot_utils.h"

namespace spot::constants {

    using namespace SpotUtils;

    inline const formula g_nullFormula {};

    inline const formula g_sing { ap("sing") };
    inline const formula g_alive { ap("alive") };
    inline const formula g_aliveUntilGNotAlive { U(g_alive, G(Not(g_alive))) };


    inline const formula g_lastFinite { Not(X(alive())) }; // !Xalive

    inline const formula g_singOpenLastPropertyFinite {
        spot::parse_infix_psl(
            "G( !alive | ((sing <-> X(alive & !sing)) | !Xalive) ) &"
            "F(sing & alive & !Xalive)"
        ).f
    };


    inline const formula g_last { spot::parse_infix_psl("!X[!] true").f }; // !X[!] true

    inline const formula g_singOpenLastProperty {
        spot::parse_infix_psl(
            "G((sing <-> X[!]!sing) | !X[!] true) &"
            "F(!X[!] true & sing)"
        ).f
    };
}

#endif //SPOT_CONSTANTS_H
