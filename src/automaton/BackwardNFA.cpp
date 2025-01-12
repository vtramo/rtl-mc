#include "BackwardNFA.h"

#include <ppl_output.h>

#include "spot_utils.h"
#include <spot/tl/formula.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>
#include <spot/twaalgos/dot.hh>

using namespace SpotUtils;

BackwardNFA::BackwardNFA(const DiscreteLtlFormula& discreteLtlFormula, PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap)
    : BackwardNFA(DiscreteLtlFormula { discreteLtlFormula }, std::move(polyhedralSystemLabelDenotationMap))
{}

BackwardNFA::BackwardNFA(DiscreteLtlFormula&& discreteLtlFormula, PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap)
    : m_discreteLtlFormula { std::move(discreteLtlFormula) }
    , m_labelDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
{
    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::Buchi);
    ltlToNbaTranslator.set_pref(spot::postprocessor::Buchi | spot::postprocessor::Small);
    spot::twa_graph_ptr nfa { spot::to_finite(ltlToNbaTranslator.run(m_discreteLtlFormula.formula())) };
    buildAutomaton(nfa);
}

void BackwardNFA::buildAutomaton(const spot::twa_graph_ptr& nfa)
{
    const spot::bdd_dict_ptr backwardNfaDict { std::make_shared<spot::bdd_dict>() };
    m_backwardNfa = std::make_shared<spot::twa_graph>(backwardNfaDict);
    m_backwardNfa->prop_state_acc(spot::trival { true });
    m_backwardNfa->set_acceptance(nfa->get_acceptance());
    const spot::acc_cond::mark_t allAcceptanceSets { m_backwardNfa->acc().all_sets() };

    std::unordered_map<int, std::vector<int>> outTransitionStates {};
    std::unordered_map<int, std::vector<int>> inTransitionStates {};
    std::set<int> acceptingTransitionStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(nfa->num_states()); ++nfaState)
    {
        const bool isInitial { static_cast<int>(nfa->get_init_state_number()) == nfaState };
        for (auto& nfaEdge: nfa->out(nfaState))
        {
            std::vector stateDenotations { extractStateDenotationsFromEdgeGuard(nfa, nfaEdge.cond) };
            for (auto& stateDenotation: stateDenotations)
            {
                unsigned transitionState { m_backwardNfa->new_state() };

                m_stateDenotationById.emplace(transitionState, std::move(stateDenotation));

                if (isInitial) m_backwardNfa->set_init_state(transitionState);
                if (!isInitial) outTransitionStates[nfaState].push_back(transitionState);

                inTransitionStates[nfaEdge.dst].push_back(transitionState);

                if (allAcceptanceSets == nfaEdge.acc)
                    acceptingTransitionStates.insert(transitionState);

                for (int outTransitionState: outTransitionStates[nfaEdge.dst])
                {
                    const bool isOutAccepting { acceptingTransitionStates.count(outTransitionState) == 1 };
                    m_backwardNfa->new_acc_edge(outTransitionState, transitionState, bdd_true(), isOutAccepting);
                }
            }
        }

        for (int inTransitionState: inTransitionStates[nfaState])
        {
            for (int outTransitionState: outTransitionStates[nfaState])
            {
                const bool isOutAccepting { acceptingTransitionStates.count(outTransitionState) == 1 };
                m_backwardNfa->new_acc_edge(outTransitionState, inTransitionState, bdd_true(), isOutAccepting);
            }
        }
    }
}

std::vector<StateDenotation> BackwardNFA::extractStateDenotationsFromEdgeGuard(const spot::twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formula { spot::bdd_to_formula(guard, nfa->get_dict()) };
    minterms_of minterms { guard, nfa->ap_vars() };

    std::vector<StateDenotation> stateDenotations {};
    for (const bdd& minterm: minterms)
    {
        spot::atomic_prop_set atoms {};
        spot::formula mintermFormula { spot::bdd_to_formula(minterm, nfa->get_dict()) };
        for (const spot::formula& label: collectPositiveLiterals(std::move(mintermFormula)))
        {
            atoms.insert(std::move(label));
        }

        AtomSet stateLabels { std::move(atoms) };
        PowersetUniquePtr powerset { m_labelDenotationMap.getDenotation(stateLabels) };
        stateDenotations.emplace_back(std::move(stateLabels), std::move(*powerset));
    }

    return stateDenotations;
}

bool BackwardNFA::isInitialState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return static_cast<int>(m_backwardNfa->get_init_state_number()) == state;
}

bool BackwardNFA::isFinalState(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return m_backwardNfa->state_is_accepting(state);
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
    return m_finalStates.size();
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

const std::vector<int>& BackwardNFA::finalStates() const
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

std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa)
{
    int totalStates { backwardNfa.totalStates() };

    out << "BACKWARD NFA\n";
    out << "Total states: " << totalStates << '\n';
    out << "Total edges: " << backwardNfa.totalEdges() << '\n';
    out << "Discrete LTL Formula: " << backwardNfa.formula() << "\n\n";

    for (int state = 0; state < totalStates; ++state)
    {
        const StateDenotation& stateDenotation { backwardNfa.stateDenotation(state) };

        out << "State " << state << '\n';
        out << "Labels: " << stateDenotation.labels() << std::boolalpha << '\n';
        const PolyhedralSystem& polyhedralSystem { backwardNfa.m_labelDenotationMap.getPolyhedralSystem() };
        out << "Denotation: " << PPLOutput::toString(stateDenotation.denotation(), polyhedralSystem.getSymbolTable()) << '\n';
        out << "IsSing: " << stateDenotation.isSingular() << '\n';
        out << "IsInitial: " << backwardNfa.isInitialState(state) << '\n';
        out << "IsFinal: " << backwardNfa.isFinalState(state) << '\n';

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