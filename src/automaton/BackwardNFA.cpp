#include "BackwardNFA.h"
#include "Timer.h"
#include "spot_utils.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ranges.h>
#include <spot/tl/formula.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/stats.hh>

BackwardNFA::BackwardNFA(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption
) : BackwardNFA(DiscreteLtlFormula { discreteLtlFormula }, std::move(polyhedralSystemLabelDenotationMap), optimizationLevel, anyOption)
{}

BackwardNFA::BackwardNFA(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
    const spot::postprocessor::optimization_level optimizationLevel,
    const bool anyOption
) : m_discreteLtlFormula { std::move(discreteLtlFormula) }
  , m_formulaDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
  , m_optimizationLevel { optimizationLevel }
{
    spot::twa_graph_ptr formulaTgba { translateDiscreteLtlFormulaIntoTgba(anyOption) };
    spot::twa_graph_ptr nfa { convertToNfa(formulaTgba) };
    std::unordered_set nfaFinalStates { killFinalStates(nfa) };
    purgeUnreachableStatesThenRenumberFinalStates(nfa, nfaFinalStates);
    buildAutomaton(nfa, nfaFinalStates);
}

spot::twa_graph_ptr BackwardNFA::translateDiscreteLtlFormulaIntoTgba(const bool anyOption) const
{
    spdlog::debug("[BackwardNFA - Translation] Translation of the discretized LTL formula into a TGBA automaton started.");
    spdlog::debug("[BackwardNFA - Translation] Optimization level: {}.", SpotUtils::toOptimizationLevelString(m_optimizationLevel));

    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::TGBA);
    if (anyOption) ltlToNbaTranslator.set_pref(spot::postprocessor::Any);
    else ltlToNbaTranslator.set_pref(spot::postprocessor::Small);
    ltlToNbaTranslator.set_level(m_optimizationLevel);

    Timer timer {};
    spot::twa_graph_ptr formulaTgba { ltlToNbaTranslator.run(m_discreteLtlFormula.formula()) };
    spdlog::debug("[BackwardNFA - Translation] Translation of the discretized LTL formula into a TGBA completed. Elapsed time: {} ms.", timer.elapsed());
    spdlog::debug("[BackwardNFA - Translation] Total TGBA states: {}.", formulaTgba->num_states());
    spdlog::debug("[BackwardNFA - Translation] Total TGBA edges {}.", formulaTgba->num_edges());
    spdlog::debug("[BackwardNFA - Translation] Total TGBA accepting sets {}.", formulaTgba->num_sets());
    return formulaTgba;
}

spot::twa_graph_ptr BackwardNFA::convertToNfa(spot::twa_graph_ptr tgba)
{
    spdlog::debug("[BackwardNFA - TGBA to NFA] Conversion from TGBA to NFA started.");
    Timer timer {};
    spot::twa_graph_ptr nfa { spot::to_finite(tgba) };
    spdlog::debug("[BackwardNFA - TGBA to NFA] Conversion from TGBA to NFA completed. Elapsed time: {} ms.", timer.elapsed());
    spdlog::debug("[BackwardNFA - TGBA to NFA] Total NFA states: {}.", nfa->num_states());
    spdlog::debug("[BackwardNFA - TGBA to NFA] Total NFA edges {}.", nfa->num_edges());
    return nfa;
}

std::unordered_set<int> BackwardNFA::killFinalStates(const spot::twa_graph_ptr& graph)
{
    spdlog::debug("[BackwardNFA - Kill Final States] Removal of outgoing edges from final states started.");
    std::unordered_set<int> acceptingStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(graph->num_states()); ++nfaState)
        if (graph->state_is_accepting(nfaState))
        {
            acceptingStates.insert(nfaState);
            graph->kill_state(nfaState);
        }
    spdlog::debug("[BackwardNFA - Kill Final States] Final states: [{}].", fmt::join(acceptingStates, ", "));
    return acceptingStates;
}

void BackwardNFA::purgeUnreachableStatesThenRenumberFinalStates(
    spot::twa_graph_ptr nfa,
    std::unordered_set<int>& nfaFinalStates
)
{
    spdlog::debug("[BackwardNFA - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph started.");
    spot::twa_graph::shift_action renumberNfaFinalStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &nfaFinalStates };
    Timer timer {};
    nfa->purge_unreachable_states(&renumberNfaFinalStates, &renumberingContext);
    spdlog::debug("[BackwardNFA - Purge unreachable states (NFA)] Removal of unreachable states from nfa graph completed. Elapsed time: {} ms.", timer.elapsed());
    spdlog::debug("[BackwardNFA - Purge unreachable states (NFA)] Total NFA states: {}.", nfa->num_states());
    spdlog::debug("[BackwardNFA - Purge unreachable states (NFA)] Total NFA edges: {}.", nfa->num_edges());
    spdlog::debug("[BackwardNFA - Purge unreachable states (NFA)] Final NFA states: [{}].", fmt::join(nfaFinalStates, ", "));
}

void BackwardNFA::renumberOrRemoveStatesAfterPurge(
    const std::vector<unsigned>& newst,
    const RenumberingContextVoidPtr renumberingContextVoidPtr
)
{
    static constexpr unsigned DELETED { -1U };

    assert(renumberingContextVoidPtr != nullptr && "RenumberingContextVoidPtr is null!");

    RenumberingContext* renumberingContext { static_cast<RenumberingContext*>(renumberingContextVoidPtr) };

    const bool updateFinalStates { renumberingContext->m_finalStates != nullptr };
    const bool updateInitialStates { renumberingContext->m_initialStates != nullptr };
    const bool updateStateDenotationById { renumberingContext->m_stateDenotationById != nullptr };
    const bool updateDummyInitialState { renumberingContext->m_dummyInitialState != nullptr };

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

    if (updateInitialStates)
    {
        std::unordered_set<int>* initialStates { renumberingContext->m_initialStates };
        std::unordered_set<int> updatedInitialStates {};
        updatedInitialStates.reserve(initialStates->size());

        for (unsigned initialState: *initialStates)
            if (newst[initialState] != DELETED)
                updatedInitialStates.insert(newst[initialState]);

        *initialStates = std::move(updatedInitialStates);
    }

    if (updateStateDenotationById)
    {
        std::unordered_map<int, StateDenotation> updatedStateDenotationById {};
        updatedStateDenotationById.reserve(renumberingContext->m_stateDenotationById->size());

        for (auto&& [state, stateDenotation]: *renumberingContext->m_stateDenotationById)
            if (newst[state] != DELETED)
                updatedStateDenotationById.emplace(newst[state], std::move(stateDenotation));

        *renumberingContext->m_stateDenotationById = std::move(updatedStateDenotationById);
    }

    if (updateDummyInitialState) *renumberingContext->m_dummyInitialState = newst[*renumberingContext->m_dummyInitialState];
}

void BackwardNFA::buildAutomaton(const spot::const_twa_graph_ptr& nfa, const std::unordered_set<int>& nfaAcceptingStates)
{
    spdlog::debug("[BackwardNFA - Construction] Backward NFA construction started.");
    Timer timer {};

    const spot::bdd_dict_ptr backwardNfaDict { std::make_shared<spot::bdd_dict>() };
    m_backwardNfa = std::make_shared<spot::twa_graph>(backwardNfaDict);
    m_backwardNfa->prop_state_acc(spot::trival { true });
    m_backwardNfa->set_acceptance(nfa->get_acceptance());

    std::unordered_map<int, std::vector<int>> outEdgeStates {};
    std::unordered_map<int, std::vector<int>> inEdgeStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(nfa->num_states()); ++nfaState)
    {
        const bool isInitial { static_cast<int>(nfa->get_init_state_number()) == nfaState };

        for (const auto& nfaEdge: nfa->out(nfaState))
        {
            int nfaEdgeDst { static_cast<int>(nfaEdge.dst) };
            const bool isAccepting { nfaAcceptingStates.count(nfaEdgeDst) == 1 };

            StateDenotation stateDenotation { extractStateDenotationFromEdgeGuard(nfa, nfaEdge.cond) };
            if (!isInitial && stateDenotation.isEmpty()) continue;

            int edgeState { static_cast<int>(m_backwardNfa->new_state()) };
            if (isAccepting) m_finalStates.insert(edgeState);
            m_stateDenotationById.emplace(edgeState, std::move(stateDenotation));

            if (isInitial) m_initialStates.insert(edgeState);
            else outEdgeStates[nfaState].push_back(edgeState);
            inEdgeStates[nfaEdgeDst].push_back(edgeState);

            for (int outEdgeState: outEdgeStates[nfaEdgeDst])
            {
                if (outEdgeState == edgeState) continue;
                const bool isOutAccepting { m_finalStates.count(outEdgeState) == 1 };
                m_backwardNfa->new_acc_edge(outEdgeState, edgeState, bdd_true(), isOutAccepting);
            }
        }

        for (const int inEdgeState: inEdgeStates[nfaState])
        {
            for (const int outEdgeState: outEdgeStates[nfaState])
            {
                if (outEdgeState == inEdgeState) continue;
                const bool isOutAccepting { m_finalStates.count(outEdgeState) == 1 };
                m_backwardNfa->new_acc_edge(outEdgeState, inEdgeState, bdd_true(), isOutAccepting);
            }
        }
    }

    assert(m_stateDenotationById.size() == m_backwardNfa->num_states());
    createDummyInitialStateWithEdgesToFinalStatesHavingPredecessors();
    spot::twa_graph::shift_action renumberBackwardNfaFinalStates { &renumberOrRemoveStatesAfterPurge };
    RenumberingContext renumberingContext { &m_initialStates, &m_finalStates, &m_stateDenotationById, &m_dummyInitialState };
    m_backwardNfa->purge_unreachable_states(&renumberBackwardNfaFinalStates, &renumberingContext);

    spdlog::debug("[BackwardNFA - Construction] Backward NFA construction completed. Elapsed time: {} ms.", timer.elapsed());
    spdlog::debug("[BackwardNFA - Construction] Backward NFA total states: {}.", totalStates());
    spdlog::debug("[BackwardNFA - Construction] Backward NFA total initial states: {}.", totalInitialStates());
    spdlog::debug("[BackwardNFA - Construction] Backward NFA total final states: {}.", totalFinalStates());
    spdlog::debug("[BackwardNFA - Construction] Backward NFA total edges: {}.", totalEdges());
    spdlog::debug("[BackwardNFA - Construction] Backward NFA initial states: [{}].", fmt::join(m_initialStates, ", "));
    spdlog::debug("[BackwardNFA - Construction] Backward NFA final states: [{}].", fmt::join(m_finalStates, ", "));
}

StateDenotation BackwardNFA::extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formula { spot::bdd_to_formula(guard, nfa->get_dict()) }; // we can also use bdd_to_cnf_formula ...
    auto [formulaWithoutSing, containsSing] { SpotUtils::removeSing(std::move(formula)) };
    const PowersetConstSharedPtr powerset { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
    return StateDenotation { std::move(formulaWithoutSing), powerset, containsSing };
}

void BackwardNFA::createDummyInitialStateWithEdgesToFinalStatesHavingPredecessors()
{
    m_dummyInitialState = m_backwardNfa->new_state();
    m_backwardNfa->set_init_state(m_dummyInitialState);
    for (const int finalState: m_finalStates)
    {
        if (hasPredecessors(finalState))
        {
            m_backwardNfa->new_edge(m_dummyInitialState, finalState, bdd_true());
            m_dummyInitialEdges++;
        }
    }
}

bool BackwardNFA::isInitialState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_initialStates.count(state) == 1;
}

bool BackwardNFA::isFinalState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_finalStates.count(state) == 1;
}

bool BackwardNFA::hasPredecessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    auto edgeStorages { m_backwardNfa->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

int BackwardNFA::totalStates() const
{
    static constexpr int DUMMY_INITIAL_STATE { 1 };
    return static_cast<int>(m_backwardNfa->num_states() - DUMMY_INITIAL_STATE);
}

int BackwardNFA::totalFinalStates() const
{
    return static_cast<int>(m_finalStates.size());
}

spot::postprocessor::optimization_level BackwardNFA::optimizationLevel() const
{
    return m_optimizationLevel;
}

BackwardNFA::EdgeIterator BackwardNFA::predecessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_backwardNfa->out(state);
}

int BackwardNFA::totalEdges() const
{
    return static_cast<int>(m_backwardNfa->num_edges() - m_dummyInitialEdges);
}

const std::unordered_set<int>& BackwardNFA::finalStates() const
{
    return m_finalStates;
}

const DiscreteLtlFormula& BackwardNFA::formula() const
{
    return m_discreteLtlFormula;
}

const StateDenotation& BackwardNFA::stateDenotation(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_stateDenotationById.at(state);
}

void BackwardNFA::printHoaFormat(std::ostream& os) const
{
    spot::print_hoa(os, m_backwardNfa);
}

void BackwardNFA::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_backwardNfa);
}

int BackwardNFA::totalInitialStates() const
{
    return m_initialStates.size();
}

std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa)
{
    int totalStates { backwardNfa.totalStates() };

    out << "Optimization level: " << SpotUtils::toOptimizationLevelString(backwardNfa.optimizationLevel()) << '\n';
    out << "Total states: " << totalStates << '\n';
    out << "Total initial states: " << backwardNfa.totalInitialStates() << '\n';
    out << "Total final states: " << backwardNfa.totalFinalStates() << '\n';
    out << "Total edges: " << backwardNfa.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << backwardNfa.formula() << '\n';

    bool first = true;
    out << "Final states: [";
    for (const int finalState: backwardNfa.finalStates())
    {
        out << (first ? "" : ", ") << finalState;
        first = false;
    }
    out << "]\n\n";

    for (int state = 0; state < totalStates; ++state)
    {
        const StateDenotation& stateDenotation { backwardNfa.stateDenotation(state) };

        out << "State " << state << '\n';
        const PolyhedralSystem& polyhedralSystem { backwardNfa.m_formulaDenotationMap.getPolyhedralSystem() };
        stateDenotation.print(out, polyhedralSystem.getSymbolTable());
        out << std::boolalpha;
        out << "\nInitial state: " << backwardNfa.isInitialState(state) << '\n';
        out << "Final state: " << backwardNfa.isFinalState(state) << '\n';
        out << std::noboolalpha;

        out << "Predecessors: [";
        first = true;
        for (const auto& edge: backwardNfa.predecessors(state))
        {
            out << (first ? "" : ", ") << edge.dst;
            first = false;
        }
        out << "]\n\n";
    }

    return out << std::noboolalpha;
}

const std::unordered_set<int>& BackwardNFA::initialStates() const
{
    return m_initialStates;
}