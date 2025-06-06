#include "PolyhedralSynchronousProductAutomaton.h"

#include <spdlog/fmt/bundled/ranges.h>
#include <spot/twaalgos/reachiter.hh>

PolyhedralSynchronousProductAutomaton::PolyhedralSynchronousProductAutomaton(
    PolyhedralLtlAutomatonConstSharedPtr ltlAutomaton,
    PolyhedralAbstractionConstSharedPtr abstraction,
    const std::string_view name
) : Automaton(name)
  , m_ltlAutomaton { ltlAutomaton }
  , m_abstraction { abstraction }
{
    Log::log(Verbosity::verbose, "\n[{}] Construction started.", name);
    Log::log(Verbosity::verbose, "[{}] Polyhedral Abstraction total states: {}.", name, m_abstraction->totalStates());
    Log::log(Verbosity::verbose, "[{}] LTL Automaton total states: {}.", name, m_ltlAutomaton->totalStates());

    buildAutomaton();
}

void PolyhedralSynchronousProductAutomaton::buildAutomaton()
{
    PolyhedralSynchronousProductAutomaton::initialiseAutomaton();

    m_productStatePair = std::vector<std::pair<unsigned, unsigned>>(m_ltlAutomaton->totalStates() * m_abstraction->totalStates());
    std::vector stateProductByPair(m_ltlAutomaton->totalStates(), std::vector(m_abstraction->totalStates(), -1));

    for (unsigned ltlAutomatonState { 0 }; ltlAutomatonState < m_ltlAutomaton->totalStates(); ++ltlAutomatonState)
    {
        for (unsigned abstractionState { 0 }; abstractionState < m_abstraction->totalStates(); ++abstractionState)
        {
            if (stateDenotationContainsAbstractionPoints(ltlAutomatonState, abstractionState))
            {
                unsigned productState { m_automaton->new_state() };
                m_productStatePair[productState] = std::make_pair(ltlAutomatonState, abstractionState);
                stateProductByPair[ltlAutomatonState][abstractionState] = productState;
                if (m_ltlAutomaton->isInitialState(ltlAutomatonState)) m_initialStates.insert(productState);
                if (m_ltlAutomaton->isAcceptingState(ltlAutomatonState)) m_acceptingStates.insert(productState);
            }
        }
    }

    for (unsigned productState { 0 }; productState < m_automaton->num_states(); ++productState)
    {

        auto [ltlAutomatonState, abstractionState] = m_productStatePair[productState];
        for (auto ltlAutomatonEdge: m_ltlAutomaton->successors(ltlAutomatonState))
        {
            if (ltlAutomatonEdge.cond == bdd_false())
            {
                m_automaton->new_acc_edge(productState, productState, bdd_true(), true);
                continue;
            }

            for (auto abstractionEdge: m_abstraction->successors(abstractionState))
            {
                int productStateSuccessor { stateProductByPair[ltlAutomatonEdge.dst][abstractionEdge.dst] };
                if (productStateSuccessor != -1)
                {
                    bdd edgeLabels { ltlAutomatonEdge.cond };
                    m_automaton->new_edge(productState, productStateSuccessor, edgeLabels, ltlAutomatonEdge.acc);
                }
            }
        }

        const bool isProductStateAccepting { m_acceptingStates.count(productState) > 0 };
        assert(!isProductStateAccepting || m_automaton->state_is_accepting(productState));
    }

    createDummyInitialStateWithEdgesToInitialStates();

    Log::logAutomaton(m_automaton, fmt::format("{}-construction-completed", m_name));
    Log::log(Verbosity::verbose, "[{}] Construction completed.", m_name);
    Log::log(Verbosity::verbose, "[{}] Total states: {}.", m_name, totalStates());
    Log::log(Verbosity::verbose, "[{}] Total edges: {}.", m_name, totalEdges());
    Log::log(Verbosity::verbose, "[{}] Total initial states: {}.", m_name, totalInitialStates());
    Log::log(Verbosity::verbose, "[{}] Total accepting states: {}.", m_name, totalAcceptingStates());
    Log::log(Verbosity::veryVerbose, "[{}] Initial states: {}.", m_name, fmt::join(initialStates(), ", "));
    Log::log(Verbosity::veryVerbose, "[{}] Accepting states: {}.", m_name, fmt::join(acceptingStates(), ", "));
    Log::log(Verbosity::debug, "{}", *this);
}

void PolyhedralSynchronousProductAutomaton::createDummyInitialStateWithEdgesToInitialStates()
{
    unsigned dummyInitialState { m_automaton->new_state() };
    m_automaton->set_init_state(dummyInitialState);
    for (const unsigned initialState: m_initialStates)
    {
        auto [ltlAutomatonState, abstractionState] { m_productStatePair[initialState] };
        const Observable& observable { m_abstraction->observable(abstractionState) };
        bdd observableAsBdd { m_abstraction->observableAsBdd(observable) };
        m_automaton->new_edge(dummyInitialState, initialState, observableAsBdd);
    }
}

void PolyhedralSynchronousProductAutomaton::initialiseAutomaton()
{
    m_automaton = std::make_shared<spot::twa_graph>(m_ltlAutomaton->twa()->get_dict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(m_ltlAutomaton->twa()->get_acceptance());
}

bool PolyhedralSynchronousProductAutomaton::stateDenotationContainsAbstractionPoints(
    const unsigned ltlAutomatonState,
    const unsigned abstractionState
)
{
    const auto& ltlAutomatonStateDenotation { m_ltlAutomaton->stateDenotation(ltlAutomatonState) };
    return ltlAutomatonStateDenotation.denotation()->contains(*m_abstraction->points(abstractionState));
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

bool PolyhedralSynchronousProductAutomaton::isInitialState(const unsigned state) const
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

    const auto [ltlAutomatonState, abstractionState] { m_productStatePair[state] };
    return m_abstraction->points(abstractionState);
}

std::pair<unsigned, unsigned> PolyhedralSynchronousProductAutomaton::productStatePair(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_productStatePair.at(state);
}

PPL::dimension_type PolyhedralSynchronousProductAutomaton::spaceDimension() const
{
    return m_abstraction->spaceDimension();
}

std::ostream& operator<< (std::ostream& os, const PolyhedralSynchronousProductAutomaton& synchronousProduct)
{
    os << "[" << synchronousProduct.name() << "]" << std::endl;

    auto polyhedralSystem { synchronousProduct.m_abstraction->polyhedralSystem() };
    for (unsigned state { 0 }; state < synchronousProduct.totalStates(); ++state)
    {
        auto statePoints { synchronousProduct.points(state) };
        os << "State: " << state << std::endl;
        os << std::boolalpha << "Is initial: " << synchronousProduct.isInitialState(state) << std::endl;
        os << "Is accepting: " << synchronousProduct.isAcceptingState(state) << std::endl << std::noboolalpha;
        os << "Points: " << PPLOutput::toString(*statePoints, polyhedralSystem->symbolTable()) << std::endl << std::endl;
    }

    return os;
}