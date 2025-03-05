#pragma once

#include <spot/twa/twagraph.hh>

class Automaton
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    Automaton();
    explicit Automaton(std::string_view name);
    explicit Automaton(std::string_view name, spot::twa_graph_ptr automaton);
    virtual ~Automaton() = default;

    [[nodiscard]] virtual unsigned totalStates() const;
    [[nodiscard]] virtual unsigned totalInitialStates() const;
    [[nodiscard]] virtual unsigned totalAcceptingStates() const;
    [[nodiscard]] virtual unsigned totalEdges() const;
    [[nodiscard]] virtual unsigned isInitialState(unsigned state) const;
    [[nodiscard]] virtual bool isAcceptingState(unsigned state) const;
    [[nodiscard]] virtual EdgeIterator successors(unsigned state) const;
    [[nodiscard]] virtual bool hasSuccessors(unsigned state) const;
    [[nodiscard]] virtual int countSuccessors(unsigned state) const;
    [[nodiscard]] virtual spot::const_twa_graph_ptr twa() const;
    [[nodiscard]] virtual spot::twa_graph_ptr transpose() const;
    void printDotFormat(std::ostream& os) const;

protected:
    spot::twa_graph_ptr m_automaton {};
    std::string m_name {};

    virtual void initializeAutomaton();
    virtual void assertThatStateIsInRange(unsigned state) const;
};
