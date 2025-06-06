#pragma once

#include <spot/twaalgos/product.hh>

#include "PolyhedralLtlAutomaton.h"
#include "PolyhedralAbstraction.h"

class PolyhedralSynchronousProductAutomaton;
using PolyhedralSynchronousProductAutomatonConstSharedPtr = std::shared_ptr<const PolyhedralSynchronousProductAutomaton>;
using PolyhedralSynchronousProductAutomatonSharedPtr = std::shared_ptr<PolyhedralSynchronousProductAutomaton>;

class PolyhedralSynchronousProductAutomaton: public Automaton
{
public:
    PolyhedralSynchronousProductAutomaton(
        PolyhedralLtlAutomatonConstSharedPtr ltlAutomaton,
        PolyhedralAbstractionConstSharedPtr abstraction,
        std::string_view name = "PolyhedralSynchronousProductAutomaton"
    );

    [[nodiscard]] bool isInitialState(unsigned state) const override;
    [[nodiscard]] bool isAcceptingState(unsigned state) const override;
    [[nodiscard]] unsigned totalInitialStates() const override;
    [[nodiscard]] unsigned totalAcceptingStates() const override;
    [[nodiscard]] const std::unordered_set<unsigned>& initialStates() const;
    [[nodiscard]] const std::unordered_set<unsigned>& acceptingStates() const;
    [[nodiscard]] PowersetConstSharedPtr points(unsigned state) const;
    [[nodiscard]] std::pair<unsigned, unsigned> productStatePair(unsigned state) const;
    [[nodiscard]] PPL::dimension_type spaceDimension() const;
    [[nodiscard]] AutomatonAcceptance acceptance() const override { return m_ltlAutomaton->acceptance(); }

    friend std::ostream& operator<< (std::ostream& os, const PolyhedralSynchronousProductAutomaton& synchronousProduct);

private:
    spot::product_states m_productStatePair {};
    PolyhedralLtlAutomatonConstSharedPtr m_ltlAutomaton {};
    PolyhedralAbstractionConstSharedPtr m_abstraction {};
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};

    void initialiseAutomaton() override;
    void buildAutomaton();
    bool stateDenotationContainsAbstractionPoints(unsigned ltlAutomatonState, unsigned abstractionState);
    void createDummyInitialStateWithEdgesToInitialStates();
};