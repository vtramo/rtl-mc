#include "BackwardNFA.h"
#include "spot_utils.h"

#include <ppl_output.h>
#include <spot/tl/formula.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>
#include <spot/twaalgos/dot.hh>
#include <spot/twaalgos/split.hh>
#include <spot/twaalgos/stats.hh>

static constexpr int BUCHI_ACCEPTACE = 0;

BackwardNFA::BackwardNFA(
    const DiscreteLtlFormula& discreteLtlFormula,
    PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap
) : BackwardNFA(DiscreteLtlFormula { discreteLtlFormula }, std::move(polyhedralSystemLabelDenotationMap))
{}

BackwardNFA::BackwardNFA(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemLabelDenotationMap&& polyhedralSystemLabelDenotationMap
) : m_discreteLtlFormula { std::move(discreteLtlFormula) }
  , m_labelDenotationMap { std::move(polyhedralSystemLabelDenotationMap) }
{
    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::Buchi);
    ltlToNbaTranslator.set_pref(spot::postprocessor::Small);
    // ltlToNbaTranslator.set_pref(spot::postprocessor::Any);
    // ltlToNbaTranslator.set_level(spot::postprocessor::Low);
    spot::twa_graph_ptr nfa { spot::to_finite(ltlToNbaTranslator.run(m_discreteLtlFormula.formula())) };
    nfa = spot::split_edges(nfa);
    std::cout << "nfa states: " << nfa->num_states() << std::endl;
    std::cout << "nfa transitions: " << spot::count_all_transitions(nfa) << std::endl;
    std::cout << "nfa edges: " << nfa->num_edges() << std::endl;
    std::cerr << "exit\n";
    buildAutomatonAlreadySplitted(nfa);
}

void BackwardNFA::buildAutomatonAlreadySplitted(const spot::const_twa_graph_ptr& nfa)
{
    const spot::bdd_dict_ptr backwardNfaDict { std::make_shared<spot::bdd_dict>() };
    m_backwardNfa = std::make_shared<spot::twa_graph>(backwardNfaDict);
    m_backwardNfa->prop_state_acc(spot::trival { true });
    m_backwardNfa->set_acceptance(nfa->get_acceptance());

    std::unordered_map<int, std::vector<int>> outTransitionStates {};
    std::unordered_map<int, std::vector<int>> inTransitionStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(nfa->num_states()); ++nfaState)
    {
        const bool isInitial { static_cast<int>(nfa->get_init_state_number()) == nfaState };
        for (const auto& nfaEdge: nfa->out(nfaState))
        {
            int nfaEdgeDst { static_cast<int>(nfaEdge.dst) };

            StateDenotation stateDenotation { extractStateDenotationFromTransition(nfa, nfaEdge.cond) };
            int transitionState { static_cast<int>(m_backwardNfa->new_state()) };
            m_stateDenotationById.emplace(transitionState, std::move(stateDenotation));

            if (isInitial) m_backwardNfa->set_init_state(transitionState);
            else outTransitionStates[nfaState].push_back(transitionState);
            inTransitionStates[nfaEdgeDst].push_back(transitionState);

            if (nfaEdge.acc.has(BUCHI_ACCEPTACE))
                m_finalStates.insert(transitionState);


            for (int outTransitionState: outTransitionStates[nfaEdgeDst])
            {
                const bool isOutAccepting { m_finalStates.count(outTransitionState) == 1 };
                m_backwardNfa->new_acc_edge(outTransitionState, transitionState, bdd_true(), isOutAccepting);
            }
        }

        for (const int inTransitionState: inTransitionStates[nfaState])
        {
            for (const int outTransitionState: outTransitionStates[nfaState])
            {
                const bool isOutAccepting { m_finalStates.count(outTransitionState) == 1 };
                m_backwardNfa->new_acc_edge(outTransitionState, inTransitionState, bdd_true(), isOutAccepting);
            }
        }
    }
}

void BackwardNFA::buildAutomaton(const spot::const_twa_graph_ptr& nfa)
{
    const spot::bdd_dict_ptr backwardNfaDict { std::make_shared<spot::bdd_dict>() };
    m_backwardNfa = std::make_shared<spot::twa_graph>(backwardNfaDict);
    m_backwardNfa->prop_state_acc(spot::trival { true });
    m_backwardNfa->set_acceptance(nfa->get_acceptance());

    std::unordered_map<int, std::vector<int>> outTransitionStates {};
    std::unordered_map<int, std::vector<int>> inTransitionStates {};
    for (int nfaState { 0 }; nfaState < static_cast<int>(nfa->num_states()); ++nfaState)
    {
        const bool isInitial { static_cast<int>(nfa->get_init_state_number()) == nfaState };
        for (auto& nfaEdge: nfa->out(nfaState))
        {
            int nfaEdgeDst { static_cast<int>(nfaEdge.dst) };
            std::vector stateDenotations { extractStateDenotationsFromEdgeGuard(nfa, nfaEdge.cond) };
            for (auto& stateDenotation: stateDenotations)
            {
                int transitionState { static_cast<int>(m_backwardNfa->new_state()) };
                m_stateDenotationById.emplace(transitionState, std::move(stateDenotation));

                if (isInitial) m_backwardNfa->set_init_state(transitionState);
                else outTransitionStates[nfaState].push_back(transitionState);
                inTransitionStates[nfaEdgeDst].push_back(transitionState);

                if (nfaEdge.acc.has(BUCHI_ACCEPTACE))
                    m_finalStates.insert(transitionState);


                for (int outTransitionState: outTransitionStates[nfaEdgeDst])
                {
                    const bool isOutAccepting { m_finalStates.count(outTransitionState) == 1 };
                    m_backwardNfa->new_acc_edge(outTransitionState, transitionState, bdd_true(), isOutAccepting);
                }
            }
        }

        for (int inTransitionState: inTransitionStates[nfaState])
        {
            for (int outTransitionState: outTransitionStates[nfaState])
            {
                const bool isOutAccepting { m_finalStates.count(outTransitionState) == 1 };
                m_backwardNfa->new_acc_edge(outTransitionState, inTransitionState, bdd_true(), isOutAccepting);
            }
        }
    }
}

std::vector<StateDenotation> BackwardNFA::extractStateDenotationsFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formula { spot::bdd_to_formula(guard, nfa->get_dict()) };
    minterms_of minterms { guard, nfa->ap_vars() };

    std::vector<StateDenotation> stateDenotations {};
    for (const bdd& minterm: minterms)
    {
        bool containsSing {};
        spot::atomic_prop_set atoms {};
        spot::formula mintermFormula { spot::bdd_to_formula(minterm, nfa->get_dict()) };

        spot::atomic_prop_set positiveLiterals = SpotUtils::collectPositiveLiterals(std::move(mintermFormula));
        for (const spot::formula& label: positiveLiterals)
        {
            if (!SpotUtils::isSing(label))
                atoms.insert(label);
            else
                containsSing = true;
        }

        AtomSet stateLabels { std::move(atoms) };
        PowersetConstSharedPtr powerset { m_labelDenotationMap.getOrComputeDenotation(stateLabels) };
        stateDenotations.emplace_back(std::move(stateLabels), powerset, containsSing);
    }

    return stateDenotations;
}

StateDenotation BackwardNFA::extractStateDenotationFromTransition(const spot::const_twa_graph_ptr& nfa, const bdd& guard)
{
    spot::formula formula { spot::bdd_to_formula(guard, nfa->get_dict()) };

    bool containsSing {};
    spot::atomic_prop_set atoms {};

    const spot::atomic_prop_set positiveLiterals = SpotUtils::collectPositiveLiterals(std::move(formula));
    for (const spot::formula& label: positiveLiterals)
    {
        if (!SpotUtils::isSing(label))
            atoms.insert(label);
        else
            containsSing = true;
    }

    AtomSet stateLabels { std::move(atoms) };
    const PowersetConstSharedPtr powerset { m_labelDenotationMap.getOrComputeDenotation(stateLabels) };

    return StateDenotation { std::move(stateLabels), powerset, containsSing };
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
        out << "Denotation: " << PPLOutput::toString(*stateDenotation.denotation(), polyhedralSystem.getSymbolTable()) << '\n';
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