#pragma once

#include <unordered_map>

#include "PolyhedralAbstraction.h"
#include "SingOpenStatePair.h"
#include "ObservableTraversalNode.h"
#include "PolyhedralSystem.h"

class GeneralPolyhedralAbstraction: public PolyhedralAbstraction
{
public:
    using GeneralPolyhedralAbstractionConstSharedPtr = std::shared_ptr<GeneralPolyhedralAbstraction>;

    explicit GeneralPolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem);

    GeneralPolyhedralAbstraction(
        PolyhedralSystemConstSharedPtr polyhedralSystem,
        std::vector<Observable>&& observables,
        int maxObservablePatchSequenceLength
    );

    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const override;
    [[nodiscard]] const Observable& observable(unsigned state) const override;
private:
    std::unordered_map<unsigned, ObservableTraversalNode> m_nodeByState {};

    void buildAbstraction(int maxObservablePatchSequenceLength, std::vector<Observable>&& observables);
    SingOpenStatePair createStates(const ObservableTraversalNode& singTravNode, const ObservableTraversalNode& openTravNode);
    void createEdges(SingOpenStatePair singOpenStatePair, const SingOpenStatePair& adjSingOpenStatePair);
    ObservablePatchSequence advanceByOne(const ObservablePatchSequence& observablePatchSequence);
};