#include "PolyhedralSynchronousProductAutomaton.h"

#include <spot/twaalgos/reachiter.hh>

PolyhedralSynchronousProductAutomaton::PolyhedralSynchronousProductAutomaton(
    PolyhedralLtlAutomatonConstSharedPtr ltlAutomaton,
    PolyhedralAbstractionConstSharedPtr abstraction,
    const std::string_view name
) : Automaton(name)
  , m_ltlAutomaton { ltlAutomaton }
  , m_abstraction { abstraction }
{
    buildAutomaton();
}

void PolyhedralSynchronousProductAutomaton::buildAutomaton()
{
    PolyhedralSynchronousProductAutomaton::initializeAutomaton();

    m_productStatePair = std::vector<std::pair<unsigned, unsigned>>(m_ltlAutomaton->totalStates() * m_abstraction->totalStates());
    std::vector stateProductByPair(m_ltlAutomaton->totalStates(), std::vector(m_abstraction->totalStates(), -1));

    for (unsigned nfaState { 0 }; nfaState < m_ltlAutomaton->totalStates(); ++nfaState)
    {
        for (unsigned abstractionState { 0 }; abstractionState < m_abstraction->totalStates(); ++abstractionState)
        {
            if (stateDenotationContainsAbstractionPoints(nfaState, abstractionState))
            {
                unsigned productState { m_automaton->new_state() };
                m_productStatePair[productState] = std::make_pair(nfaState, abstractionState);
                stateProductByPair[nfaState][abstractionState] = productState;
                if (m_ltlAutomaton->isInitialState(nfaState)) m_initialStates.insert(productState);
                if (m_ltlAutomaton->isAcceptingState(nfaState)) m_acceptingStates.insert(productState);
            }
        }
    }

    for (unsigned productState { 0 }; productState < m_automaton->num_states(); ++productState)
    {
        auto [nfaState, abstractionState] = m_productStatePair[productState];
        for (auto nfaEdge: m_ltlAutomaton->successors(nfaState))
        {
            for (auto abstractionEdge: m_abstraction->successors(abstractionState))
            {
                int productStateSuccessor { stateProductByPair[nfaEdge.dst][abstractionEdge.dst] };
                if (productStateSuccessor != -1)
                {
                    m_automaton->new_edge(productState, productStateSuccessor, nfaEdge.cond, nfaEdge.acc);
                }
            }
        }
    }
}

void PolyhedralSynchronousProductAutomaton::initializeAutomaton()
{
    m_automaton = std::make_shared<spot::twa_graph>(m_ltlAutomaton->twa()->get_dict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(m_ltlAutomaton->twa()->get_acceptance());
}

bool PolyhedralSynchronousProductAutomaton::stateDenotationContainsAbstractionPoints(
    const unsigned nfaState,
    const unsigned abstractionState
)
{
    const auto& nfaStateDenotation { m_ltlAutomaton->stateDenotation(nfaState) };
    return nfaStateDenotation.denotation()->contains(*m_abstraction->points(abstractionState));
}

unsigned PolyhedralSynchronousProductAutomaton::totalAcceptingStates() const
{
    return m_acceptingStates.size();
}

unsigned PolyhedralSynchronousProductAutomaton::totalInitialStates() const
{
    return m_initialStates.size();
}

const std::unordered_set<unsigned>& PolyhedralSynchronousProductAutomaton::initialStates() const
{
    return m_initialStates;
}

unsigned PolyhedralSynchronousProductAutomaton::isInitialState(const unsigned state) const
{
    return m_initialStates.count(state);
}

bool PolyhedralSynchronousProductAutomaton::isAcceptingState(const unsigned state) const
{
    return m_acceptingStates.count(state) > 0;
}

const std::unordered_set<unsigned>& PolyhedralSynchronousProductAutomaton::acceptingStates() const
{
    return m_acceptingStates;
}

PowersetConstSharedPtr PolyhedralSynchronousProductAutomaton::points(const unsigned state) const
{
    assertThatStateIsInRange(state);

    const auto [nfaState, abstractionState] { m_productStatePair[state] };
    return m_abstraction->points(abstractionState);
}

std::pair<unsigned, unsigned> PolyhedralSynchronousProductAutomaton::productStatePair(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_productStatePair.at(state);
}
