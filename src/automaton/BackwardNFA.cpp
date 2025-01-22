#include "BackwardNFA.h"
#include "spot_utils.h"

#include <ppl_output.h>
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
    spot::translator ltlToNbaTranslator {};

    ltlToNbaTranslator.set_type(spot::postprocessor::TGBA);
    if (anyOption) ltlToNbaTranslator.set_pref(spot::postprocessor::Any);
    else ltlToNbaTranslator.set_pref(spot::postprocessor::Small);
    ltlToNbaTranslator.set_level(m_optimizationLevel);

    spot::twa_graph_ptr nfa { spot::to_finite(ltlToNbaTranslator.run(m_discreteLtlFormula.formula())) };
    buildAutomaton(nfa);
}

spot::postprocessor::optimization_level BackwardNFA::optimizationLevel() const
{
    return m_optimizationLevel;
}

void BackwardNFA::buildAutomaton(const spot::const_twa_graph_ptr& nfa)
{
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
            const bdd nfaEdgeCond { nfaEdge.cond }; // If an accepting state has no outgoing edges, Spot creates a dummy edge labeled F. We don't need this edge!
            if (nfaEdgeCond == bdd_false())
                continue;

            int nfaEdgeDst { static_cast<int>(nfaEdge.dst) };
            StateDenotation stateDenotation { extractStateDenotationFromEdgeGuard(nfa, nfaEdgeCond) };
            int edgeState { static_cast<int>(m_backwardNfa->new_state()) };
            m_stateDenotationById.emplace(edgeState, std::move(stateDenotation));

            if (isInitial)
                m_initialStates.insert(edgeState);
            else
                outEdgeStates[nfaState].push_back(edgeState);

            inEdgeStates[nfaEdgeDst].push_back(edgeState);

            if (nfa->state_is_accepting(nfaEdgeDst))
                m_finalStates.insert(edgeState);

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
}

StateDenotation BackwardNFA::extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formula { spot::bdd_to_formula(guard, nfa->get_dict()) }; // we can also use bdd_to_cnf_formula ...
    auto [formulaWithoutSing, containsSing] { SpotUtils::removeSing(std::move(formula)) };
    const PowersetConstSharedPtr powerset { m_formulaDenotationMap.getOrComputeDenotation(formulaWithoutSing) };
    return StateDenotation { std::move(formulaWithoutSing), powerset, containsSing };
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

    auto edgeStorages = m_backwardNfa->out(state);
    return edgeStorages.begin() != edgeStorages.end();
}

int BackwardNFA::totalStates() const
{
    return static_cast<int>(m_backwardNfa->num_states());
}

int BackwardNFA::totalFinalStates() const
{
    return static_cast<int>(m_finalStates.size());
}

BackwardNFA::EdgeIterator BackwardNFA::predecessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_backwardNfa->out(state);
}

int BackwardNFA::totalEdges() const
{
    return static_cast<int>(m_backwardNfa->num_edges());
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

    out << "BACKWARD NFA\n";
    out << "Optimization level: " << SpotUtils::toOptimizationLevelString(backwardNfa.optimizationLevel()) << '\n';
    out << "Total states: " << totalStates << '\n';
    out << "Total initial states: " << backwardNfa.totalInitialStates() << '\n';
    out << "Total final states: " << backwardNfa.totalFinalStates() << '\n';
    out << "Total edges: " << backwardNfa.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << backwardNfa.formula() << "\n\n";

    for (int state = 0; state < totalStates; ++state)
    {
        const StateDenotation& stateDenotation { backwardNfa.stateDenotation(state) };

        out << "State " << state << '\n';
        out << "Formula: " << stateDenotation.formula() << std::boolalpha << '\n';
        const PolyhedralSystem& polyhedralSystem { backwardNfa.m_formulaDenotationMap.getPolyhedralSystem() };
        out << "Denotation: " << PPLOutput::toString(*stateDenotation.denotation(), polyhedralSystem.getSymbolTable()) << '\n';
        out << "Singular state: " << stateDenotation.isSingular() << '\n';
        out << "Initial state: " << backwardNfa.isInitialState(state) << '\n';
        out << "Final state: " << backwardNfa.isFinalState(state) << '\n';

        out << "Predecessors: [";
        bool first = true;
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