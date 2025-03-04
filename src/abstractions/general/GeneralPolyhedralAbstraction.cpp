#include <spot/twaalgos/dot.hh>
#include <spot/twa/formula2bdd.hh>

#include "GeneralPolyhedralAbstraction.h"
#include "observable_patch_cartesian_product.h"
#include "SingOpenStatePair.h"

GeneralPolyhedralAbstraction::GeneralPolyhedralAbstraction(
    PolyhedralSystemConstSharedPtr polyhedralSystem,
    const int totalStatesFiniteAutomaton
) : m_spaceDimension { polyhedralSystem->spaceDimension() }
{
    std::vector observables { polyhedralSystem->generateObservables() };
    computeSufficientHorizon(totalStatesFiniteAutomaton, observables);
    initializeGraph(polyhedralSystem->bddDict());
    buildAbstraction(std::move(observables), polyhedralSystem->preFlow());
}

void GeneralPolyhedralAbstraction::computeSufficientHorizon(
    const int totalStatesFiniteAutomaton,
    const std::vector<Observable>& observables
)
{
    int maxPatches {};
    for (const Observable& observable: observables)
        maxPatches = std::max(maxPatches, observable.totalPatches());
    m_sufficientHorizon = 2 * totalStatesFiniteAutomaton * maxPatches;
}

void GeneralPolyhedralAbstraction::buildAbstraction(std::vector<Observable>&& observables, const Poly& preFlow)
{
    auto compareObservableByAtoms {
        [](const Observable& observable1, const Observable& observable2)
        { return observable1.atoms() < observable2.atoms(); }
    };

    std::unordered_map<int, std::unordered_map<ObservablePatchSequence, SingOpenStatePair>> singOpenStatePairBySequence {};
    for (int k { 1 }; k <= m_sufficientHorizon; ++k)
    {
        std::vector kObservableCombinations {
            combinations<Observable>(
                observables.begin(),
                observables.end(),
                observables.size(),
                k
            )
        };

        for (std::vector<Observable>& kObservableCombination: kObservableCombinations)
        {
            do
            {
                std::vector sequences { observablePatchesCartesianProduct(kObservableCombination) };
                for (const auto& sequence: sequences)
                {
                    ObservablePatchSequence observablePatchSequence { sequence, spaceDimension() };
                    ObservableTraversalNode singTravNode { observablePatchSequence, preFlow, true };
                    ObservableTraversalNode openTravNode { observablePatchSequence, preFlow };
                    if (singTravNode.isEmpty() && openTravNode.isEmpty()) continue;

                    SingOpenStatePair singOpenStatePair { createStates(singTravNode, openTravNode) };
                    assert(singOpenStatePair.containsOpenState() || singOpenStatePair.containsSingState());

                    if (singOpenStatePair.containsSingState())
                    {
                        m_nodeByState[singOpenStatePair.singState()] = singTravNode;
                    }

                    if (singOpenStatePair.containsOpenState())
                    {
                        m_nodeByState[singOpenStatePair.openState()] = openTravNode;
                    }

                    singOpenStatePairBySequence[k][observablePatchSequence] = singOpenStatePair;
                    if (k == 1) continue;

                    ObservablePatchSequence advancedByOneObservablePatchSequence { advanceByOne(observablePatchSequence) };
                    SingOpenStatePair adjSingOpenStatePair { singOpenStatePairBySequence[k - 1][advancedByOneObservablePatchSequence] };
                    createEdges(singOpenStatePair, adjSingOpenStatePair);
                }
            } while (std::next_permutation(kObservableCombination.begin(), kObservableCombination.end(), compareObservableByAtoms));
        }

        singOpenStatePairBySequence[k-1].clear();
    }
}

SingOpenStatePair GeneralPolyhedralAbstraction::createStates(
    const ObservableTraversalNode& singTravNode,
    const ObservableTraversalNode& openTravNode
)
{
    if (singTravNode.isEmpty() && openTravNode.isEmpty())
        return {};

    if (!singTravNode.isEmpty() && !openTravNode.isEmpty())
    {
        unsigned singState { m_graph->new_state() };
        unsigned openState { m_graph->new_state() };
        return SingOpenStatePair { singState, openState };
    }

    if (!singTravNode.isEmpty())
    {
        unsigned singState { m_graph->new_state() };
        return SingOpenStatePair { singState, true };
    }

    unsigned openState { m_graph->new_state() };
    return SingOpenStatePair { openState, false };
}

void GeneralPolyhedralAbstraction::createEdges(const SingOpenStatePair singOpenStatePair, const SingOpenStatePair& adjSingOpenStatePair)
{
    if (adjSingOpenStatePair.containsSingState())
    {
        unsigned adjSingState { adjSingOpenStatePair.singState() };
        const Observable& adjSingStateObservable { observable(adjSingState) };
        bdd adjSingStateObservableBdd { observableAsBdd(adjSingStateObservable) };

        if (singOpenStatePair.containsSingState())
        {
            m_graph->new_acc_edge(singOpenStatePair.singState(), adjSingState, adjSingStateObservableBdd);
        }

        if (singOpenStatePair.containsOpenState())
        {
            m_graph->new_acc_edge(singOpenStatePair.openState(), adjSingState, adjSingStateObservableBdd);
        }
    }

    if (adjSingOpenStatePair.containsOpenState())
    {
        unsigned adjOpenState { adjSingOpenStatePair.openState() };
        const Observable& adjOpenStateObservable { observable(adjOpenState) };
        bdd adjSingStateObservableBdd { observableAsBdd(adjOpenStateObservable) };

        if (singOpenStatePair.containsSingState())
        {
            m_graph->new_acc_edge(singOpenStatePair.singState(), adjOpenState, adjSingStateObservableBdd);
        }

        if (singOpenStatePair.containsOpenState())
        {
            m_graph->new_acc_edge(singOpenStatePair.openState(), adjOpenState, adjSingStateObservableBdd);
        }
    }
}

bdd GeneralPolyhedralAbstraction::observableAsBdd(const Observable& observable)
{
    const spot::atomic_prop_set& observableAtoms { observable.atoms() };
    return spot::formula_to_bdd(SpotUtils::andAtoms(observableAtoms), m_graph->get_dict(), m_graph);
}

ObservablePatchSequence GeneralPolyhedralAbstraction::advanceByOne(const ObservablePatchSequence& observablePatchSequence)
{
    if (observablePatchSequence.isEmpty())
    {
        return ObservablePatchSequence { {}, spaceDimension() };
    }

    std::vector<ObservablePatch> advancedByOneSequence {};
    advancedByOneSequence.reserve(observablePatchSequence.totalPatches() - 1);
    const std::vector<ObservablePatch>& sequence { observablePatchSequence.sequence() };
    std::copy(sequence.begin() + 1, sequence.end(), std::back_inserter(advancedByOneSequence));
    return { std::move(advancedByOneSequence), spaceDimension() };
}

PPL::dimension_type GeneralPolyhedralAbstraction::spaceDimension()
{
    return m_spaceDimension;
}

void GeneralPolyhedralAbstraction::initializeGraph(spot::bdd_dict_ptr bddDict)
{
    m_graph = std::make_shared<spot::twa_graph>(bddDict);
    m_graph->prop_state_acc(spot::trival { true });
    m_graph->set_acceptance(spot::acc_cond::acc_code::t());
}

PowersetConstSharedPtr GeneralPolyhedralAbstraction::points(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    const ObservableTraversalNode& observableTraversalNode { m_nodeByState.at(state) };
    return observableTraversalNode.travPoints();
}

const Observable& GeneralPolyhedralAbstraction::observable(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    const ObservableTraversalNode& observableTraversalNode { m_nodeByState.at(state) };
    return observableTraversalNode.observable();
}

int GeneralPolyhedralAbstraction::totalStates() const
{
    return static_cast<int>(m_graph->num_states());
}

int GeneralPolyhedralAbstraction::totalEdges() const
{
    return static_cast<int>(m_graph->num_edges());
}

GeneralPolyhedralAbstraction::EdgeIterator GeneralPolyhedralAbstraction::successors(const unsigned state)
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    return m_graph->out(state);
}

bool GeneralPolyhedralAbstraction::hasSuccessors(const unsigned state)
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    auto edgeStorages { m_graph->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

int GeneralPolyhedralAbstraction::countSuccessors(const unsigned state)
{
    const EdgeIterator& edgeIterator { m_graph->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

spot::const_twa_graph_ptr GeneralPolyhedralAbstraction::twa() const
{
    return m_graph;
}

int GeneralPolyhedralAbstraction::initialState() const
{
    return static_cast<int>(m_graph->get_init_state_number());
}

void GeneralPolyhedralAbstraction::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_graph);
}

int GeneralPolyhedralAbstraction::sufficientHorizon() const
{
    return m_sufficientHorizon;
}
