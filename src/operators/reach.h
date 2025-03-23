#pragma once

#include "ppl_aliases.h"

/*!
 * \brief Type alias representing a pair of polyhedra \f$\{(P_i, X_i)\}\f$.
 *
 * The functions:
 * - \ref ReachPairs reach0(const Powerset& A, const Poly& B, const Poly& preFlow)
 * - \ref ReachPairs reachPlus(const Powerset& A, const Poly& B, const Poly& preFlow)
 *
 * return a collection of pairs \f$\{(P_i, X_i)\}_{i=0}^n\f$, where:\n
 *   (i) \f$ P_i \f$ is a convex polyhedron contained in \f$ A \f$ (a patch of \f$ A \f$),\n
 *  (ii) \f$ X_i \f$ is a convex polyhedron such that \f$ X_i \subseteq P_i \f$,\n
 * (iii) The union of all \f$ X_i \f$ represents the set of points in \f$ A \f$ that can reach \f$ B \f$.\n
 *
 * This type alias is used to represent the result of the split operation in the reachability functions.
 */
using ReachPair = std::pair<Poly, Poly>;

/*!
 * \brief A vector of \ref ReachPair.
 */
using ReachPairs = std::vector<ReachPair>;

/*!
 *  \brief Computes the set of points in \f$ A \f$ from which there exists an admissible trajectory that immediately reaches \f$ B \f$.
 *
 * The function computes the set of points in \f$ A \f$ that can reach \f$ B \f$ without any delay, following a trajectory
 * that never leaves the union of \f$ A \f$ and \f$ B \f$. This corresponds to the operator \f$\mathit{reach}^0(A, B)\f$.
 *
 * \param A The source convex polyhedron.
 * \param B The target general polyhedron.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return A `PowersetUniquePtr` representing the set of points in \f$ A \f$ that can reach \f$ B \f$ immediately.
 */
PowersetUniquePtr reach0(const Poly& A, const Powerset& B, const Poly& preFlow);

/*!
 *  \overload
 *  \see reach0(const Poly& A, const Powerset& B, const Poly& preFlow)
 */
PowersetUniquePtr reach0(const Powerset& A, const Powerset& B, const Poly& preFlow);

/*!
 *  \brief Computes the set of points in \f$ A \f$ from which there exists an admissible trajectory that immediately reaches \f$ B \f$, split into convex polyhedra.
 *
 * The function computes the set of points in \f$ A \f$ that can reach \f$ B \f$ without any delay, following an admissible trajectory
 * that never leaves the union of \f$ A \f$ and \f$ B \f$. This corresponds to the operator \f$\mathit{reach}^0(A, B)\f$.
 * The result is split into convex polyhedra, each contained in one of the patches of \f$ A \f$.
 *
 * \param A The source convex polyhedron.
 * \param B The target general polyhedron.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return The set of points in \f$ A \f$ that can reach \f$ B \f$ immediately, split as a \ref ReachPairs.
 */
ReachPairs reach0(const Powerset& A, const Poly& B, const Poly& preFlow);

/*!
 *  \brief Computes the set of points in \f$ A \f$ that can reach \f$ B \f$, following an admissible trajectory that never leaves their union.
 *
 * This function computes the set of points in \f$ A \f$ that can reach \f$ B \f$ after a strictly positive delay, following an admissible trajectory
 * that never leaves \f$ A \f$ before reaching \f$ B \f$.
 *
 * \param A The source convex polyhedron.
 * \param B The target general polyhedron.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return A `PowersetUniquePtr` representing the set of points in \f$ A \f$ that can reach \f$ B \f$.
 */
PowersetUniquePtr reachPlus(const Powerset& A, const Powerset& B, const Poly& preFlow);

/*!
 *  \brief Computes the set of points in \f$ A \f$ that can reach \f$ B \f$, following an admissible trajectory that never leaves their union.
 *
 * This function computes the set of points in \f$ A \f$ that can reach \f$ B \f$ after a strictly positive delay, following an admissible trajectory
 * that never leaves \f$ A \f$ before reaching \f$ B \f$. The result is split into convex polyhedra, each contained in one of the patches of \f$ A \f$.
 *
 * \param A The source convex polyhedron.
 * \param B The target general polyhedron.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return The set of points in \f$ A \f$ that can reach \f$ B \f$, split as a \ref ReachPairs.
 */
ReachPairs reachPlus(const Powerset& A, const Poly& B, const Poly& preFlow);

/*!
 *  \brief Partitions the complement of \f$ V \f$ into two sets \f$ A \f$ and \f$ B \f$, based on reachability while avoiding \f$ V \f$.
 *
 * This function computes a partition of the complement of \f$ V \f$ (denoted as \f$ \overline{V} \f$) into two sets \f$ A \f$ and \f$ B \f$, where:
 * - \f$ A \f$ is the set of points in \f$ \overline{V} \f$ that can reach \f$ U \f$ while avoiding \f$ V \f$.
 * - \f$ B \f$ contains the other points of \f$ \overline{V} \f$.
 *
 * The partition satisfies \f$ A \cup B = \overline{V} \f$.
 *
 * \param U The target powerset.
 * \param V The powerset to avoid.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return A pair of `Powerset` objects \f$ (A, B) \f$, where:
 *   - \f$ A \f$ contains points in \f$ \overline{V} \f$ that can reach \f$ U \f$ while avoiding \f$ V \f$,
 *   - \f$ B \f$ contains the remaining points in \f$ \overline{V} \f$.
 *
 * \note This version admits a finite number of sharp corners in each trajectory. It uses adjacency maps.
 */
std::pair<Powerset, Powerset> rwaMaps(Powerset U, Powerset V, Poly preFlow);