#pragma once

#include "ppl_utils.h"

inline PowersetUniquePtr border(const Poly& p, const Poly& q)
{
    Poly closureP { p };
    closureP.topological_closure_assign();

    Poly closureQ { q };
    closureQ.topological_closure_assign();

    PolyUniquePtr pIntersectClosureQ { PPLUtils::intersect(p, closureQ) };
    PolyUniquePtr qIntersectClosureP { PPLUtils::intersect(q, std::move(closureP)) };
    return PPLUtils::fusion(*pIntersectClosureQ, *qIntersectClosureP);
}

inline bool areAdjacent(const Poly& p, const Poly& q)
{
    PowersetUniquePtr borderPQ { border(p, q) };
    return !borderPQ->is_empty();
}

inline PowersetUniquePtr border(const Powerset& p, const Powerset& q)
{
    Powerset closureP { p };
    closureP.topological_closure_assign();

    Powerset closureQ { q };
    closureQ.topological_closure_assign();

    PowersetConstUniquePtr pIntersectClosureQ { PPLUtils::intersect(p, closureQ) };
    PowersetConstUniquePtr qIntersectClosureP { PPLUtils::intersect(q, std::move(closureP)) };
    return PPLUtils::fusion(*pIntersectClosureQ, *qIntersectClosureP);
}

inline bool areAdjacent(const Powerset& p, const Powerset& q)
{
    PowersetUniquePtr borderPQ { border(p, q) };
    return !borderPQ->is_empty();
}