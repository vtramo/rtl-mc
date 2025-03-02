#include "FiniteLtlAutomaton.h"
#include "logger.h"
#include "Timer.h"
#include "spot_utils.h"
#include <spdlog/fmt/ranges.h>
#include <spot/tl/formula.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/stats.hh>
#include <spot/twaalgos/sccinfo.hh>

FiniteLtlAutomaton::FiniteLtlAutomaton(const FiniteLtlAutomaton& other)
    : m_nfa { std::make_shared<spot::twa_graph>(*other.m_nfa) }
    , m_finalStates { other.m_finalStates }
    , m_discreteLtlFormula { other.m_discreteLtlFormula }
    , m_formulaDenotationMap { other.m_formulaDenotationMap }
    , m_optimizationLevel { other.m_optimizationLevel }
    , m_automatonStats { other.m_automatonStats }
{
}

FiniteLtlAutomaton::FiniteLtlAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption
)
    : FiniteLtlAutomaton(
        DiscreteLtlFormula { discreteLtlFormula },
        std::move(polyhedralSystemLabelDenotationMap),
        optimizationLevel,
        anyOption
    )
{}

FiniteLtlAutomaton::FiniteLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption
) : m_discreteLtlFormula { std::move(discreteLtlFormula) }
  , m_formulaDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
  , m_optimizationLevel { optimizationLevel }
{
    const auto& polyhedralSystem { m_formulaDenotationMap.getPolyhedralSystem() };
    spot::twa_graph_ptr formulaTgba { translateDiscreteLtlFormulaIntoTgba(anyOption, polyhedralSystem.bddDict()) };
    m_nfa = convertToNfa(formulaTgba);
    eraseEdgesWithEmptyDenotation();
    m_finalStates = killFinalStates();
    purgeUnreachableStatesThenRenumberFinalStates();
    logConstructionCompleted();
    const auto [totalPatches, maxPatches] { collectPatchStats() };
    setStats(totalPatches, maxPatches);
}

spot::twa_graph_ptr FiniteLtlAutomaton::translateDiscreteLtlFormulaIntoTgba(const bool anyOption, spot::bdd_dict_ptr bddDict)
{
    std::string optimizationLevel { SpotUtils::toOptimizationLevelString(m_optimizationLevel) };
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Translation of the discretised LTL formula into a TGBA automaton started.");
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Optimization level: {}.", optimizationLevel);

    spot::translator ltlToNbaTranslator { bddDict };
    ltlToNbaTranslator.set_type(spot::postprocessor::Buchi);
    if (anyOption) ltlToNbaTranslator.set_pref(spot::postprocessor::Any);
    else ltlToNbaTranslator.set_pref(spot::postprocessor::Small);
    ltlToNbaTranslator.set_level(m_optimizationLevel);

    Timer timer {};

    spot::twa_graph_ptr formulaTgba { ltlToNbaTranslator.run(m_discreteLtlFormula.formula()) };

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Translation of the discretised LTL formula into a TGBA completed. Elapsed time: {} s.", executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Total TGBA states: {}.", formulaTgba->num_states());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Total TGBA edges {}.", formulaTgba->num_edges());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Translation] Total TGBA accepting sets {}.", formulaTgba->num_sets());
    m_automatonStats.translationFormulaIntoTgba =
        AutomatonStats::TranslationFormulaIntoTgbaStats {
            optimizationLevel,
            executionTimeSeconds,
            static_cast<int>(formulaTgba->num_states()),
            static_cast<int>(formulaTgba->num_edges()),
            static_cast<int>(formulaTgba->num_sets())
        };
    return formulaTgba;
}

spot::twa_graph_ptr FiniteLtlAutomaton::convertToNfa(spot::twa_graph_ptr tgba)
{
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - TGBA to NFA] Conversion from TGBA to NFA started.");
    Timer timer {};

    spot::twa_graph_ptr nfa { spot::to_finite(tgba) };

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - TGBA to NFA] Conversion from TGBA to NFA completed. Elapsed time: {} s.", executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - TGBA to NFA] Total NFA states: {}.", nfa->num_states());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - TGBA to NFA] Total NFA edges {}.", nfa->num_edges());
    m_automatonStats.translationTgbaIntoNfaStats =
        AutomatonStats::NfaStats {
            executionTimeSeconds,
            static_cast<int>(nfa->num_states()),
            static_cast<int>(nfa->num_edges()),
            static_cast<int>(nfa->num_sets())
        };

    return nfa;
}

void FiniteLtlAutomaton::eraseEdgesWithEmptyDenotation()
{
    for (int nfaState { 0 }; nfaState < static_cast<int>(m_nfa->num_states()); ++nfaState)
    {
        auto outIteraser { m_nfa->out_iteraser(nfaState) };
        while (outIteraser)
        {
            spot::formula formula { spot::bdd_to_formula(outIteraser->cond, m_nfa->get_dict()) };
            if (formula.is_ff())
            {
                ++outIteraser;
                continue;
            }

            const auto& [formulaWithoutSing, _] { SpotUtils::removeSing(std::move(formula)) };
            PowersetConstSharedPtr denotation { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
            if (denotation->is_empty()) outIteraser.erase();
            ++outIteraser;
        }
    }
}

std::unordered_set<int> FiniteLtlAutomaton::killFinalStates()
{
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Kill Final States] Removal of outgoing edges from final states started.");
    std::unordered_set<int> acceptingStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(m_nfa->num_states()); ++nfaState)
        if (m_nfa->state_is_accepting(nfaState))
        {
            acceptingStates.insert(nfaState);
            m_nfa->kill_state(nfaState);
            m_nfa->new_acc_edge(nfaState, nfaState, bdd_false(), true);
        }
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Kill Final States] Final states: [{}].", fmt::join(acceptingStates, ", "));
    return acceptingStates;
}

void FiniteLtlAutomaton::purgeUnreachableStatesThenRenumberFinalStates(
)
{
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph started.");
    Timer timer {};

    spot::twa_graph::shift_action renumberNfaFinalStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_finalStates };
    m_nfa->purge_unreachable_states(&renumberNfaFinalStates, &renumberingContext);

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph completed. Elapsed time: {} s.", executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Purge unreachable states (NFA)] Total NFA states: {}.", m_nfa->num_states());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Purge unreachable states (NFA)] Total NFA edges: {}.", m_nfa->num_edges());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Purge unreachable states (NFA)] Final NFA states: [{}].", fmt::join(m_finalStates, ", "));
    m_automatonStats.nfaOptimizations =
        AutomatonStats::NfaStats {
            executionTimeSeconds,
            static_cast<int>(m_nfa->num_states()),
            static_cast<int>(m_nfa->num_edges()),
            static_cast<int>(m_finalStates.size())
        };
}

void FiniteLtlAutomaton::renumberOrRemoveStatesAfterPurge(
    const std::vector<unsigned>& newst,
    const RenumberingContextVoidPtr renumberingContextVoidPtr
)
{
    static constexpr unsigned DELETED { -1U };

    assert(renumberingContextVoidPtr != nullptr && "RenumberingContextVoidPtr is null!");

    RenumberingContext* renumberingContext { static_cast<RenumberingContext*>(renumberingContextVoidPtr) };

    const bool updateFinalStates { renumberingContext->m_finalStates != nullptr };
    if (updateFinalStates)
    {
        std::unordered_set<int>* finalStates { renumberingContext->m_finalStates };
        std::unordered_set<int> updatedFinalStates {};
        updatedFinalStates.reserve(finalStates->size());

        for (unsigned finalState: *finalStates)
            if (newst[finalState] != DELETED)
                updatedFinalStates.insert(newst[finalState]);

        *finalStates = std::move(updatedFinalStates);
    }
}

void FiniteLtlAutomaton::logConstructionCompleted()
{
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton construction completed.");
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton total states: {}.", totalStates());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton total initial states: 1.");
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton total final states: {}.", totalFinalStates());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton total edges: {}.", totalEdges());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton initial states: [{}].", m_nfa->get_init_state_number());
    Log::log(Verbosity::veryVerbose, "[FiniteLtlAutomaton - Construction] FiniteLtlAutomaton final states: [{}].", fmt::join(m_finalStates, ", "));
}

void FiniteLtlAutomaton::purgeUnreachableStates()
{
    spot::twa_graph::shift_action renumberBackwardNfaFinalStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_finalStates };
    m_nfa->purge_unreachable_states(&renumberBackwardNfaFinalStates, &renumberingContext);
}

void FiniteLtlAutomaton::updateMaxNumberOfPatchesStats(const int totPatches)
{
    m_automatonStats.backwardNfaConstructionStats.maxNumberPatches =
        std::max(
            m_automatonStats.backwardNfaConstructionStats.maxNumberPatches,
            totPatches
        );
}

int FiniteLtlAutomaton::maxRecursiveDepth() const
{
    return m_automatonStats.backwardNfaConstructionStats.maxRecursiveDepth;
}

void FiniteLtlAutomaton::setStats(const int totalPatches, const int maxNumberPatches)
{
    auto* backwardNfaStats { &m_automatonStats.backwardNfaConstructionStats };
    backwardNfaStats->totalInitialStates = 1;
    backwardNfaStats->totalStates = totalStates();
    backwardNfaStats->totalEdges = totalEdges();
    backwardNfaStats->totalFinalStates = totalFinalStates();
    backwardNfaStats->sccInfo = spot::scc_info { m_nfa };
    backwardNfaStats->totalNumberPatches = totalPatches;
    backwardNfaStats->maxNumberPatches = maxNumberPatches;
    backwardNfaStats->maxRecursiveDepth = 1 + 2 * totalPatches;
}

const AutomatonStats& FiniteLtlAutomaton::stats() const
{
    return m_automatonStats;
}

std::array<int, 2> FiniteLtlAutomaton::collectPatchStats()
{
    class CollectPatchesDfs: public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit CollectPatchesDfs(
            const spot::const_twa_ptr& nfa,
            PolyhedralSystemFormulaDenotationMap* const formulaDenotationMap
        )
            : twa_reachable_iterator_depth_first(nfa)
        {
            m_nfa = nfa;
            m_formulaDenotationMap = formulaDenotationMap;
        }

        void process_state(const spot::state* s, const int n, spot::twa_succ_iterator* si) override
        {
            if (!si->first()) return;

            do
            {
                bdd bddFormula { si->cond() };
                spot::formula formula { spot::bdd_to_formula(bddFormula, m_nfa->get_dict()) };
                const auto [formulaWithoutSing, _] { SpotUtils::removeSing(std::move(formula)) };
                PowersetConstSharedPtr denotation { m_formulaDenotationMap->getOrComputeDenotation(formulaWithoutSing) };
                int patches { static_cast<int>(denotation->size()) };
                m_totalPatches += patches;
                m_maxPatches = std::max(m_maxPatches, patches);
            } while (si->next());

            spot::twa_reachable_iterator_depth_first::process_state(s, n, si);
        }

        int totalPatches() const { return m_totalPatches; }
        int maxPatches() const { return m_maxPatches; }

    private:
        int m_totalPatches {};
        int m_maxPatches {};
        spot::const_twa_ptr m_nfa {};
        PolyhedralSystemFormulaDenotationMap* m_formulaDenotationMap {};
    };

    CollectPatchesDfs dfs { m_nfa, &m_formulaDenotationMap };
    dfs.run();
    return { dfs.totalPatches(), dfs.maxPatches() };
}

StateDenotation FiniteLtlAutomaton::extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formulaPossiblyWithSing { spot::bdd_to_formula(guard, nfa->get_dict()) };
    auto [formulaWithoutSing, containsSing] { SpotUtils::removeSing(spot::formula { formulaPossiblyWithSing }) };
    const PowersetConstSharedPtr powerset { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
    return StateDenotation { std::move(formulaPossiblyWithSing), powerset, containsSing };
}

spot::const_twa_graph_ptr FiniteLtlAutomaton::twa() const
{
    return m_nfa;
}

bool FiniteLtlAutomaton::hasSuccessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    auto edgeStorages { m_nfa->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

FiniteLtlAutomaton::EdgeIterator FiniteLtlAutomaton::successors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_nfa->out(state);
}

int FiniteLtlAutomaton::countSuccessors(const int state) const
{
    const EdgeIterator& edgeIterator { m_nfa->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

bool FiniteLtlAutomaton::isInitialState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return static_cast<int>(m_nfa->get_init_state_number()) == state;
}

bool FiniteLtlAutomaton::isFinalState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_finalStates.count(state) == 1;
}

int FiniteLtlAutomaton::totalStates() const
{
    return static_cast<int>(m_nfa->num_states());
}

int FiniteLtlAutomaton::totalFinalStates() const
{
    return static_cast<int>(m_finalStates.size());
}

spot::postprocessor::optimization_level FiniteLtlAutomaton::optimizationLevel() const
{
    return m_optimizationLevel;
}

int FiniteLtlAutomaton::totalEdges() const
{
    return static_cast<int>(m_nfa->num_edges());
}

const std::unordered_set<int>& FiniteLtlAutomaton::finalStates() const
{
    return m_finalStates;
}

const DiscreteLtlFormula& FiniteLtlAutomaton::formula() const
{
    return m_discreteLtlFormula;
}

void FiniteLtlAutomaton::printHoaFormat(std::ostream& os) const
{
    spot::print_hoa(os, m_nfa);
}

void FiniteLtlAutomaton::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_nfa);
}

std::ostream& operator<< (std::ostream& out, const FiniteLtlAutomaton& finiteLtlAutomaton)
{
    int totalStates { finiteLtlAutomaton.totalStates() };

    out << "Optimization level: " << SpotUtils::toOptimizationLevelString(finiteLtlAutomaton.optimizationLevel()) << '\n';
    out << "Total states: " << totalStates << '\n';
    out << "Total initial states: 1\n";
    out << "Total final states: " << finiteLtlAutomaton.totalFinalStates() << '\n';
    out << "Total edges: " << finiteLtlAutomaton.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << finiteLtlAutomaton.formula() << '\n';

    bool first = true;
    out << "Final states: [";
    for (const int finalState: finiteLtlAutomaton.finalStates())
    {
        out << (first ? "" : ", ") << finalState;
        first = false;
    }
    out << "]\n\n";

    for (int state = 0; state < totalStates; ++state)
    {
        out << "State " << state << '\n';
        out << std::boolalpha;
        out << "\nInitial state: " << finiteLtlAutomaton.isInitialState(state) << '\n';
        out << "Final state: " << finiteLtlAutomaton.isFinalState(state) << '\n';
        out << std::noboolalpha;

        out << "Successors: [";
        first = true;
        for (const auto& edge: finiteLtlAutomaton.m_nfa->out(state))
        {
            out << (first ? "" : ", ") << edge.dst;
            first = false;
        }
        out << "]\n\n";
    }

    return out << std::noboolalpha;
}

int FiniteLtlAutomaton::initialState() const
{
    return m_nfa->get_init_state_number();
}