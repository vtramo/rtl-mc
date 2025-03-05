#include <spot/twaalgos/translate.hh>

#include "LtlAutomaton.h"
#include "logger.h"
#include "spot_utils.h"
#include "Timer.h"

LtlAutomaton::LtlAutomaton()
{
}

LtlAutomaton::LtlAutomaton(
    const std::string_view name,
    const DiscreteLtlFormula& discreteLtlFormula
)
    : Automaton(name)
    , m_discreteLtlFormula { discreteLtlFormula }
{
}

LtlAutomaton::LtlAutomaton(
    const std::string_view name,
    DiscreteLtlFormula&& discreteLtlFormula
)
    : Automaton(name)
    , m_discreteLtlFormula { std::move(discreteLtlFormula) }
{
}

LtlAutomaton::LtlAutomaton(
    const std::string_view name,
    spot::twa_graph_ptr automaton,
    const DiscreteLtlFormula& discreteLtlFormula
)
    : Automaton(name, automaton)
    , m_discreteLtlFormula { discreteLtlFormula }
{
}

LtlAutomaton::~LtlAutomaton()
{
}


spot::postprocessor::optimization_level LtlAutomaton::optimizationLevel() const
{
    return m_optimizationLevel;
}

const DiscreteLtlFormula& LtlAutomaton::formula() const
{
    return m_discreteLtlFormula;
}

spot::twa_graph_ptr LtlAutomaton::translateDiscreteLtlFormulaIntoTgba(const bool anyOption)
{
    std::string optimizationLevel { SpotUtils::toOptimizationLevelString(m_optimizationLevel) };
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Translation of the discretised LTL formula into a TGBA automaton started.", m_name);
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Optimization level: {}.", m_name, optimizationLevel);

    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::TGBA);
    if (anyOption) ltlToNbaTranslator.set_pref(spot::postprocessor::Any);
    else ltlToNbaTranslator.set_pref(spot::postprocessor::Small);
    ltlToNbaTranslator.set_level(m_optimizationLevel);

    Timer timer {};

    spot::twa_graph_ptr formulaTgba { ltlToNbaTranslator.run(m_discreteLtlFormula.formula()) };

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Translation of the discretised LTL formula into a TGBA completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA states: {}.", m_name, formulaTgba->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA edges {}.", m_name, formulaTgba->num_edges());
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA accepting sets {}.", m_name, formulaTgba->num_sets());
    m_translationStats =
      AutomatonStats::TranslationFormulaIntoTgbaStats {
          optimizationLevel,
          executionTimeSeconds,
          static_cast<int>(formulaTgba->num_states()),
          static_cast<int>(formulaTgba->num_edges()),
          static_cast<int>(formulaTgba->num_sets())
      };
    return formulaTgba;
}