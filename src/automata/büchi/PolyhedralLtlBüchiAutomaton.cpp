#include "PolyhedralLtlBüchiAutomaton.h"
#include "Timer.h"

PolyhedralLtlBüchiAutomaton::PolyhedralLtlBüchiAutomaton(const PolyhedralLtlBüchiAutomaton& other)
    : PolyhedralLtlAutomaton(other)
{
}

PolyhedralLtlBüchiAutomaton::PolyhedralLtlBüchiAutomaton()
{
}

PolyhedralLtlBüchiAutomaton::PolyhedralLtlBüchiAutomaton(
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

PolyhedralLtlBüchiAutomaton::PolyhedralLtlBüchiAutomaton(
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

PolyhedralLtlBüchiAutomatonConstSharedPtr PolyhedralLtlBüchiAutomaton::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr finiteLtlAutomaton {
        std::make_shared<PolyhedralLtlBüchiAutomaton>(
            PolyhedralLtlBüchiAutomaton {
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
    finiteLtlAutomaton->purgeUnreachableStates(formulaTgba);
    finiteLtlAutomaton->PolyhedralLtlAutomaton::buildAutomaton(formulaTgba);
    finiteLtlAutomaton->onConstructionCompleted(timer.elapsedInSeconds());

    return finiteLtlAutomaton;
}
