#pragma once

#include <PolyhedralSystemLabelDenotationMap.h>
#include <spot/twa/twagraph.hh>
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"

class BackwardNFA {
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    BackwardNFA(const DiscreteLtlFormula& discreteLtlFormula, PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap);
    BackwardNFA(DiscreteLtlFormula&& discreteLtlFormula, PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap);

    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] bool isInitialState(int state) const;
    [[nodiscard]] bool isFinalState(int state) const;
    [[nodiscard]] bool hasPredecessors(int state) const;
    [[nodiscard]] EdgeIterator predecessors(int state) const;
    [[nodiscard]] const std::unordered_set<int>& finalStates() const;
    [[nodiscard]] const DiscreteLtlFormula& formula() const;
    [[nodiscard]] const StateDenotation& stateDenotation(int state) const;

    void printHoaFormat(std::ostream& os) const;
    void printDotFormat(std::ostream& os) const;

    friend std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa);
private:
    spot::twa_graph_ptr m_backwardNfa {};
    std::unordered_set<int> m_finalStates {};
    std::unordered_map<int, StateDenotation> m_stateDenotationById {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    PolyhedralSystemLabelDenotationMap m_labelDenotationMap {};

    void buildAutomaton(const spot::const_twa_graph_ptr& nfa);
    std::vector<StateDenotation> extractStateDenotationsFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard);
};