#ifndef PAIRWISE_REDUCE_H
#define PAIRWISE_REDUCE_H

#include "ppl_aliases.h"

namespace PPLUtils {
    void rawPairwiseReduce(Powerset &powerset);
    void fastPairwiseReduce(Powerset &powerset);
    void metaPairwiseReduce(Powerset &powerset, bool check_correctness=false);
}

#endif //PAIRWISE_REDUCE_H
