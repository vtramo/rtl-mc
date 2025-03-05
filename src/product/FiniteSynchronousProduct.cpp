#include "FiniteSynchronousProduct.h"

#include <spot/twaalgos/reachiter.hh>

FiniteSynchronousProduct::FiniteSynchronousProduct(
    FiniteLtlAutomatonConstSharedPtr nfa,
    OmnidirectionalPolyhedralAbstractionConstSharedPtr abstraction,
    const std::string_view name
) : Automaton(name)
  , m_nfa { nfa }
  , m_abstraction { abstraction }
{
    buildAutomaton();
}

void FiniteSynchronousProduct::buildAutomaton()
{
    m_automaton = std::make_shared<spot::twa_graph>(m_nfa->twa()->get_dict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(m_nfa->twa()->get_acceptance());
    m_productStatePair = std::vector<std::pair<unsigned, unsigned>>(m_nfa->totalStates() * m_abstraction->totalStates());
    std::vector stateProductByPair(m_nfa->totalStates(), std::vector(m_abstraction->totalStates(), -1));

    for (unsigned nfaState { 0 }; nfaState < m_nfa->totalStates(); ++nfaState)
    {
        for (unsigned abstractionState { 0 }; abstractionState < m_abstraction->totalStates(); ++abstractionState)
        {
            if (stateDenotationContainsAbstractionPoints(nfaState, abstractionState))
            {
                unsigned productState { m_automaton->new_state() };
                m_productStatePair[productState] = std::make_pair(nfaState, abstractionState);
                stateProductByPair[nfaState][abstractionState] = productState;
                if (m_nfa->isInitialState(nfaState)) m_initialStates.insert(productState);
                if (m_nfa->isAcceptingState(nfaState)) m_acceptingStates.insert(productState);
            }
        }
    }

    for (unsigned productState { 0 }; productState < m_automaton->num_states(); ++productState)
    {
        auto [nfaState, abstractionState] = m_productStatePair[productState];
        for (auto nfaEdge: m_nfa->successors(nfaState))
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

bool FiniteSynchronousProduct::stateDenotationContainsAbstractionPoints(
    const unsigned nfaState,
    const unsigned abstractionState
)
{
    const auto& nfaStateDenotation { m_nfa->stateDenotation(nfaState) };
    return nfaStateDenotation.denotation()->contains(*m_abstraction->points(abstractionState));
}

unsigned FiniteSynchronousProduct::totalAcceptingStates() const
{
    return m_acceptingStates.size();
}

unsigned FiniteSynchronousProduct::totalInitialStates() const
{
    return m_initialStates.size();
}

const std::unordered_set<unsigned>& FiniteSynchronousProduct::initialStates() const
{
    return m_initialStates;
}

unsigned FiniteSynchronousProduct::isInitialState(const unsigned state) const
{
    return m_initialStates.count(state);
}

bool FiniteSynchronousProduct::isAcceptingState(const unsigned state) const
{
    return m_acceptingStates.count(state) > 0;
}

const std::unordered_set<unsigned>& FiniteSynchronousProduct::acceptingStates() const
{
    return m_acceptingStates;
}

PowersetConstSharedPtr FiniteSynchronousProduct::points(const unsigned state) const
{
    assertThatStateIsInRange(state);

    const auto [nfaState, abstractionState] { m_productStatePair[state] };
    return m_abstraction->points(abstractionState);
}

std::pair<unsigned, unsigned> FiniteSynchronousProduct::productStatePair(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_productStatePair.at(state);
}
