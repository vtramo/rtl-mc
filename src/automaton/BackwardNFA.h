#pragma once

#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/postproc.hh>

#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"

class BackwardNFA {
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    BackwardNFA() = default;
    BackwardNFA(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    BackwardNFA(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalInitialStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] bool isInitialState(int state) const;
    [[nodiscard]] bool isFinalState(int state) const;
    [[nodiscard]] const std::unordered_set<int>& initialStates() const;
    [[nodiscard]] const std::unordered_set<int>& finalStates() const;
    [[nodiscard]] bool hasPredecessors(int state) const;
    [[nodiscard]] EdgeIterator predecessors(int state) const;
    [[nodiscard]] const DiscreteLtlFormula& formula() const;
    [[nodiscard]] const StateDenotation& stateDenotation(int state) const;

    void printHoaFormat(std::ostream& os) const;
    void printDotFormat(std::ostream& os) const;

    friend std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa);

private:
    spot::twa_graph_ptr m_backwardNfa {};
    std::unordered_set<int> m_initialStates {};
    std::unordered_set<int> m_finalStates {};
    std::unordered_map<int, StateDenotation> m_stateDenotationById {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    PolyhedralSystemFormulaDenotationMap m_formulaDenotationMap {};

    void buildAutomaton(const spot::const_twa_graph_ptr& nfa);
    StateDenotation extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard);
};