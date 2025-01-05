#ifndef BACKWARDAUTOMATON
#define BACKWARDAUTOMATON

#include <spot/tl/formula.hh>
#include <spot/twa/fwd.hh>
#include "State.h"
#include "LabelDenotationMap.h"

class BackwardNFA {
public:

    explicit BackwardNFA(spot::formula&& rtlf, LabelDenotationMap& labelDenotationMap);

    [[nodiscard]] const std::vector<State>& states() const;
    [[nodiscard]] const std::vector<State*>& finalStates() const;
    [[nodiscard]] const std::vector<State*>& predecessors(int state) const;
    [[nodiscard]] bool hasPredecessors(int state) const;
    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] const spot::formula& discretizedLtlFormula() const;

private:
    std::vector<State> m_states {};
    std::vector<std::vector<State*>> m_predecessors {};
    std::vector<State*> m_finalStates {};
    spot::twa_graph_ptr m_nfa {};
    spot::formula m_discretizedLtlFormula {};

    void buildAutomaton(LabelDenotationMap& labelDenotationMap);
    bool isInitial(unsigned state) const;
};

std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa);
std::ostream& operator<< (std::ostream& out, const std::vector<State*>& states);
std::ostream& operator<<(std::ostream& out, const std::vector<State>& states);

#endif
