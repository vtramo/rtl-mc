#include "PolyhedralFiniteLtlAutomaton.h"
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

PolyhedralFiniteLtlAutomaton::PolyhedralFiniteLtlAutomaton()
{
    PolyhedralFiniteLtlAutomaton::initializeStats();
}

PolyhedralFiniteLtlAutomaton::PolyhedralFiniteLtlAutomaton(const PolyhedralFiniteLtlAutomaton& other)
    : PolyhedralLtlAutomaton(other)
    , m_polyhedralFiniteLtlAutomatonStats { std::make_shared<PolyhedralFiniteLtlAutomatonStats>(*other.m_polyhedralFiniteLtlAutomatonStats) }
{
    m_polyhedralLtlAutomatonStats = m_polyhedralFiniteLtlAutomatonStats;
    m_automatonStats = m_polyhedralFiniteLtlAutomatonStats;
}

PolyhedralFiniteLtlAutomaton::PolyhedralFiniteLtlAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralFiniteLtlAutomaton(
        DiscreteLtlFormula { discreteLtlFormula },
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
    PolyhedralFiniteLtlAutomaton::initializeStats();
}

PolyhedralFiniteLtlAutomaton::PolyhedralFiniteLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : PolyhedralLtlAutomaton(
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{
    PolyhedralFiniteLtlAutomaton::initializeStats();
}

PolyhedralFiniteLtlAutomatonConstSharedPtr PolyhedralFiniteLtlAutomaton::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr finiteLtlAutomaton {
        std::make_shared<PolyhedralFiniteLtlAutomaton>(
            PolyhedralFiniteLtlAutomaton {
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
    std::unordered_set nfaAcceptingStates { finiteLtlAutomaton->killAcceptingStates(nfa) };
    finiteLtlAutomaton->purgeUnreachableStatesThenRenumberAcceptingStates(nfa, nfaAcceptingStates);
    finiteLtlAutomaton->PolyhedralLtlAutomaton::buildAutomaton(nfa, nfaAcceptingStates);
    finiteLtlAutomaton->onConstructionCompleted(timer.elapsedInSeconds());

    return finiteLtlAutomaton;
}

spot::twa_graph_ptr PolyhedralFiniteLtlAutomaton::convertToNfa(spot::twa_graph_ptr tgba)
{
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Conversion from TGBA to NFA started.", m_name);
    Timer timer {};

    spot::twa_graph_ptr nfa { spot::to_finite(tgba) };

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

void PolyhedralFiniteLtlAutomaton::onConstructionCompleted(const double executionTimeSeconds)
{
    PolyhedralLtlAutomaton::onConstructionCompleted(executionTimeSeconds);
    m_polyhedralFiniteLtlAutomatonStats->setNfaMaxRecursiveDepth(1 + 2 * m_polyhedralFiniteLtlAutomatonStats->getTotalNumberPatches());
}

void PolyhedralFiniteLtlAutomaton::initializeAutomaton()
{
    const PolyhedralSystem& polyhedralSystem { m_formulaDenotationMap.getPolyhedralSystem() };
    m_automaton = std::make_shared<spot::twa_graph>(polyhedralSystem.bddDict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(spot::acc_cond::inf({0}));
}

void PolyhedralFiniteLtlAutomaton::initializeStats()
{
    m_polyhedralFiniteLtlAutomatonStats = std::make_shared<PolyhedralFiniteLtlAutomatonStats>();
    m_polyhedralLtlAutomatonStats = m_polyhedralFiniteLtlAutomatonStats;
    m_automatonStats = m_polyhedralFiniteLtlAutomatonStats;
}

const PolyhedralFiniteLtlAutomatonStats& PolyhedralFiniteLtlAutomaton::stats() const
{
    return *m_polyhedralFiniteLtlAutomatonStats;
}
