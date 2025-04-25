#include "PolyhedralLtlFiniteAutomaton.h"
#include "logger.h"
#include "Timer.h"
#include "spot_utils.h"
#include <spdlog/fmt/ranges.h>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/stats.hh>
#include <spot/twaalgos/sccinfo.hh>

PolyhedralLtlFiniteAutomaton::PolyhedralLtlFiniteAutomaton()
{
    PolyhedralLtlFiniteAutomaton::initialiseStats();
}

PolyhedralLtlFiniteAutomaton::PolyhedralLtlFiniteAutomaton(const PolyhedralLtlFiniteAutomaton& other)
    : PolyhedralLtlAutomaton(other)
    , m_polyhedralFiniteLtlAutomatonStats { std::make_shared<PolyhedralLtlFiniteAutomatonStats>(*other.m_polyhedralFiniteLtlAutomatonStats) }
{
    m_polyhedralLtlAutomatonStats = m_polyhedralFiniteLtlAutomatonStats;
    m_automatonStats = m_polyhedralFiniteLtlAutomatonStats;
}

PolyhedralLtlFiniteAutomaton::PolyhedralLtlFiniteAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralLtlFiniteAutomaton(
        DiscreteLtlFormula { discreteLtlFormula },
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
    PolyhedralLtlFiniteAutomaton::initialiseStats();
}

PolyhedralLtlFiniteAutomaton::PolyhedralLtlFiniteAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralLtlAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
    PolyhedralLtlFiniteAutomaton::initialiseStats();
}

PolyhedralLtlFiniteAutomatonConstSharedPtr PolyhedralLtlFiniteAutomaton::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr finiteLtlAutomaton {
        std::make_shared<PolyhedralLtlFiniteAutomaton>(
            PolyhedralLtlFiniteAutomaton {
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemLabelDenotationMap),
                name
            }
        )
    };

    Timer timer {};
    finiteLtlAutomaton->m_optimizationLevel = optimizationLevel;
    spot::twa_graph_ptr formulaTgba { finiteLtlAutomaton->translateDiscreteLtlFormulaIntoTgba(anyOption) };
    spot::twa_graph_ptr nfa { finiteLtlAutomaton->convertToNfa(formulaTgba) };
    finiteLtlAutomaton->eraseInitialEdgesWithEmptyDenotation(nfa);
    finiteLtlAutomaton->purgeUnreachableStates(nfa);
    finiteLtlAutomaton->PolyhedralLtlAutomaton::buildAutomaton(nfa);
    finiteLtlAutomaton->onConstructionCompleted(timer.elapsedInSeconds());

    return finiteLtlAutomaton;
}

spot::twa_graph_ptr PolyhedralLtlFiniteAutomaton::convertToNfa(spot::twa_graph_ptr tgba)
{
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Conversion from TGBA to NFA started.", m_name);
    Timer timer {};

    spot::twa_graph_ptr nfa { spot::to_finite(tgba) };
    Log::logAutomaton(nfa, fmt::format("{}-nfa", m_name));

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Conversion from TGBA to NFA completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Total NFA states: {}.", m_name, nfa->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Total NFA edges {}.", m_name, nfa->num_edges());
    m_polyhedralFiniteLtlAutomatonStats->setNfaTotalStates(nfa->num_states());
    m_polyhedralFiniteLtlAutomatonStats->setNfaTotalInitialStates(1);
    m_polyhedralFiniteLtlAutomatonStats->setNfaTotalAcceptingStates(SpotUtils::collectAcceptingStates(nfa).size());
    m_polyhedralFiniteLtlAutomatonStats->setNfaTotalEdges(nfa->num_edges());
    m_polyhedralFiniteLtlAutomatonStats->setNfaSccInfo(spot::scc_info { nfa });
    m_polyhedralFiniteLtlAutomatonStats->setNfaExecutionTimeSeconds(executionTimeSeconds);

    return nfa;
}

bool PolyhedralLtlFiniteAutomaton::isAcceptingEdge(const spot::const_twa_graph_ptr& nfa, const Edge& edge)
{
    return nfa->state_is_accepting(edge.dst);
}

void PolyhedralLtlFiniteAutomaton::onConstructionCompleted(const double executionTimeSeconds)
{
    PolyhedralLtlAutomaton::onConstructionCompleted(executionTimeSeconds);
    m_polyhedralFiniteLtlAutomatonStats->setNfaMaxRecursiveDepth(1 + 2 * m_polyhedralFiniteLtlAutomatonStats->getTotalNumberPatches());
}

void PolyhedralLtlFiniteAutomaton::initialiseAutomaton()
{
    PolyhedralLtlAutomaton::initialiseAutomaton();
    m_automaton->set_acceptance(spot::acc_cond::inf({0}));
}

void PolyhedralLtlFiniteAutomaton::initialiseStats()
{
    m_polyhedralFiniteLtlAutomatonStats = std::make_shared<PolyhedralLtlFiniteAutomatonStats>();
    m_polyhedralLtlAutomatonStats = m_polyhedralFiniteLtlAutomatonStats;
    m_automatonStats = m_polyhedralFiniteLtlAutomatonStats;
}

const PolyhedralLtlFiniteAutomatonStats& PolyhedralLtlFiniteAutomaton::stats() const
{
    return *m_polyhedralFiniteLtlAutomatonStats;
}
