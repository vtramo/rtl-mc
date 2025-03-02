#pragma once

#include <spot/twaalgos/product.hh>
#include <spot/twaalgos/dot.hh>

#include "FiniteLtlAutomaton.h"
#include "OmnidirectionalPolyhedralAbstraction.h"

class FiniteSynchronousProduct
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    FiniteSynchronousProduct(
        FiniteLtlAutomatonConstSharedPtr nfa,
        OmnidirectionalPolyhedralAbstractionConstSharedPtr abstraction
    );

    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalInitialStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] bool isInitialState(unsigned state) const;
    [[nodiscard]] bool isFinalState(unsigned state) const;
    [[nodiscard]] const std::unordered_set<int>& initialStates() const;
    [[nodiscard]] const std::unordered_set<int>& finalStates() const;
    [[nodiscard]] bool hasSuccessors(unsigned state) const;
    [[nodiscard]] EdgeIterator successors(unsigned state) const;
    [[nodiscard]] int countSuccessors(unsigned state) const;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const;
    [[nodiscard]] std::pair<int, int> productStatePair(unsigned state) const;
    [[nodiscard]] PowersetConstSharedPtr existentialDenotation() const;

    void printDotFormat(std::ostream& os) const;

private:
    spot::twa_graph_ptr m_synchronousProduct {};
    spot::product_states* m_productStatePair {};
    FiniteLtlAutomatonConstSharedPtr m_nfa {};
    OmnidirectionalPolyhedralAbstractionConstSharedPtr m_abstraction {};
    std::unordered_map<int, PowersetConstSharedPtr> m_denotationByState {};
    std::unordered_set<int> m_initialStates {};
    std::unordered_set<int> m_finalStates {};
    PowersetSharedPtr m_existentialDenotation {};

    void buildAutomaton();
    spot::const_twa_graph_ptr twa();
};
