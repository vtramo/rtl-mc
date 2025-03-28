#include <spot/twaalgos/translate.hh>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/bundled/format-inl.h>
#include <spdlog/fmt/bundled/ranges.h>
#include <spot/twa/formula2bdd.hh>

#include "PolyhedralLtlAutomaton.h"
#include "logger.h"
#include "spot_utils.h"
#include "Timer.h"

PolyhedralLtlAutomaton::PolyhedralLtlAutomaton()
{
    PolyhedralLtlAutomaton::initializeStats();
}

PolyhedralLtlAutomaton::PolyhedralLtlAutomaton(const PolyhedralLtlAutomaton& other)
    : Automaton(other)
    , m_initialStates { other.m_initialStates }
    , m_acceptingStates { other.m_acceptingStates }
    , m_dummyInitialState { other.m_dummyInitialState }
    , m_dummyEdges { other.m_dummyEdges }
    , m_stateDenotationById { other.m_stateDenotationById }
    , m_formulaDenotationMap { other.m_formulaDenotationMap }
    , m_polyhedralLtlAutomatonStats { std::make_shared<PolyhedralLtlAutomatonStats>(*other.m_polyhedralLtlAutomatonStats) }
    , m_discreteLtlFormula { other.m_discreteLtlFormula }
    , m_optimizationLevel { other.m_optimizationLevel }
{
    m_automatonStats = m_polyhedralLtlAutomatonStats;
}

PolyhedralLtlAutomaton::PolyhedralLtlAutomaton(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
)   : Automaton(name)
    , m_formulaDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
    , m_discreteLtlFormula { std::move(discreteLtlFormula) }
{
    if (m_discreteLtlFormula.formula() == nullptr)
    {
        throw std::invalid_argument("The provided DiscreteLtlFormula contains a null formula. A valid formula must be provided.");
    }

    PolyhedralLtlAutomaton::initializeStats();
}

PolyhedralLtlAutomaton::PolyhedralLtlAutomaton(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const std::string_view name
)   : Automaton(name)
    , m_formulaDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
    , m_discreteLtlFormula { std::move(discreteLtlFormula) }
{
    if (m_discreteLtlFormula.formula() == nullptr)
    {
        throw std::invalid_argument("The provided DiscreteLtlFormula contains a null formula. A valid formula must be provided.");
    }

    PolyhedralLtlAutomaton::initializeStats();
}

void PolyhedralLtlAutomaton::initializeAutomaton()
{
    const PolyhedralSystem& polyhedralSystem { m_formulaDenotationMap.getPolyhedralSystem() };
    m_automaton = std::make_shared<spot::twa_graph>(polyhedralSystem.bddDict());
    m_automaton->prop_state_acc(spot::trival { true });
    m_automaton->set_acceptance(spot::acc_cond {spot::acc_cond::acc_code::buchi()});
    assert(m_automaton->prop_state_acc().is_true());
}

void PolyhedralLtlAutomaton::initializeStats()
{
    m_polyhedralLtlAutomatonStats = std::make_shared<PolyhedralLtlAutomatonStats>();
    m_automatonStats = m_polyhedralLtlAutomatonStats;
}

PolyhedralLtlAutomaton::~PolyhedralLtlAutomaton()
{
}

spot::postprocessor::optimization_level PolyhedralLtlAutomaton::optimizationLevel() const
{
    return m_optimizationLevel;
}

const DiscreteLtlFormula& PolyhedralLtlAutomaton::formula() const
{
    return m_discreteLtlFormula;
}

unsigned PolyhedralLtlAutomaton::totalInitialStates() const
{
    return m_initialStates.size();
}

unsigned PolyhedralLtlAutomaton::totalEdges() const
{
    const unsigned numEdges { m_automaton->num_edges() };
    if (numEdges == 0) return 0;
    return m_automaton->num_edges() - m_dummyEdges;
}

unsigned PolyhedralLtlAutomaton::totalAcceptingStates() const
{
    return m_acceptingStates.size();
}

unsigned PolyhedralLtlAutomaton::totalStates() const
{
    static constexpr unsigned DUMMY_INITIAL_STATE { 1 };
    return m_automaton->num_states() - DUMMY_INITIAL_STATE;
}

unsigned PolyhedralLtlAutomaton::isInitialState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_initialStates.count(state) == 1;
}

bool PolyhedralLtlAutomaton::isAcceptingState(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_acceptingStates.count(state) == 1;
}

const std::unordered_set<unsigned>& PolyhedralLtlAutomaton::initialStates() const
{
    return m_initialStates;
}

const std::unordered_set<unsigned>& PolyhedralLtlAutomaton::acceptingStates() const
{
    return m_acceptingStates;
}

int PolyhedralLtlAutomaton::countSuccessors(const unsigned state) const
{
    const EdgeIterator& edgeIterator { m_automaton->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

const StateDenotation& PolyhedralLtlAutomaton::stateDenotation(const unsigned state) const
{
    assertThatStateIsInRange(state);

    return m_stateDenotationById.at(state);
}

const PolyhedralLtlAutomatonStats& PolyhedralLtlAutomaton::stats() const
{
    return *m_polyhedralLtlAutomatonStats;
}

void PolyhedralLtlAutomaton::buildAutomaton(
    const spot::const_twa_graph_ptr& twaGraph,
    const std::unordered_set<unsigned>& acceptingStates
)
{
    Log::log(Verbosity::verbose, "[{} - Construction] Construction started.", m_name);

    initializeAutomaton();

    int totalPatches {};
    std::unordered_map<unsigned, std::vector<unsigned>> outEdgeStates {};
    std::unordered_map<unsigned, std::vector<unsigned>> inEdgeStates {};
    for (unsigned nfaState { 0 }; nfaState < twaGraph->num_states(); ++nfaState)
    {
        const bool isInitial { twaGraph->get_init_state_number() == nfaState };

        for (const auto& nfaEdge: twaGraph->out(nfaState))
        {
            if (nfaEdge.cond == bdd_false()) continue;

            unsigned nfaEdgeDst { nfaEdge.dst };
            const bool isAccepting { acceptingStates.count(nfaEdgeDst) == 1 };

            StateDenotation stateDenotation { extractStateDenotationFromEdgeGuard(twaGraph, nfaEdge.cond) };
            if (stateDenotation.isEmpty()) continue;
            updatePatchStats(stateDenotation.totalPatches());
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

    createDummyInitialStateWithEdgesToInitialStates();
    purgeUnreachableStates();
}

void PolyhedralLtlAutomaton::onConstructionCompleted(const double executionTimeSeconds)
{
    logConstructionCompleted(executionTimeSeconds);
    setAutomatonStats(executionTimeSeconds);
}

StateDenotation PolyhedralLtlAutomaton::extractStateDenotationFromEdgeGuard(
    const spot::const_twa_graph_ptr& twaGraph,
    const bdd& guard
)
{
    spot::formula formulaPossiblyWithSing { spot::bdd_to_formula(guard, twaGraph->get_dict()) };
    auto [formulaWithoutSing, containsSing] { removeSing(spot::formula { formulaPossiblyWithSing }) };
    const PowersetConstSharedPtr powerset { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
    return StateDenotation { std::move(formulaPossiblyWithSing), powerset, containsSing };
}

bdd PolyhedralLtlAutomaton::stateLabelsAsBdd(const unsigned state) const
{
    const StateDenotation& outStateDenotation { m_stateDenotationById.at(state) };
    spot::atomic_prop_set labels { outStateDenotation.labels() };
    return { spot::formula_to_bdd(andFormulae(labels), m_automaton->get_dict(), m_automaton) };
}

void PolyhedralLtlAutomaton::eraseInitialEdgesWithEmptyDenotation(const spot::twa_graph_ptr twaGraph)
{
    unsigned initialState { twaGraph->get_init_state_number() };
    auto outIteraser { twaGraph->out_iteraser(initialState) };
    while (outIteraser)
    {
        spot::formula formula { spot::bdd_to_formula(outIteraser->cond, twaGraph->get_dict()) };
        if (formula.is_ff())
        {
            ++outIteraser;
            continue;
        }

        const auto& [formulaWithoutSing, _] { removeSing(std::move(formula)) };
        PowersetConstSharedPtr denotation { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
        if (denotation->is_empty()) outIteraser.erase();
        ++outIteraser;
    }
}

void PolyhedralLtlAutomaton::createNewEdge(const unsigned srcState, const unsigned dstState)
{
    const bool isSrcAccepting { m_acceptingStates.count(srcState) == 1 };
    bdd labels { stateLabelsAsBdd(dstState) };
    m_automaton->new_acc_edge(srcState, dstState, labels, isSrcAccepting);
    const bool isDstAccepting { m_acceptingStates.count(dstState) == 1 };
    if (isDstAccepting && !m_automaton->state_is_accepting(dstState))
    {
        m_automaton->new_acc_edge(dstState, dstState, bdd_false(), true);
        ++m_dummyEdges;
        assert(m_automaton->state_is_accepting(dstState));
    }
}

void PolyhedralLtlAutomaton::purgeUnreachableStatesThenRenumberAcceptingStates(
    const spot::twa_graph_ptr twaGraph,
    std::unordered_set<unsigned>& acceptingStates
)
{
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states] Removal of unreachable states from automaton started.", m_name);
    Timer timer {};

    spot::twa_graph::shift_action renumberNfaAcceptingStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &acceptingStates };
    twaGraph->purge_unreachable_states(&renumberNfaAcceptingStates, &renumberingContext);

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states] Removal of unreachable states from automaton completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states] Total states: {}.", m_name, twaGraph->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states] Total edges: {}.", m_name, twaGraph->num_edges());
    Log::log(Verbosity::veryVerbose, "[{} - Purge unreachable states] Accepting states: [{}].", m_name, fmt::join(acceptingStates, ", "));
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonExecutionTimeSeconds(executionTimeSeconds);
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonTotalStates(twaGraph->num_states());
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonTotalAcceptingStates(acceptingStates.size());
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonTotalEdges(twaGraph->num_edges());
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonTotalInitialStates(1);
    m_polyhedralLtlAutomatonStats->setOptimizedAutomatonSccInfo(spot::scc_info { twaGraph });
}

void PolyhedralLtlAutomaton::logConstructionCompleted(double executionTimeSeconds)
{
    Log::log(Verbosity::verbose, "[{} - Construction] Construction completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::verbose, "[{} - Construction] Total states: {}.", m_name, totalStates());
    Log::log(Verbosity::verbose, "[{} - Construction] Total initial states: {}.", m_name, totalInitialStates());
    Log::log(Verbosity::verbose, "[{} - Construction] Total accepting states: {}.", m_name, totalAcceptingStates());
    Log::log(Verbosity::verbose, "[{} - Construction] Total edges: {}.", m_name, totalEdges());
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Initial states: [{}].", m_name, fmt::join(m_initialStates, ", "));
    Log::log(Verbosity::veryVerbose, "[{} - Construction] Accepting states: [{}].", m_name, fmt::join(m_acceptingStates, ", "));
}

void PolyhedralLtlAutomaton::purgeUnreachableStates()
{
    spot::twa_graph::shift_action renumberNfaAcceptingStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_initialStates, &m_acceptingStates, &m_stateDenotationById, &m_dummyInitialState };
    m_automaton->purge_unreachable_states(&renumberNfaAcceptingStates, &renumberingContext);
}

void PolyhedralLtlAutomaton::createDummyInitialStateWithEdgesToInitialStates()
{
    m_dummyInitialState = m_automaton->new_state();
    m_automaton->set_init_state(m_dummyInitialState);
    for (const unsigned initialState: m_initialStates)
    {
        m_automaton->new_edge(m_dummyInitialState, initialState, stateLabelsAsBdd(initialState));
        m_dummyEdges++;
    }
}

void PolyhedralLtlAutomaton::updatePatchStats(const int totPatches)
{
    m_polyhedralLtlAutomatonStats->setMaxNumberPatches(
        std::max(
            m_polyhedralLtlAutomatonStats->getMaxNumberPatches(),
            totPatches
        ));
    m_polyhedralLtlAutomatonStats->setTotalNumberPatches(m_polyhedralLtlAutomatonStats->getTotalNumberPatches() + totPatches);
}

std::unordered_set<unsigned> PolyhedralLtlAutomaton::killAcceptingStates(const spot::twa_graph_ptr& graph)
{
    Log::log(Verbosity::veryVerbose, "[{} - Kill Accepting States] Removal of outgoing edges from accepting states started.", m_name);
    std::unordered_set<unsigned> acceptingStates {};
    for (unsigned nfaState { 0 }; nfaState < graph->num_states(); ++nfaState)
    {
        if (graph->state_is_accepting(nfaState))
        {
            acceptingStates.insert(nfaState);
            graph->kill_state(nfaState);
            graph->new_acc_edge(nfaState, nfaState, bdd_false(), true);
        }
    }
    Log::log(Verbosity::veryVerbose, "[{} - Kill Accepting States] Accepting states: [{}].", m_name, fmt::join(acceptingStates, ", "));
    return acceptingStates;
}

void PolyhedralLtlAutomaton::renumberOrRemoveStatesAfterPurge(
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

spot::twa_graph_ptr PolyhedralLtlAutomaton::translateDiscreteLtlFormulaIntoTgba(const bool anyOption)
{
    std::string optimizationLevel {SpotUtils::toOptimizationLevelString(m_optimizationLevel) };
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Translation of the discretised LTL formula into a TGBA automaton started.", m_name);
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Optimization level: {}.", m_name, optimizationLevel);

    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::TGBA);
    if (anyOption) ltlToNbaTranslator.set_pref(spot::postprocessor::Any | spot::postprocessor::SBAcc | spot::postprocessor::Small);
    else ltlToNbaTranslator.set_pref(spot::postprocessor::SBAcc | spot::postprocessor::Small);
    ltlToNbaTranslator.set_level(m_optimizationLevel);

    Timer timer {};

    spot::twa_graph_ptr formulaTgba { ltlToNbaTranslator.run(m_discreteLtlFormula.formula()) };

    const double executionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Translation of the discretised LTL formula into a TGBA completed. Elapsed time: {} s.", m_name, executionTimeSeconds);
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA states: {}.", m_name, formulaTgba->num_states());
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA edges {}.", m_name, formulaTgba->num_edges());
    Log::log(Verbosity::veryVerbose, "[{} - Translation] Total TGBA accepting sets {}.", m_name, formulaTgba->num_sets());
    m_polyhedralLtlAutomatonStats->setTranslationOptimizationLevel(optimizationLevel);
    m_polyhedralLtlAutomatonStats->setTranslationExecutionTimeSeconds(executionTimeSeconds);
    m_polyhedralLtlAutomatonStats->setTranslationTotalStates(formulaTgba->num_states());
    m_polyhedralLtlAutomatonStats->setTranslationTotalEdges(formulaTgba->num_edges());
    m_polyhedralLtlAutomatonStats->setTranslationTotalAcceptingSets(formulaTgba->num_sets());
    m_polyhedralLtlAutomatonStats->setTranslationTotalInitialStates(1);
    m_polyhedralLtlAutomatonStats->setTranslationSccInfo(spot::scc_info { formulaTgba });
    m_polyhedralLtlAutomatonStats->setTranslationTotalAcceptingStates(SpotUtils::collectAcceptingStates(formulaTgba).size());

    return formulaTgba;
}

PPL::dimension_type PolyhedralLtlAutomaton::spaceDimension() const
{
    const auto& polyhedralSystem { m_formulaDenotationMap.getPolyhedralSystem() };
    return polyhedralSystem.spaceDimension();
}

std::ostream& operator<< (std::ostream& out, const PolyhedralLtlAutomaton& automaton)
{
    const unsigned totalStates { automaton.totalStates() };

    out << "Optimization level: " << SpotUtils::toOptimizationLevelString(automaton.optimizationLevel()) << '\n';
    out << "Total states: " << totalStates << '\n';
    out << "Total initial states: " << automaton.totalInitialStates() << '\n';
    out << "Total accepting states: " << automaton.totalAcceptingStates() << '\n';
    out << "Total edges: " << automaton.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << automaton.formula() << '\n';

    bool first = true;
    out << "Accepting states: [";
    for (const unsigned acceptingState: automaton.acceptingStates())
    {
        out << (first ? "" : ", ") << acceptingState;
        first = false;
    }
    out << "]\n\n";

    for (unsigned state { 0 }; state < totalStates; ++state)
    {
        const StateDenotation& stateDenotation { automaton.stateDenotation(state) };

        out << "State " << state << '\n';
        const PolyhedralSystem& polyhedralSystem { automaton.m_formulaDenotationMap.getPolyhedralSystem() };
        stateDenotation.print(out, polyhedralSystem.symbolTable());
        out << std::boolalpha;
        out << "\nIs initial state: " << automaton.isInitialState(state) << '\n';
        out << "Is accepting state: " << automaton.isAcceptingState(state) << '\n';
        out << std::noboolalpha;

        out << "Successors: [";
        first = true;
        for (const auto& edge: automaton.successors(state))
        {
            out << (first ? "" : ", ") << edge.dst;
            first = false;
        }
        out << "]\n\n";
    }

    return out << std::noboolalpha;
}
