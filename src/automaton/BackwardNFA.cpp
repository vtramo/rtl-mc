#include "BackwardNFA.h"

#include <spot_utils.h>

#include "discretization.h"
#include <spot/tl/formula.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/remprop.hh>

using namespace SpotUtils;
using ConstStatePtrVec = const std::vector<const State* const>;

BackwardNFA::BackwardNFA(spot::formula&& rtlf)
{
    spot::formula discretizedLtl { discretize(std::move(rtlf)) };
    spot::translator ltlToNbaTranslator {};
    ltlToNbaTranslator.set_type(spot::postprocessor::Buchi);
    ltlToNbaTranslator.set_pref(spot::postprocessor::SBAcc | spot::postprocessor::Small);
    m_nfa = { spot::to_finite(ltlToNbaTranslator.run(&discretizedLtl)) };
    buildAutomaton();
}

void BackwardNFA::buildAutomaton()
{
    unsigned totalStates { m_nfa->num_states() };
    m_states.resize(totalStates);
    m_predecessors.resize(totalStates);

    for (unsigned srcStateId { 0 }; srcStateId < totalStates; ++srcStateId)
    {
        State* srcState { &m_states[srcStateId] };
        srcState->setId(srcStateId);
        srcState->setIsInitial( this->isInitial(srcStateId) );
        srcState->setIsFinal( m_nfa->state_is_accepting(srcStateId) );

        bool isSing { false };
        spot::atomic_prop_set labels {};
        for (auto& edge: m_nfa->out(srcStateId))
        {
            const spot::atomic_prop_set extractedLabels { extractLabelsFromEdgeGuard(m_nfa, edge.cond) };
            labels.insert(extractedLabels.begin(), extractedLabels.end());

            if (!isSing)
            {
                isSing = containsSing(extractedLabels);
            }

            const unsigned dstState { edge.dst };
            if (!this->isInitial(dstState))
            {
                m_predecessors[static_cast<int>(dstState)].push_back(srcState);
            }
        }

        srcState->setIsSing(isSing);
        srcState->setLabels(std::move(labels));
        // srcState->setDenotation() TODO:

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

const std::vector<State>& BackwardNFA::states() const
{
    return m_states;
}

const std::vector<State*>& BackwardNFA::finalStates() const
{
    return m_finalStates;
}
