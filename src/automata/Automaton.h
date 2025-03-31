#pragma once

#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/sccinfo.hh>

#include "AutomatonStats.h"

class Automaton
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    Automaton();
    explicit Automaton(std::string_view name);
    explicit Automaton(std::string_view name, spot::twa_graph_ptr automaton);
    virtual ~Automaton() = default;

    [[nodiscard]] std::string_view name() const { return m_name; }
    [[nodiscard]] virtual unsigned totalStates() const;
    [[nodiscard]] virtual unsigned totalInitialStates() const;
    [[nodiscard]] virtual unsigned totalAcceptingStates() const;
    [[nodiscard]] virtual unsigned totalEdges() const;
    [[nodiscard]] virtual bool isInitialState(unsigned state) const;
    [[nodiscard]] virtual bool isAcceptingState(unsigned state) const;
    [[nodiscard]] virtual EdgeIterator successors(unsigned state) const;
    [[nodiscard]] virtual bool hasSuccessors(unsigned state) const;
    [[nodiscard]] virtual int countSuccessors(unsigned state) const;
    [[nodiscard]] virtual spot::const_twa_graph_ptr twa() const;
    [[nodiscard]] virtual spot::twa_graph_ptr transpose() const;
    [[nodiscard]] virtual const AutomatonStats& stats() const;
    void printDotFormat(std::ostream& os) const;

protected:
    spot::twa_graph_ptr m_automaton {};
    std::string m_name {};
    std::shared_ptr<AutomatonStats> m_automatonStats {};

    virtual void initialiseAutomaton();
    virtual void initialiseStats()
    {
        m_automatonStats = std::make_unique<AutomatonStats>();
    }
    virtual void assertThatStateIsInRange(unsigned state) const;
    virtual void setAutomatonStats(const double executionTimeSeconds)
    {
        if (!m_automatonStats)
        {
            initialiseStats();
        }

        m_automatonStats->setTotalStates(totalStates());
        m_automatonStats->setTotalInitialStates(totalInitialStates());
        m_automatonStats->setTotalAcceptingStates(totalAcceptingStates());
        m_automatonStats->setTotalEdges(totalEdges());
        m_automatonStats->setExecutionTimeSeconds(executionTimeSeconds);
        m_automatonStats->setSccInfo(spot::scc_info { m_automaton });
    }
};
