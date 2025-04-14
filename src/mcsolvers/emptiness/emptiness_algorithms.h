#pragma once

#include <spot/twaalgos/se05.hh>
#include <spot/twaalgos/tau03.hh>
#include <spot/twaalgos/magic.hh>
#include <spot/twaalgos/reachiter.hh>

#include "PolyhedralSynchronousProductAutomaton.h"
#include "ExplicitEmptinessCheckDenotationResult.h"
#include "Timer.h"

enum class ExplicitEmptinessCheckAlgorithm
{
    se05,
    tau03,
    magic
};

static spot::emptiness_check_ptr emptinessCheckAlgorithm(
    const ExplicitEmptinessCheckAlgorithm algorithm,
    spot::const_twa_graph_ptr twa
)
{
    switch (algorithm)
    {
    case ExplicitEmptinessCheckAlgorithm::se05:
        return spot::explicit_se05_search(twa);
    case ExplicitEmptinessCheckAlgorithm::tau03:
        return spot::explicit_tau03_search(twa);
    case ExplicitEmptinessCheckAlgorithm::magic:
        return spot::explicit_magic_search(twa);
    }

    throw std::invalid_argument("Invalid EmptinessCheckAlgorithm");
}

inline ExplicitEmptinessCheckDenotationResult explicitEmptinessCheckDenotationSearch(
    PolyhedralSynchronousProductAutomatonConstSharedPtr synchronousProductAutomaton,
    const ExplicitEmptinessCheckAlgorithm algorithm = ExplicitEmptinessCheckAlgorithm::magic
)
{
    Timer timer {};
    PowersetSharedPtr result { std::make_shared<Powerset>(synchronousProductAutomaton->spaceDimension(), PPL::EMPTY) };
    spot::emptiness_check_ptr emptinessCheckSearch { emptinessCheckAlgorithm(algorithm, synchronousProductAutomaton->twa()) };

    std::vector<spot::twa_run> acceptingRuns {};
    std::set<unsigned> initialStates {};
    spot::emptiness_check_result_ptr emptinessCheckResult { emptinessCheckSearch->check() };
    while (emptinessCheckResult != nullptr)
    {
        spot::twa_run_ptr twaRun { emptinessCheckResult->accepting_run() };
        acceptingRuns.push_back(*twaRun);

        std::list<spot::twa_run::step>::iterator prefixStepIterator { twaRun->prefix.begin() };
        ++prefixStepIterator; // skip dummy initial state

        const spot::state* initialState { prefixStepIterator->s };
        const unsigned initialStateNumber { synchronousProductAutomaton->twa()->state_number(initialState) };
        initialStates.insert(initialStateNumber);
        assert(synchronousProductAutomaton->isInitialState(initialStateNumber));

        PowersetConstSharedPtr initialStatePoints { synchronousProductAutomaton->points(initialStateNumber) };
        PPLUtils::fusion(*result, *initialStatePoints);

        emptinessCheckResult = emptinessCheckSearch->check();
    }

    return { acceptingRuns, initialStates, static_cast<int>(acceptingRuns.size()), acceptingRuns.size() == 0, result, timer.elapsedInSeconds() };
}

static spot::twa_graph_ptr buildTransposedTwaWithInitialStateWithEdgesToAcceptingStatesInNonTrivialScc(const PolyhedralSynchronousProductAutomaton& automaton);
static spot::twa_graph_ptr buildTransposedTwaWithInitialStateWithEdgesToAcceptingStates(const PolyhedralSynchronousProductAutomaton& automaton);

inline std::unordered_set<unsigned> collectInitialStatesWithAcceptingRuns(const PolyhedralSynchronousProductAutomaton& automaton)
{
    spot::twa_graph_ptr transposedTwa {};
    if (automaton.acceptance() == AutomatonAcceptance::buchi)
    {
        transposedTwa = buildTransposedTwaWithInitialStateWithEdgesToAcceptingStatesInNonTrivialScc(automaton);
    }
    else
    {
        transposedTwa = buildTransposedTwaWithInitialStateWithEdgesToAcceptingStates(automaton);
    }

    class CollectReachableInitialStatesDfs : public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit CollectReachableInitialStatesDfs(const spot::twa_graph_ptr& twa, const Automaton& automaton)
            : twa_reachable_iterator_depth_first(twa)
            , m_transposedTwa { twa }
            , m_automaton { automaton }
        {
        }

        void process_state(const spot::state* state, [[maybe_unused]] const int _, spot::twa_succ_iterator* si) override
        {
            unsigned stateNumber { m_transposedTwa->state_number(state) };
            if (m_automaton.isInitialState(stateNumber))
            {
                m_reachableInitialStates.insert(stateNumber);
            }
        }

        const spot::twa_graph_ptr& m_transposedTwa;
        std::unordered_set<unsigned> m_reachableInitialStates {};
        const Automaton& m_automaton {};
    };

    return CollectReachableInitialStatesDfs { transposedTwa, automaton }.m_reachableInitialStates;
}

static spot::twa_graph_ptr buildTransposedTwaWithInitialStateWithEdgesToAcceptingStatesInNonTrivialScc(const PolyhedralSynchronousProductAutomaton& automaton)
{
    spot::const_twa_graph_ptr twa { automaton.twa() };
    spot::scc_info sccInfo { twa };

    std::unordered_set<unsigned> acceptingStatesInScc {};
    for (auto sccNode: sccInfo)
    {
        const auto& sccStates { sccNode.states() };
        if (sccStates.size() <= 1)
        {
            continue;
        }

        for (unsigned sccState: sccStates)
        {
            if (automaton.isAcceptingState(sccState))
            {
                acceptingStatesInScc.insert(sccState);
            }
        }
    }

    spot::twa_graph_ptr transposedTwa { SpotUtils::transpose(twa) };
    unsigned initialState { transposedTwa->new_state() };
    transposedTwa->set_init_state(initialState);
    for (unsigned sccState: acceptingStatesInScc)
    {
        transposedTwa->new_acc_edge(initialState, sccState, bdd_true());
    }
    return transposedTwa;
}

static spot::twa_graph_ptr buildTransposedTwaWithInitialStateWithEdgesToAcceptingStates(const PolyhedralSynchronousProductAutomaton& automaton)
{
    spot::const_twa_graph_ptr twa { automaton.twa() };
    spot::twa_graph_ptr transposedTwa { SpotUtils::transpose(twa) };
    unsigned initialState { transposedTwa->new_state() };
    transposedTwa->set_init_state(initialState);
    for (unsigned acceptingState: automaton.acceptingStates())
    {
        transposedTwa->new_acc_edge(initialState, acceptingState, bdd_true());
    }
    return transposedTwa;
}