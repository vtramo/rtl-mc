#include "Automaton.h"
#include "spot_utils.h"

#include <spot/misc/trival.hh>
#include <spot/twa/acc.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/sccinfo.hh>

Automaton::Automaton()
{
    Automaton::initialiseAutomaton();
    Automaton::initialiseStats();
}

Automaton::Automaton(const std::string_view name)
    : m_name { name }
{
    Automaton::initialiseAutomaton();
    Automaton::initialiseStats();
}

Automaton::Automaton(const std::string_view name, spot::twa_graph_ptr automaton)
    : m_automaton { automaton }
    , m_name { name }
{
    Automaton::initialiseStats();
}

unsigned Automaton::totalStates() const
{
    return m_automaton->num_states();
}

unsigned Automaton::totalInitialStates() const
{
    return 0;
}

unsigned Automaton::totalAcceptingStates() const
{
    return 0;
}

unsigned Automaton::totalEdges() const
{
    return m_automaton->num_edges();
}

bool Automaton::isInitialState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_automaton->get_init_state_number() == state;
}

bool Automaton::isAcceptingState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_automaton->state_is_accepting(state);
}

Automaton::EdgeIterator Automaton::successors(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_automaton->out(state);
}

bool Automaton::hasSuccessors(const unsigned state) const
{
    assertThatStateIsInRange(state);

    auto edgeStorages { m_automaton->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

int Automaton::countSuccessors(const unsigned state) const
{
    assertThatStateIsInRange(state);

    const EdgeIterator& edgeIterator { m_automaton->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

spot::const_twa_graph_ptr Automaton::twa() const
{
    return m_automaton;
}

void Automaton::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_automaton);
}

void Automaton::initialiseAutomaton()
{
    const spot::bdd_dict_ptr dict { std::make_shared<spot::bdd_dict>() };
    m_automaton = std::make_shared<spot::twa_graph>(dict);
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(spot::acc_cond::acc_code::t());
}

void Automaton::assertThatStateIsInRange(const unsigned state) const
{
    if (state >= totalStates())
    {
        throw std::out_of_range("State is out of range!");
    }
}

spot::twa_graph_ptr Automaton::transpose() const
{
    return SpotUtils::transpose(m_automaton);
}

const AutomatonStats& Automaton::stats() const
{
    return *m_automatonStats;
}

void Automaton::setAutomatonStats(const double executionTimeSeconds)
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

void Automaton::initialiseStats()
{
    m_automatonStats = std::make_unique<AutomatonStats>();
}