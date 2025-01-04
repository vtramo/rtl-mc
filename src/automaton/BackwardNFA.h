#ifndef BACKWARDAUTOMATON
#define BACKWARDAUTOMATON

#include <spot/tl/formula.hh>
#include <spot/twa/fwd.hh>
#include "State.h"

class BackwardNFA {
public:

    explicit BackwardNFA(spot::formula&& rtlf);

    [[nodiscard]] const std::vector<State*>& predecessors(int state) const;
    [[nodiscard]] const std::vector<State*>& finalStates() const;
    [[nodiscard]] bool hasPredecessors(int state) const;
    [[nodiscard]] int totalStates() const;

private:
    std::vector<State> m_states {};
    std::vector<std::vector<State*>> m_predecessors {};
    std::vector<State*> m_finalStates {};
    spot::twa_graph_ptr m_nfa {};

    void buildAutomaton();
    bool isInitial(unsigned state) const;
};

#endif
