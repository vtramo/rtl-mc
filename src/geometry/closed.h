#pragma once

#include "ppl_utils.h"

/*!
 *  \brief Checks if a \c Powerset is closed.
 *
 * This function checks if a polyhedron \f$A\f$, represented as a \c Powerset, is closed.
 * A \c Powerset is considered closed if the following condition is satisfied:
 * \f[
 *     (\top\,\setminus\, A)\,\cap\,\mathit{cl}(A)\,==\,\emptyset
 * \f]
 * where:
 * - \f$\top\f$ represents the universe,
 * - \f$A\f$ is the given \c Powerset,
 * - \f$\mathit{cl}(A)\f$ is the topological closure of \f$A\f$.
 *
 * In other words, the function checks whether the intersection between the set of points
 * outside \f$A\f$ and the topological closure of \f$A\f$ is empty. If this intersection is empty,
 * the \c Powerset is closed.
 *
 * \note The `is_topologically_closed()` function from the Parma Polyhedral Library (PPL) checks
 *       whether every disjoint inside the \c Powerset is closed. In contrast, this function checks
 *       if the union of all disjoints within the \c Powerset actually forms a closed polyhedron.
 *
 * \param powerset The input \c Powerset representing a polyhedron, which may not be convex.
 * \return `true` if the \c Powerset is closed, `false` otherwise.
 */
inline bool isClosed(const Powerset& powerset)
{
    Powerset universe {  powerset.space_dimension() };
    PowersetUniquePtr universeMinusPowerset { PPLUtils::minus(universe, powerset) };

    Powerset powersetClosure { powerset };
    powersetClosure.topological_closure_assign();

    return PPLUtils::intersect(*universeMinusPowerset, powersetClosure)->is_empty();
}