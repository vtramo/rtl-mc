#pragma once

#include <spot/twaalgos/product.hh>

#include "FiniteLtlAutomaton.h"
#include "OmnidirectionalPolyhedralAbstraction.h"

class FiniteSynchronousProduct: public Automaton
{
public:
    FiniteSynchronousProduct(
        FiniteLtlAutomatonConstSharedPtr nfa,
        OmnidirectionalPolyhedralAbstractionConstSharedPtr abstraction,
        std::string_view name = "FiniteSynchronousProduct"
    );

    [[nodiscard]] unsigned isInitialState(unsigned state) const override;
    [[nodiscard]] bool isAcceptingState(unsigned state) const override;
    [[nodiscard]] unsigned totalInitialStates() const override;
    [[nodiscard]] unsigned totalAcceptingStates() const override;
    [[nodiscard]] const std::unordered_set<unsigned>& initialStates() const;
    [[nodiscard]] const std::unordered_set<unsigned>& acceptingStates() const;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const;
    [[nodiscard]] std::pair<unsigned, unsigned> productStatePair(unsigned state) const;

private:
    spot::product_states m_productStatePair {};
    FiniteLtlAutomatonConstSharedPtr m_nfa {};
    OmnidirectionalPolyhedralAbstractionConstSharedPtr m_abstraction {};
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};

    void buildAutomaton();
    bool stateDenotationContainsAbstractionPoints(unsigned nfaState, unsigned abstractionState);
};
