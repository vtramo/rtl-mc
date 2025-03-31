/**
 * \file GeneralPolyhedralAbstraction.h
 * \brief Concrete class representing a \c PolyhedralAbstraction of a \c PolyhedralSystem (general case)
 */

#pragma once

#include <unordered_map>

#include "PolyhedralAbstraction.h"
#include "SingOpenStatePair.h"
#include "ObservableTraverseNode.h"
#include "PolyhedralSystem.h"

/**
 * \class GeneralPolyhedralAbstraction
 * \brief Represents a concrete and more general \c PolyhedralAbstraction of a \c PolyhedralSystem whose nodes
 *        are finite sequences of patches of the \c Observable s of the \c PolyhedralSystem
 *
 * Implements the formal abstraction \f$G = (V, E, \text{pts}, \text{obs})\f$ where:
 * - Nodes represent patch sequences with alternating interval types (\f$\flat \in \{0, +\}\f$)
 * - Edges connect compatible trajectory segments
 * - Observables combine patch properties with interval type markers
 *
 * \par Formal Construction:
 * For maximum sequence length \f$H_\phi\f$:
 * \f[
 * \begin{aligned}
 * V &= \{(\flat, \Delta) \mid \flat \in \{0,+\}, \Delta \in P^*, 0 < |\Delta| \leq H_\phi, \text{Trav}_\flat(\Delta) \neq \emptyset\} \\
 * E &= \{((\flat, P\Delta), (\neg\flat, \Delta)) \mid (\flat, P\Delta) \in V\} \\
 * \text{obs}((0, \Delta)) &= \alpha_P \cup \{\text{sing}\} \\
 * \text{obs}((+, \Delta)) &= \alpha_P \\
 * \text{pts}((\flat, \Delta)) &= \text{Trav}_\flat(\Delta)
 * \end{aligned}
 * \f]
 *
 * \extends PolyhedralAbstraction
 * \see PolyhedralAbstraction
 * \see ObservableTraverseNode
 * \see SingOpenStatePair
 */
class GeneralPolyhedralAbstraction: public PolyhedralAbstraction
{
public:
    using GeneralPolyhedralAbstractionConstSharedPtr = std::shared_ptr<GeneralPolyhedralAbstraction>;

    explicit GeneralPolyhedralAbstraction(
        PolyhedralSystemSharedPtr polyhedralSystem,
        std::string_view name = "GeneralPolyhedralAbstraction"
    );

    GeneralPolyhedralAbstraction(
        PolyhedralSystemSharedPtr polyhedralSystem,
        std::vector<Observable>&& observables,
        unsigned maxObservablePatchSequenceLength,
        std::string_view name = "GeneralPolyhedralAbstraction"
    );

    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const override;
    [[nodiscard]] const Observable& observable(unsigned state) const override;
private:
    std::unordered_map<unsigned, ObservableTraverseNode> m_nodeByState {};

    void buildAbstraction(unsigned maxObservablePatchSequenceLength, std::vector<Observable>&& observables);
    SingOpenStatePair createStates(const ObservableTraverseNode& singTravNode, const ObservableTraverseNode& openTravNode);
    void createEdges(SingOpenStatePair singOpenStatePair, const SingOpenStatePair& adjSingOpenStatePair);
    ObservablePatchSequence advanceByOne(const ObservablePatchSequence& observablePatchSequence);
};