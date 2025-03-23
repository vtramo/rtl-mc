/*!
 * \file pairwise_reduce.h
 * \brief Functions for efficient pairwise reduction of a set of convex polyhedra.
 *
 * Given a set of convex polyhedra \f$ A \f$, returns an equivalent set of convex polyhedra \f$ B \f$
 * such that, for all convex polyhedra \f$ P_1 \f$ and \f$ P_2 \f$ in \f$ B \f$, the union of \f$ P_1 \f$ and \f$ P_2 \f$ is not
 * a convex polyhedron.
 *
 * \note The input `Powerset` is modified in place.
 */

#pragma once

#include "ppl_aliases.h"

void rawPairwiseReduce(Powerset &powerset);
void fastPairwiseReduce(Powerset &powerset);
void metaPairwiseReduce(Powerset &powerset, bool check_correctness=false);