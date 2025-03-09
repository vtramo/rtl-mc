#pragma once

#include <spot/twaalgos/product.hh>

#include "PolyhedralLtlAutomaton.h"
#include "PolyhedralAbstraction.h"

class PolyhedralSynchronousProductAutomaton;
using PolyhedralSynchronousProductAutomatonConstSharedPtr = std::shared_ptr<PolyhedralSynchronousProductAutomaton>;

class PolyhedralSynchronousProductAutomaton: public Automaton
{
public:
    PolyhedralSynchronousProductAutomaton(
        PolyhedralLtlAutomatonConstSharedPtr ltlAutomaton,
        PolyhedralAbstractionConstSharedPtr abstraction,
        std::string_view name = "PolyhedralSynchronousProductAutomaton"
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
    PolyhedralLtlAutomatonConstSharedPtr m_ltlAutomaton {};
    PolyhedralAbstractionConstSharedPtr m_abstraction {};
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};

    void initializeAutomaton() override;
    void buildAutomaton();
    bool stateDenotationContainsAbstractionPoints(unsigned nfaState, unsigned abstractionState);
};
