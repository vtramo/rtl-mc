#pragma once

#include "ppl_aliases.h"

using ReachPair = std::pair<Poly, Poly>;
using ReachPairs = std::vector<ReachPair>;

ReachPairs reach0(const Powerset& A, const Poly& B, const Poly& preFlow);
ReachPairs reachPlus(const Powerset& A, const Poly& B, const Poly& preFlow);

/* Returns a partition of "not V" into a pair of polyhedra (A, B):
   - A is the set of points that may reach U while avoiding V
   - B contains the other points of not V
   So, A \/ B = not V

   This version admits a finite number of sharp corners in each trajectory.
   It uses adjacency maps.
*/
std::pair<Powerset, Powerset> rwaMaps(Powerset U, Powerset V, Poly preFlow);