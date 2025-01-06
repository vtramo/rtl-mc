#include "BackwardNFA.h"
#include "spot_utils.h"
#include <spot/tl/formula.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>

using namespace SpotUtils;

BackwardNFA::BackwardNFA(spot::formula&& formula, LabelDenotationMap& labelDenotationMap)
    : m_formula { std::move(formula) }
{
    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::Buchi);
    ltlToNbaTranslator.set_pref(spot::postprocessor::SBAcc | spot::postprocessor::Small);
    m_nfa = { to_finite(ltlToNbaTranslator.run(&m_formula)) };
    buildAutomaton(labelDenotationMap);
}

void BackwardNFA::buildAutomaton(LabelDenotationMap& labelDenotationMap)
{
    unsigned totalStates { m_nfa->num_states() };
    m_states.resize(totalStates);
    m_predecessors.resize(totalStates);

    for (unsigned srcStateId { 0 }; srcStateId < totalStates; ++srcStateId)
    {
        State* srcState { &m_states[srcStateId] };
        srcState->setIndex(srcStateId);
        srcState->setIsInitial( this->isInitial(srcStateId) );
        srcState->setIsFinal( m_nfa->state_is_accepting(srcStateId) );

        bdd outgoingGuardsAnd { bdd_true() };
        for (auto& edge: m_nfa->out(srcStateId))
        {
            outgoingGuardsAnd &= edge.cond;

            const unsigned dstState { edge.dst };
            if (!this->isInitial(dstState))
            {
                m_predecessors[static_cast<int>(dstState)].push_back(srcState);
            }
        }

        spot::atomic_prop_set stateLabels { collectPositiveLiterals(bdd_to_formula(outgoingGuardsAnd, m_nfa->get_dict())) };
        AtomSet atomSet { std::move(stateLabels), true };
        srcState->setDenotation(labelDenotationMap.getDenotation(atomSet));
        srcState->setLabels(std::move(atomSet));

        if (srcState->isFinal())
        {
            m_finalStates.push_back(srcState);
        }
    }
}

bool BackwardNFA::isInitial(const unsigned state) const
{
    return m_nfa->get_init_state_number() == state;
}

bool BackwardNFA::hasPredecessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    return !m_predecessors[state].empty();
}

int BackwardNFA::totalStates() const
{
    return static_cast<int>(m_nfa->num_states());
}

int BackwardNFA::totalFinalStates() const
{
    return m_finalStates.size();
}

const std::vector<State*>& BackwardNFA::predecessors(const int state) const
{
    assert(state >= 0 && state < totalStates() && "State is out of range!");

    if (!hasPredecessors(state))
    {
        static std::vector<State*> emptyVector {};
        return emptyVector;
    }

    return m_predecessors[state];
}

int BackwardNFA::totalEdges() const
{
    return static_cast<int>(m_nfa->num_edges());
}

const std::vector<State>& BackwardNFA::states() const
{
    return m_states;
}

const std::vector<State*>& BackwardNFA::finalStates() const
{
    return m_finalStates;
}

const spot::formula& BackwardNFA::formula() const
{
    return m_formula;
}

std::ostream& operator<<(std::ostream& out, const std::vector<State>& states)
{
    out << "[";
    bool first = true;
    for (const auto& state : states)
    {
        out << (first ? "" : ", ") << state.index();
        first = false;
    }
    out << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<State*>& states)
{
    out << "[";
    bool first = true;
    for (const auto& state : states)
    {
        out << (first ? "" : ", ") << state->index();
        first = false;
    }
    out << "]";
    return out;
}


std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa)
{
    out << "BACKWARD NFA\n";
    out << "Total states: " << backwardNfa.totalStates() << '\n';
    out << "Total edges: " << backwardNfa.totalEdges() << '\n';
    out << "Discretized LTL Formula: " << backwardNfa.formula() << "\n\n";

    for (const State& state: backwardNfa.states())
    {
        out << state;
        out << "Predecessors: " << backwardNfa.predecessors(state.index()) << "\n\n";
    }

    return out;
}