#include "PolyhedralBuchiLtlAutomaton.h"

#include <Timer.h>

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

    Timer timer {};
    finiteLtlAutomaton->m_optimizationLevel = optimizationLevel;
    spot::twa_graph_ptr formulaTgba { finiteLtlAutomaton->translateDiscreteLtlFormulaIntoTgba(anyOption) };
    finiteLtlAutomaton->eraseInitialEdgesWithEmptyDenotation(formulaTgba);
    std::unordered_set acceptingStates { SpotUtils::collectAcceptingStates(formulaTgba) };
    finiteLtlAutomaton->purgeUnreachableStatesThenRenumberAcceptingStates(formulaTgba, acceptingStates);
    finiteLtlAutomaton->PolyhedralLtlAutomaton::buildAutomaton(formulaTgba, acceptingStates);
    finiteLtlAutomaton->onConstructionCompleted(timer.elapsedInSeconds());

    return finiteLtlAutomaton;
}
