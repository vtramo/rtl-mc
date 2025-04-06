#include "BackwardNFA.h"
#include "Timer.h"

BackwardNFA::BackwardNFA()
{
}

BackwardNFA::BackwardNFA(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
): PolyhedralLtlFiniteAutomaton(
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
): PolyhedralLtlFiniteAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemLabelDenotationMap),
        name
   )
{
}

BackwardNFA::BackwardNFA(const BackwardNFA& other): PolyhedralLtlFiniteAutomaton(other)
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
    backwardNfa->PolyhedralLtlAutomaton::purgeUnreachableStates(nfa);
    backwardNfa->PolyhedralLtlAutomaton::buildAutomaton(nfa);
    backwardNfa->onConstructionCompleted(timer.elapsedInSeconds());

    return backwardNfa;
}

void BackwardNFA::createNewEdge(const unsigned srcState, const unsigned dstState)
{
    const bool isSrcAccepting { m_acceptingStates.count(srcState) == 1 };
    if (isSrcAccepting)
    {
        return;
    }

    const bool isDstAccepting { m_acceptingStates.count(dstState) == 1 };
    bdd labels { stateLabelsAsBdd(srcState) };
    m_automaton->new_acc_edge(dstState, srcState, labels, isDstAccepting);
}

void BackwardNFA::postprocessAutomaton()
{
    createDummyInitialStateWithEdgesToReachableAcceptingStates();
    purgeUnreachableStates();
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
