/**
 * \file TripleTileNode.h
 * \brief Representation of a triple \c Tile node in a \c OmnidirectionalPolyhedralAbstraction
 */

#pragma once

#include "formula.h"
#include "Tile.h"

/**
 * \class TripleTileNode
 * \brief Represents a triple \c Tile node in a \c OmnidirectionalPolyhedralAbstraction.
 *
 * This node consists of three \c Tile \f$(P, Q, R)\f$.
 * The node includes:
 * - An \c Observable representing the set of atomic propositions true in \f$Q\f$, along with the atomic proposition \f$sing\f$.
 * - A \c PowersetConstSharedPtr representing the set of points in \f$Q\f$ that can immediately reach a point in \f$P\f$ and
 *   a point in \f$R\f$. Formally, \ref TripleTileNode::points() represents the set of points \f$x \in Q\f$ such that
 *   there exist two points \f$y \in P\f$ and \f$z \in R\f$, two admissible trajectories \f$f_1\f$ and \f$f_2\f$,
 *   and two time instants \f$t_1\f$ and \f$t_2\f$, where \f$f_1 (0) = f_2 (0) = x\f$, \f$f_1 (t_1) = y\f$,
 *   \f$f_1 (t) \in P\f$ for all \f$ t \in (0, t_1)\f$, and \f$ f_2 (t) \in R\f$ for all \f$t \in (0, t_2)\f$.
 *   Since \f$\mathbf{0} \in \mathit{int}(\mathit{Flow})\f$, any straight segment is an admissible trajectory, and the
 *   above property simplifies to requiring that point \f$x\f$ be adjacent to both \f$P\f$ and \f$R\f$. Therefore,
 *   the set \f$\mathit{pts}((P,Q,R))\f$ corresponds precisely to \f$Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\f$.
 *
 * In summary:
 * \f[
 * \begin{aligned}
 * \mathit{obs}((P,Q,R)) &= \mathit{obs}(Q) \cup \{\mathit{sing}\} \\
 * \mathit{pts}((P,Q,R)) &= Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)
 * \end{aligned}
 * \f]
 *
 * where \f$\mathit{cl}(P)\f$ and \f$\mathit{cl}(R)\f$ denote the topological closures of \f$P\f$ and \f$R\f$, respectively.
 *
 * \see Observable
 * \see Tile
 * \see OmnidirectionalPolyhedralAbstraction
 */
class TripleTileNode
{
public:
    /**
     * \brief Constructs a triple-tile node from three tiles
     * \param tile1 \f$(P)\f$
     * \param tile2 \f$(Q)\f$
     * \param tile3 \f$(R)\f$
     * \throws Abort if space dimensions don't match (enabled only in DEBUG mode)
     *
     * \post Computes \p observable as \f$\mathit{obs}(Q) \cup \{sing\}\f$
     * \post Computes \p points as \f$Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\f$
     */
    TripleTileNode(const Tile& tile1, const Tile& tile2, const Tile& tile3)
        : m_triple { std::make_tuple(tile1, tile2, tile3) }
    {
        assert(tile1.spaceDimension() == tile2.spaceDimension());
        assert(tile2.spaceDimension() == tile3.spaceDimension());

        computeObservable();
        computePoints();
    }

    /**
     * \brief Gets the \c Observable for this node \f$(\mathit{obs}(Q) \cup \{sing\})\f$
     * \return Const reference to the \c Observable
     *
     * The \c Observable consists of all atomic propositions that hold in tile \f$Q\f$,
     * plus the special \f$sing\f$ atomic proposition that marks singular observables.
     */
    [[nodiscard]] const Observable& observable() const { return m_observable; }

    /**
     * \brief Gets the set of points for this node \f$(Q \cap \mathit{cl}(P) \cap \mathit{cl}(R))\f$
     * \return Shared pointer to const \c Powerset \f$(Q \cap \mathit{cl}(P) \cap \mathit{cl}(R))\f$
     *
     * The points represent the intersection:
     * \f[
     *      Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)
     * \f]
     * which contains all points in \f$Q\f$ that can immediately reach a point in \f$P\f$ and a point in \f$R\f$
     */
    [[nodiscard]] PowersetConstSharedPtr points() const { return m_points; }

    /**
     * \brief Checks if the set of points associated to this node covers the entire state space
     * \return `true` if the set of points associated to this node is the universe
     */
    [[nodiscard]] bool isUniverse() const { return m_points->is_universe(); }

    /**
     * \brief Checks if the set of points associated to this node is empty
     * \return `true` if the set of points associated to this node is empty \f$(Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\,==\,\emptyset)\f$
     */
    [[nodiscard]] bool isEmpty() const { return m_points->is_empty(); }

    /**
     * \brief Gets the space dimension of the state space in which this node is located
     * \return The dimension of the \c Observable
     */
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_observable.spaceDimension(); }

    friend bool operator!= (const TripleTileNode& tripleTileNode1, const TripleTileNode& tripleTileNode2)
    {
        return tripleTileNode1.m_triple != tripleTileNode2.m_triple;
    }

    friend bool operator== (const TripleTileNode& tripleTileNode1, const TripleTileNode& tripleTileNode2)
    {
        return tripleTileNode1.m_triple == tripleTileNode2.m_triple;
    }

private:
    std::tuple<Tile, Tile, Tile> m_triple;
    Observable m_observable {};
    PowersetConstSharedPtr m_points {};

    /**
     * \brief Computes the node's observable
     *
     * Sets observable to be \c Tile Q's \c Observable plus the \f$sing\f$ proposition:
     * observable \f$= \mathit{obs}(Q) \cup \{\mathit{sing}\}\f$
     */
    void computeObservable()
    {
        const Tile& tile2 { std::get<1>(m_triple) };
        const Observable& tile2Observable { tile2.observable() };
        spot::atomic_prop_set tile2AtomsWithSing { tile2Observable.atoms() };
        tile2AtomsWithSing.insert(sing());
        m_observable = Observable { tile2AtomsWithSing, *tile2.patches() };
    }

    /**
     * \brief Computes the node's polyhedral region
     *
     * Sets points to the intersection:
     * \f$Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\f$
     * where \f$\mathit{cl}(P)\f$ denotes \f$P\f$'s topological closure
     */
    void computePoints()
    {
        const Tile& tile1 { std::get<0>(m_triple) };
        const Tile& tile2 { std::get<1>(m_triple) };
        const Tile& tile3 { std::get<2>(m_triple) };
        m_points = PPLUtils::intersect({ tile1.closure(), tile2.patches(), tile3.closure() });
    }
};