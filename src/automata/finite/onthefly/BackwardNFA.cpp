#include "BackwardNFA.h"
#include "Timer.h"

BackwardNFA::BackwardNFA()
{
}

BackwardNFA::BackwardNFA(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
): PolyhedralFiniteLtlAutomaton(
    discreteLtlFormula,
    std::move(polyhedralSystemLabelDenotationMap),
    name
  )
{
}

BackwardNFA::BackwardNFA(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
): PolyhedralFiniteLtlAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemLabelDenotationMap),
        name
   )
{
}

BackwardNFA::BackwardNFA(const BackwardNFA& other): PolyhedralFiniteLtlAutomaton(other)
{
}

BackwardNFAConstSharedPtr BackwardNFA::buildAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    return buildAutomaton(
        DiscreteLtlFormula { discreteLtlFormula },
        std::move(polyhedralSystemLabelDenotationMap),
        optimizationLevel,
        anyOption,
        name
    );
}

BackwardNFAConstSharedPtr BackwardNFA::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr backwardNfa {
        std::make_shared<BackwardNFA>(
            BackwardNFA {
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemLabelDenotationMap),
                name
            }
        )
    };

    Timer timer {};
    backwardNfa->m_optimizationLevel = optimizationLevel;
    spot::twa_graph_ptr formulaTgba { backwardNfa->translateDiscreteLtlFormulaIntoTgba(anyOption) };
    spot::twa_graph_ptr nfa { backwardNfa->convertToNfa(formulaTgba) };
    backwardNfa->eraseInitialEdgesWithEmptyDenotation(nfa);
    std::unordered_set nfaAcceptingStates { backwardNfa->killAcceptingStates(nfa) };
    backwardNfa->purgeUnreachableStatesThenRenumberAcceptingStates(nfa, nfaAcceptingStates);
    backwardNfa->PolyhedralLtlAutomaton::buildAutomaton(nfa, nfaAcceptingStates);
    backwardNfa->onConstructionCompleted(timer.elapsedInSeconds());

    return backwardNfa;
}

void BackwardNFA::createNewEdge(const unsigned srcState, const unsigned dstState)
{
    const bool isDstAccepting { m_acceptingStates.count(dstState) == 1 };
    bdd labels { stateLabelsAsBdd(srcState) };
    m_automaton->new_acc_edge(dstState, srcState, labels, isDstAccepting);
    const bool isSrcAccepting { m_acceptingStates.count(srcState) == 1 };
    if (isSrcAccepting && !m_automaton->state_is_accepting(srcState))
    {
        m_automaton->new_acc_edge(srcState, srcState, bdd_false(), true);
        ++m_dummyEdges;
        assert(m_automaton->state_is_accepting(srcState));
    }
}

void BackwardNFA::postprocessAutomaton()
{
    createDummyInitialStateWithEdgesToReachableAcceptingStates();
    purgeUnreachableStates();
}

void BackwardNFA::purgeUnreachableStates()
{
    spot::twa_graph::shift_action renumberNfaAcceptingStatesStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_initialStates, &m_acceptingStates, &m_stateDenotationById, &m_dummyInitialState };
    m_automaton->purge_unreachable_states(&renumberNfaAcceptingStatesStates, &renumberingContext);
}

void BackwardNFA::createDummyInitialStateWithEdgesToReachableAcceptingStates()
{
    m_dummyInitialState = m_automaton->new_state();
    m_automaton->set_init_state(m_dummyInitialState);
    for (const unsigned acceptingState: m_acceptingStates)
    {
        if (isInitialState(acceptingState) || hasSuccessors(acceptingState))
        {
            m_automaton->new_edge(m_dummyInitialState, acceptingState, bdd_true());
            ++m_dummyEdges;
        }
    }
}
