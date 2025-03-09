#include "PolyhedralBuchiLtlAutomaton.h"

PolyhedralBuchiLtlAutomaton::PolyhedralBuchiLtlAutomaton(const PolyhedralBuchiLtlAutomaton& other)
    : PolyhedralLtlAutomaton(other)
{
}

PolyhedralBuchiLtlAutomaton::PolyhedralBuchiLtlAutomaton()
{
}

PolyhedralBuchiLtlAutomaton::PolyhedralBuchiLtlAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralLtlAutomaton(
        discreteLtlFormula,
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
}

PolyhedralBuchiLtlAutomaton::PolyhedralBuchiLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralLtlAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
}

PolyhedralBuchiLtlAutomatonConstSharedPtr PolyhedralBuchiLtlAutomaton::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr finiteLtlAutomaton {
        std::make_shared<PolyhedralBuchiLtlAutomaton>(
            PolyhedralBuchiLtlAutomaton {
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemLabelDenotationMap),
                name
            }
        )
    };

    finiteLtlAutomaton->m_optimizationLevel = optimizationLevel;
    spot::twa_graph_ptr formulaTgba { finiteLtlAutomaton->translateDiscreteLtlFormulaIntoTgba(anyOption) };
    finiteLtlAutomaton->eraseInitialEdgesWithEmptyDenotation(formulaTgba);
    std::unordered_set acceptingStates { finiteLtlAutomaton->collectAcceptingStates(formulaTgba) };
    finiteLtlAutomaton->purgeUnreachableStatesThenRenumberAcceptingStates(formulaTgba, acceptingStates);
    finiteLtlAutomaton->PolyhedralLtlAutomaton::buildAutomaton(formulaTgba, acceptingStates);

    return finiteLtlAutomaton;
}

std::unordered_set<unsigned> PolyhedralBuchiLtlAutomaton::collectAcceptingStates(spot::const_twa_graph_ptr twaGraph)
{
    std::unordered_set<unsigned> acceptingStates {};
    for (unsigned nfaState { 0 }; nfaState < twaGraph->num_states(); ++nfaState)
    {
        if (twaGraph->state_is_accepting(nfaState))
        {
            acceptingStates.insert(nfaState);
        }
    }
    return acceptingStates;
}
