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
    ltlToNbaTranslator.set_pref(spot::postprocessor::SBAcc | spot::postprocessor::Small);
    m_backwardNfa = { spot::to_finite(ltlToNbaTranslator.run(m_discreteLtlFormula.formula())) };
    transposeNfa();
}

void BackwardNFA::transposeNfa()
{
    const unsigned totalStates { m_backwardNfa->num_states() };
    m_stateDenotationById.reserve(totalStates);

    for (unsigned srcStateId { 0 }; srcStateId < totalStates; ++srcStateId)
    {
        bdd outgoingGuardsAnd { bdd_true() };
        for (auto& nfaEdge: m_backwardNfa->out(srcStateId))
        {
            if (m_backwardNfa->get_init_state_number() != nfaEdge.dst)
            {
                unsigned nfaEdgeDst = nfaEdge.dst;
                nfaEdge.dst = srcStateId;
                nfaEdge.src = nfaEdgeDst;
            }

            outgoingGuardsAnd &= nfaEdge.cond;
        }

        spot::atomic_prop_set stateLabels { collectPositiveLiterals(spot::bdd_to_formula(outgoingGuardsAnd, m_backwardNfa->get_dict())) };
        AtomSet stateLabelsAtomSet { std::move(stateLabels) };
        PowersetUniquePtr powerset { m_labelDenotationMap.getDenotation(stateLabelsAtomSet) };
        StateDenotation stateDenotation { std::move(stateLabelsAtomSet), std::move(*powerset) };
        m_stateDenotationById.emplace(std::make_pair(srcStateId, std::move(stateDenotation)));
    }
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
            out << (first ? "" : ", ") << edge.src;
            first = false;
        }
        out << "]\n\n";
    }

    return out << std::noboolalpha;
}