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

FiniteLtlAutomaton::FiniteLtlAutomaton()
{
}

FiniteLtlAutomaton::FiniteLtlAutomaton(const FiniteLtlAutomaton& other)
    : LtlAutomaton(
        other.m_name,
        std::make_shared<spot::twa_graph>(*other.m_automaton),
        other.m_discreteLtlFormula
    )
    , m_initialStates { other.m_initialStates }
    , m_acceptingStates { other.m_acceptingStates }
    , m_dummyInitialState { other.m_dummyInitialState }
    , m_dummyInitialEdges { other.m_dummyInitialEdges }
    , m_stateDenotationById { other.m_stateDenotationById }
    , m_formulaDenotationMap { other.m_formulaDenotationMap }
    , m_automatonStats { other.m_automatonStats }
{
}

FiniteLtlAutomaton::FiniteLtlAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : FiniteLtlAutomaton(
        DiscreteLtlFormula { discreteLtlFormula },
        std::move(polyhedralSystemLabelDenotationMap),
        name
    )
{}

FiniteLtlAutomaton::FiniteLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
) : LtlAutomaton(
        name,
        std::move(discreteLtlFormula)
    )
    , m_formulaDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
{
}

FiniteLtlAutomatonConstSharedPtr FiniteLtlAutomaton::buildAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption,
    const std::string_view name
)
{
    std::shared_ptr finiteLtlAutomaton {
        std::make_shared<FiniteLtlAutomaton>(
            FiniteLtlAutomaton {
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemLabelDenotationMap),
                name
            }
        )
    };

    finiteLtlAutomaton->m_optimizationLevel = optimizationLevel;
    spot::twa_graph_ptr formulaTgba { finiteLtlAutomaton->translateDiscreteLtlFormulaIntoTgba(anyOption) };
    spot::twa_graph_ptr nfa { finiteLtlAutomaton->convertToNfa(formulaTgba) };
    finiteLtlAutomaton->eraseInitialEdgesWithEmptyDenotation(nfa);
    std::unordered_set nfaAcceptingStates { finiteLtlAutomaton->killAcceptingStates(nfa) };
    finiteLtlAutomaton->purgeUnreachableStatesThenRenumberAcceptingStates(nfa, nfaAcceptingStates);
    finiteLtlAutomaton->buildAutomaton(nfa, nfaAcceptingStates);

    return finiteLtlAutomaton;
}

void FiniteLtlAutomaton::eraseInitialEdgesWithEmptyDenotation(spot::twa_graph_ptr nfa)
{
    unsigned initialState { nfa->get_init_state_number() };
    auto outIteraser { nfa->out_iteraser(initialState) };
    while (outIteraser)
    {
        spot::formula formula { spot::bdd_to_formula(outIteraser->cond, nfa->get_dict()) };
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

spot::twa_graph_ptr FiniteLtlAutomaton::translateDiscreteLtlFormulaIntoTgba(const bool anyOption)
{
    spot::twa_graph_ptr formulaTgba { LtlAutomaton::translateDiscreteLtlFormulaIntoTgba(anyOption) };
    m_automatonStats.translationFormulaIntoTgba = m_translationStats;
    return formulaTgba;
}

spot::twa_graph_ptr FiniteLtlAutomaton::convertToNfa(spot::twa_graph_ptr tgba)
{
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Conversion from TGBA to NFA started.", m_name);
    Timer timer {};

    spot::twa_graph_ptr nfa { spot::to_finite(tgba) };

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Conversion from TGBA to NFA completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Total NFA states: {}.", m_name, nfa->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - TGBA to NFA] Total NFA edges {}.", m_name, nfa->num_edges());
    m_automatonStats.translationTgbaIntoNfaStats =
        AutomatonStats::NfaStats {
            executionTimeSeconds,
            static_cast<int>(nfa->num_states()),
            static_cast<int>(nfa->num_edges()),
            static_cast<int>(nfa->num_sets())
        };

    return nfa;
}

std::unordered_set<unsigned> FiniteLtlAutomaton::killAcceptingStates(const spot::twa_graph_ptr& graph)
{
    Log::log(Verbosity::veryVerbose, "[{} - Kill Accepting States] Removal of outgoing edges from accepting states started.", m_name);
    std::unordered_set<unsigned> acceptingStates {};
    for (unsigned nfaState { 0 }; nfaState < graph->num_states(); ++nfaState)
        if (graph->state_is_accepting(nfaState))
        {
            acceptingStates.insert(nfaState);
            graph->kill_state(nfaState);
            graph->new_acc_edge(nfaState, nfaState, bdd_false(), true);
        }
    Log::log(Verbosity::veryVerbose, "[{} - Kill Accepting States] Accepting states: [{}].", m_name, fmt::join(acceptingStates, ", "));
    return acceptingStates;
}

void FiniteLtlAutomaton::purgeUnreachableStatesThenRenumberAcceptingStates(
    spot::twa_graph_ptr nfa,
    std::unordered_set<unsigned>& nfaAcceptingStates
)
{
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph started.", m_name);
    Timer timer {};

    spot::twa_graph::shift_action renumberNfaAcceptingStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &nfaAcceptingStates };
    nfa->purge_unreachable_states(&renumberNfaAcceptingStates, &renumberingContext);

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states (NFA)] Total NFA states: {}.", m_name, nfa->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states (NFA)] Total NFA edges: {}.", m_name, nfa->num_edges());
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states (NFA)] Accepting NFA states: [{}].", m_name, fmt::join(nfaAcceptingStates, ", "));
    m_automatonStats.nfaOptimizations =
        AutomatonStats::NfaStats {
            executionTimeSeconds,
            static_cast<int>(nfa->num_states()),
            static_cast<int>(nfa->num_edges()),
            static_cast<int>(nfaAcceptingStates.size())
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

    const bool updateAcceptingStates { renumberingContext->m_acceptingStates != nullptr };
    const bool updateInitialStates { renumberingContext->m_initialStates != nullptr };
    const bool updateStateDenotationById { renumberingContext->m_stateDenotationById != nullptr };
    const bool updateDummyInitialState { renumberingContext->m_dummyInitialState != nullptr };

    if (updateAcceptingStates)
    {
        std::unordered_set<unsigned>* acceptingStates { renumberingContext->m_acceptingStates };
        std::unordered_set<unsigned> updatedAcceptingStates {};
        updatedAcceptingStates.reserve(acceptingStates->size());

        for (unsigned acceptingState: *acceptingStates)
            if (newst[acceptingState] != DELETED)
                updatedAcceptingStates.insert(newst[acceptingState]);

        *acceptingStates = std::move(updatedAcceptingStates);
    }

    if (updateInitialStates)
    {
        std::unordered_set<unsigned>* initialStates { renumberingContext->m_initialStates };
        std::unordered_set<unsigned> updatedInitialStates {};
        updatedInitialStates.reserve(initialStates->size());

        for (unsigned initialState: *initialStates)
            if (newst[initialState] != DELETED)
                updatedInitialStates.insert(newst[initialState]);

        *initialStates = std::move(updatedInitialStates);
    }

    if (updateStateDenotationById)
    {
        std::unordered_map<unsigned, StateDenotation> updatedStateDenotationById {};
        updatedStateDenotationById.reserve(renumberingContext->m_stateDenotationById->size());

        for (auto&& [state, stateDenotation]: *renumberingContext->m_stateDenotationById)
            if (newst[state] != DELETED)
                updatedStateDenotationById.emplace(newst[state], std::move(stateDenotation));

        *renumberingContext->m_stateDenotationById = std::move(updatedStateDenotationById);
    }

    if (updateDummyInitialState) *renumberingContext->m_dummyInitialState = newst[*renumberingContext->m_dummyInitialState];
}

void FiniteLtlAutomaton::logNfaConstruction(const double executionTimeSeconds)
{
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Construction completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Total states: {}.", m_name, totalStates());
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Total initial states: {}.", m_name, totalInitialStates());
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Total accepting states: {}.", m_name, totalAcceptingStates());
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Total edges: {}.", m_name, totalEdges());
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Initial states: [{}].", m_name, fmt::join(m_initialStates, ", "));
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Accepting states: [{}].", m_name, fmt::join(m_acceptingStates, ", "));
}

void FiniteLtlAutomaton::purgeUnreachableStates()
{
    createDummyInitialStateWithEdgesToInitialStates();
    spot::twa_graph::shift_action renumberNfaAcceptingStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_initialStates, &m_acceptingStates, &m_stateDenotationById, &m_dummyInitialState };
    m_automaton->purge_unreachable_states(&renumberNfaAcceptingStates, &renumberingContext);
}

bdd FiniteLtlAutomaton::stateLabelsAsBdd(const unsigned outEdgeState) const
{
    const StateDenotation& outStateDenotation { m_stateDenotationById.at(outEdgeState) };
    spot::atomic_prop_set labels { outStateDenotation.labels() };
    return { spot::formula_to_bdd(SpotUtils::andAtoms(labels), m_automaton->get_dict(), m_automaton) };
}

void FiniteLtlAutomaton::buildAutomaton(const spot::const_twa_graph_ptr& nfa, const std::unordered_set<unsigned>& nfaAcceptingStates)
{
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Construction started.", m_name);
    Timer timer {};

    initializeAutomaton();

    int totalPatches {};
    std::unordered_map<unsigned, std::vector<unsigned>> outEdgeStates {};
    std::unordered_map<unsigned, std::vector<unsigned>> inEdgeStates {};
    for (unsigned nfaState { 0 }; nfaState < nfa->num_states(); ++nfaState)
    {
        const bool isInitial { nfa->get_init_state_number() == nfaState };

        for (const auto& nfaEdge: nfa->out(nfaState))
        {
            if (nfaEdge.cond == bdd_false()) continue;

            unsigned nfaEdgeDst { nfaEdge.dst };
            const bool isAccepting { nfaAcceptingStates.count(nfaEdgeDst) == 1 };

            StateDenotation stateDenotation { extractStateDenotationFromEdgeGuard(nfa, nfaEdge.cond) };
            if (stateDenotation.isEmpty()) continue;
            updateMaxNumberOfPatchesStats(stateDenotation.totalPatches());
            totalPatches += stateDenotation.totalPatches();

            unsigned edgeState { m_automaton->new_state() };
            if (isAccepting) m_acceptingStates.insert(edgeState);
            m_stateDenotationById.emplace(edgeState, std::move(stateDenotation));

            if (isInitial) m_initialStates.insert(edgeState);
            else outEdgeStates[nfaState].push_back(edgeState);
            inEdgeStates[nfaEdgeDst].push_back(edgeState);

            for (const unsigned outEdgeState: outEdgeStates[nfaEdgeDst])
            {
                if (outEdgeState == edgeState) continue;
                createNewEdge(edgeState, outEdgeState);
            }
        }

        for (const unsigned inEdgeState: inEdgeStates[nfaState])
        {
            for (const unsigned outEdgeState: outEdgeStates[nfaState])
            {
                if (outEdgeState == inEdgeState) continue;
                createNewEdge(inEdgeState, outEdgeState);
            }
        }
    }

    assert(m_stateDenotationById.size() == m_automaton->num_states());
    purgeUnreachableStates();

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    logNfaConstruction(timer.elapsedInSeconds());
    setNfaStats(executionTimeSeconds, totalPatches);
}

void FiniteLtlAutomaton::createNewEdge(const unsigned srcState, const unsigned dstState)
{
    const bool isSrcAccepting { m_acceptingStates.count(srcState) == 1 };
    bdd labels { stateLabelsAsBdd(dstState) };
    m_automaton->new_acc_edge(srcState, dstState, labels, isSrcAccepting);
}

void FiniteLtlAutomaton::updateMaxNumberOfPatchesStats(const int totPatches)
{
    m_automatonStats.nfaConstructionStats.maxNumberPatches =
        std::max(
            m_automatonStats.nfaConstructionStats.maxNumberPatches,
            totPatches
        );
}

void FiniteLtlAutomaton::initializeAutomaton()
{
    const PolyhedralSystem& polyhedralSystem { m_formulaDenotationMap.getPolyhedralSystem() };
    m_automaton = std::make_shared<spot::twa_graph>(polyhedralSystem.bddDict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(spot::acc_cond::inf({0}));
}

void FiniteLtlAutomaton::setNfaStats(const double executionTimeSeconds, const int totalPatches)
{
    auto* nfaStats { &m_automatonStats.nfaConstructionStats };
    nfaStats->executionTimeSeconds = executionTimeSeconds;
    nfaStats->totalInitialStates = totalInitialStates();
    nfaStats->totalStates = totalStates();
    nfaStats->totalEdges = totalEdges();
    nfaStats->totalAcceptingStates = totalAcceptingStates();
    nfaStats->sccInfo = spot::scc_info { m_automaton };
    nfaStats->totalNumberPatches = totalPatches;
    nfaStats->maxRecursiveDepthOnTheFly = 1 + 2 * totalPatches;
}

const AutomatonStats& FiniteLtlAutomaton::stats() const
{
    return m_automatonStats;
}

StateDenotation FiniteLtlAutomaton::extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formulaPossiblyWithSing { spot::bdd_to_formula(guard, nfa->get_dict()) };
    auto [formulaWithoutSing, containsSing] { SpotUtils::removeSing(spot::formula { formulaPossiblyWithSing }) };
    const PowersetConstSharedPtr powerset { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
    return StateDenotation { std::move(formulaPossiblyWithSing), powerset, containsSing };
}

void FiniteLtlAutomaton::createDummyInitialStateWithEdgesToInitialStates()
{
    m_dummyInitialState = m_automaton->new_state();
    m_automaton->set_init_state(m_dummyInitialState);
    for (const unsigned initialState: m_initialStates)
    {
        m_automaton->new_edge(m_dummyInitialState, initialState, bdd_true());
        m_dummyInitialEdges++;
    }
}

unsigned FiniteLtlAutomaton::isInitialState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_initialStates.count(state) == 1;
}

bool FiniteLtlAutomaton::isAcceptingState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_acceptingStates.count(state) == 1;
}

unsigned FiniteLtlAutomaton::totalAcceptingStates() const
{
    return m_acceptingStates.size();
}

int FiniteLtlAutomaton::countSuccessors(const unsigned state) const
{
    const EdgeIterator& edgeIterator { m_automaton->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

const std::unordered_set<unsigned>& FiniteLtlAutomaton::acceptingStates() const
{
    return m_acceptingStates;
}

const DiscreteLtlFormula& FiniteLtlAutomaton::formula() const
{
    return m_discreteLtlFormula;
}

const StateDenotation& FiniteLtlAutomaton::stateDenotation(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_stateDenotationById.at(state);
}

unsigned FiniteLtlAutomaton::totalInitialStates() const
{
    return m_initialStates.size();
}

unsigned FiniteLtlAutomaton::totalEdges() const
{
    return m_automaton->num_edges() - m_dummyInitialEdges;
}

unsigned FiniteLtlAutomaton::totalStates() const
{
    static constexpr int DUMMY_INITIAL_STATE { 1 };
    return m_automaton->num_states() - DUMMY_INITIAL_STATE;
}

std::ostream& operator<< (std::ostream& out, const FiniteLtlAutomaton& nfa)
{
    const unsigned totalStates { nfa.totalStates() };

    out << "Optimization level: " << SpotUtils::toOptimizationLevelString(nfa.optimizationLevel()) << '\n';
    out << "Total states: " << totalStates << '\n';
    out << "Total initial states: " << nfa.totalInitialStates() << '\n';
    out << "Total accepting states: " << nfa.totalAcceptingStates() << '\n';
    out << "Total edges: " << nfa.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << nfa.formula() << '\n';

    bool first = true;
    out << "Accepting states: [";
    for (const unsigned acceptingState: nfa.acceptingStates())
    {
        out << (first ? "" : ", ") << acceptingState;
        first = false;
    }
    out << "]\n\n";

    for (unsigned state { 0 }; state < totalStates; ++state)
    {
        const StateDenotation& stateDenotation { nfa.stateDenotation(state) };

        out << "State " << state << '\n';
        const PolyhedralSystem& polyhedralSystem { nfa.m_formulaDenotationMap.getPolyhedralSystem() };
        stateDenotation.print(out, polyhedralSystem.symbolTable());
        out << std::boolalpha;
        out << "\nIs initial state: " << nfa.isInitialState(state) << '\n';
        out << "Is accepting state: " << nfa.isAcceptingState(state) << '\n';
        out << std::noboolalpha;

        out << "Successors: [";
        first = true;
        for (const auto& edge: nfa.successors(state))
        {
            out << (first ? "" : ", ") << edge.dst;
            first = false;
        }
        out << "]\n\n";
    }

    return out << std::noboolalpha;
}

const std::unordered_set<unsigned>& FiniteLtlAutomaton::initialStates() const
{
    return m_initialStates;
}