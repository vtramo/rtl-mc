#pragma once

#include <unordered_map>

#include "PolyhedralAbstraction.h"
#include "SingOpenStatePair.h"
#include "ObservableTraverseNode.h"
#include "PolyhedralSystem.h"

class GeneralPolyhedralAbstraction: public PolyhedralAbstraction
{
public:
    using GeneralPolyhedralAbstractionConstSharedPtr = std::shared_ptr<GeneralPolyhedralAbstraction>;

    explicit GeneralPolyhedralAbstraction(
        PolyhedralSystemConstSharedPtr polyhedralSystem,
        std::string_view name = "GeneralPolyhedralAbstraction"
    );

    GeneralPolyhedralAbstraction(
        PolyhedralSystemConstSharedPtr polyhedralSystem,
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