#pragma once

#include <unordered_map>
#include <spot/twa/fwd.hh>

#include "SingOpenStatePair.h"
#include "ObservableTraversalNode.h"
#include "PolyhedralSystem.h"

class GeneralPolyhedralAbstraction
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    GeneralPolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem, int totalStatesFiniteAutomaton);

    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const;
    [[nodiscard]] const Observable& observable(unsigned state) const;
    [[nodiscard]] int sufficientHorizon() const;
    [[nodiscard]] int initialState() const;
    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] EdgeIterator successors(unsigned state);
    [[nodiscard]] bool hasSuccessors(unsigned state);
    [[nodiscard]] int countSuccessors(unsigned state);
    [[nodiscard]] PPL::dimension_type spaceDimension();
    [[nodiscard]] spot::const_twa_graph_ptr twa() const;

    void printDotFormat(std::ostream& os) const;

private:
    spot::twa_graph_ptr m_graph {};
    std::unordered_map<unsigned, ObservableTraversalNode> m_nodeByState {};
    PPL::dimension_type m_spaceDimension {};
    int m_sufficientHorizon {};

    void computeSufficientHorizon(int totalStatesFiniteAutomaton, const std::vector<Observable>& observables);
    void initializeGraph(spot::bdd_dict_ptr bddDict);
    void buildAbstraction(std::vector<Observable>&& observables, const Poly&);
    SingOpenStatePair createStates(const ObservableTraversalNode& singTravNode, const ObservableTraversalNode& openTravNode);
    void createEdges(SingOpenStatePair singOpenStatePair, const SingOpenStatePair& adjSingOpenStatePair);
    bdd observableAsBdd(const Observable& observable);
    ObservablePatchSequence advanceByOne(const ObservablePatchSequence& observablePatchSequence);
};