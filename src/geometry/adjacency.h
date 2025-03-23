#pragma once

#include "ppl_utils.h"

/*!
 *  \brief Computes the border between two convex polyhedra \f$ P \f$ and \f$ Q \f$.
 * \param p The first polyhedron \f$ P \f$.
 * \param q The second polyhedron \f$ Q \f$.
 * \return A `PowersetUniquePtr` representing the border between \f$ P \f$ and \f$ Q \f$.
 * \see border(const Powerset& p, const Powerset& q)
 */
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

/*!
 *  \brief Checks if two convex polyhedra \f$ P \f$ and \f$ Q \f$ are adjacent.
 *
 * Two convex polyhedra \f$ P \f$ and \f$ Q \f$ are said to be adjacent if their \ref border is not empty.
 *
 * \param p The first convex polyhedron \f$ P \f$.
 * \param q The second convex polyhedron \f$ Q \f$.
 * \return `true` if \f$ P \f$ and \f$ Q \f$ are adjacent, `false` otherwise.
 */
inline bool areAdjacent(const Poly& p, const Poly& q)
{
    PowersetUniquePtr borderPQ { border(p, q) };
    return !borderPQ->is_empty();
}

/*!
 *  \brief Computes the border between two general polyhedra \f$ P \f$ and \f$ Q \f$.
 *
 * The border between two general polyhedra \f$ P \f$ and \f$ Q \f$ is defined as the set of points that are adjacent to both polyhedra
 * and belong to at least one of them. Formally, the border is given by:
 * \f[
 * \mathit{border}(P, Q) = (P \cap \mathit{cl}(Q)) \cup (\mathit{cl}(P) \cap Q)
 * \f]
 * where \f$\mathit{cl}(P)\f$ and \f$\mathit{cl}(Q)\f$ represent the topological closures of \f$P\f$ and \f$ Q \f$, respectively.
 *
 * \param p The first general polyhedron \f$ P \f$.
 * \param q The second general polyhedron \f$ Q \f$.
 * \return A `PowersetUniquePtr` representing the border between \f$ P \f$ and \f$ Q \f$.
 */
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

/*!
 *  \brief Checks if two general polyhedra \f$ P \f$ and \f$ Q \f$ are adjacent.
 *
 * Two general polyhedra \f$ P \f$ and \f$ Q \f$ are said to be adjacent if their \ref border is not empty.
 *
 * \param p The first powerset \f$ P \f$.
 * \param q The second powerset \f$ Q \f$.
 * \return `true` if \f$ P \f$ and \f$ Q \f$ are adjacent, `false` otherwise.
 */
inline bool areAdjacent(const Powerset& p, const Powerset& q)
{
    PowersetUniquePtr borderPQ { border(p, q) };
    return !borderPQ->is_empty();
}