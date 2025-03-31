/**
 * \file OmnidirectionalPolyhedralAbstraction.h
 * \brief Concrete class representing a \c PolyhedralAbstraction of a \c PolyhedralSystem (omnidirectional case)
 */

#pragma once

#include <variant>
#include "TileNode.h"
#include "TripleTileNode.h"
#include "PolyhedralSystem.h"
#include "PolyhedralAbstraction.h"

class OmnidirectionalPolyhedralAbstraction;

/*!
 * \brief Type alias for a shared pointer to a const \c OmnidirectionalPolyhedralAbstraction.
 *
 * This type manages shared ownership of a const-qualified \c OmnidirectionalPolyhedralAbstraction object.
 * The object will be automatically deleted when the last shared pointer referencing it is destroyed.
 * Use this for read-only access to the abstraction when shared ownership is required.
 */
using OmnidirectionalPolyhedralAbstractionConstSharedPtr = std::shared_ptr<const OmnidirectionalPolyhedralAbstraction>;

/*!
 * \brief Type alias for a shared pointer to a mutable \c OmnidirectionalPolyhedralAbstraction.
 *
 * This type manages shared ownership of a mutable \c OmnidirectionalPolyhedralAbstraction object.
 * The object will be automatically deleted when the last shared pointer referencing it is destroyed.
 * Use this when multiple components need to modify the same abstraction instance.
 */
using OmnidirectionalPolyhedralAbstractionSharedPtr = std::shared_ptr<OmnidirectionalPolyhedralAbstraction>;

/*!
 * \brief Type alias for a unique pointer to an \c OmnidirectionalPolyhedralAbstraction.
 *
 * This type manages exclusive ownership of an \c OmnidirectionalPolyhedralAbstraction object.
 * The object will be automatically deleted when the unique pointer goes out of scope.
 * Use this when single ownership and automatic cleanup are required.
 */
using OmnidirectionalPolyhedralAbstractionUniquePtr = std::unique_ptr<OmnidirectionalPolyhedralAbstraction>;


/**
 * \class OmnidirectionalPolyhedralAbstraction
 * \brief Represents a concrete \c PolyhedralAbstraction of a \c PolyhedralSystem with an omnidirectional \f$\mathit{Flow}\f$
 *
 *
 * Represents an abstraction of a \c PolyhedralSystem in which the \f$\mathit{Flow}\f$ includes the origin
 * \f$(\mathbf{0} \in \mathit{Flow})\f$. In this case, compared to a \c GeneralPolyhedralAbstraction, a more
 * compact abstraction based on geometric adjacency is sufficient.
 *
 * We shall call \c Tile a maximal connected set of patches of the same observable contained in the invariant.
 * Every \c Tile is, therefore, a polyhedron.
 *
 * The abstraction is formally defined as a tuple \f$G = (V, E, \text{pts}, \text{obs})\f$ where:
 * - \f$V\f$ contains nodes of two types:
 *   1. \f$(P)\f$ for each \c Tile \f$P\f$ (single-tile nodes, represented as a \c TileNode)
 *   2. \f$(P,Q,R)\f$ for each triple of \c Tile s \f$P, Q, R\f$ (triple-tile nodes, represented as a \c TripleTileNode)
 * - \f$E\f$ contains edges connecting compatible nodes:
 *   - \f$(P) \rightarrow (P,Q,R)\f$ if the nodes \f$(P)\f$ and \f$(P,Q,R)\f$ both belong to \f$V\f$
 *   - \f$(P,Q,R) \rightarrow (R)\f$ if the nodes \f$(P,Q,R)\f$ and \f$(R)\f$ both belong to \f$V\f$
 *   - For \f$(P) \in V\f$ nodes (\c TileNode):
 *      - \f$\mathit{obs}(v) = \mathit{obs}(P)\f$ (\ref TileNode::observable())
 *      - \f$\mathit{pts}(v) = P\f$ (\ref TileNode::points())
 *   - For \f$(P,Q,R) \in V\f$ nodes (\c TripleTileNode):
 *      - \f$\mathit{obs}(v) = \mathit{obs}(Q) \cup \{\mathit{sing}\}\f$ (\ref TripleTileNode::observable())
 *      - \f$\mathit{pts}(v) = Q \cap \mathit{cl}(P) \cap \mathit{cl}(R)\f$ (\ref TripleTileNode::points())
 *
 * \see PolyhedralAbstraction
 * \see isOmnidirectionalFlow
 * \see Tile
 * \see TileExtractor
 * \see TileNode
 * \see TripleTileNode
 * \see GeneralPolyhedralAbstraction
 */
class OmnidirectionalPolyhedralAbstraction: public PolyhedralAbstraction
{
public:
    /**
     * \brief Constructs an OmnidirectionalPolyhedralAbstraction
     * \param polyhedralSystem The \c PolyhedralSystem to abstract
     * \param name Optional name for the abstraction (default: "OmnidirectionalPolyhedralAbstraction")
     */
    explicit OmnidirectionalPolyhedralAbstraction(
        PolyhedralSystemSharedPtr polyhedralSystem,
        std::string_view name = "OmnidirectionalPolyhedralAbstraction"
    );

    [[nodiscard]] const Observable& observable(unsigned state) const override;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const override;

protected:
    /// Map of \c Automaton::m_automaton state to node (either \c TileNode or \c TripleTileNode)
    std::unordered_map<unsigned, std::variant<TileNode, TripleTileNode>> m_tileNodes {};

    /**
     * \brief Constructs the abstraction graph from the given set of the extracted \c Tile s.
     * \param tiles A vector containing the extracted \c Tile s from the \c PolyhedralSystem.
     *
     * \post Populates the \c OmnidirectionalPolyhedralAbstraction::m_tileNodes and constructs
     *       the corresponding \c OmnidirectionalPolyhedralAbstraction::m_automaton.
     */
    void buildAbstraction(std::vector<Tile>&& tiles);

    /**
     * \brief Retrieves the state associated with a \c Tile, or creates a new one if it doesn't exist.
     * \param tile The \c Tile for which the state is retrieved or created.
     * \param stateByTile A map that tracks the existing states for each \c Tile.
     * \return The state associated with the \c Tile.
     *
     * \post If the \c Tile does not exist in the map, a new state is created and added to \p stateByTile.
     */
    unsigned getStateByTileOrCreate(const Tile& tile, std::unordered_map<Tile, unsigned>& stateByTile);

    /**
     * \brief Processes a triple of \c Tile s to create nodes and edges in the abstraction
     * \param tile1 \f$P\f$
     * \param tile2 \f$Q\f$
     * \param tile3 \f$R\f$
     * \param[in,out] stateByTile Mapping between \c Tile objects and their corresponding state
     */
    void processTriple(const Tile& tile1, const Tile& tile2, const Tile& tile3, std::unordered_map<Tile, unsigned>& stateByTile);

    std::vector<Tile> extractTilesFromPolyhedralSystem(PolyhedralSystemSharedPtr polyhedralSystem);
};