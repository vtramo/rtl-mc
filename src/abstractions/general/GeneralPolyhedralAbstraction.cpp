#include <spot/twaalgos/dot.hh>
#include <spot/twa/formula2bdd.hh>

#include "GeneralPolyhedralAbstraction.h"
#include "observable_patch_cartesian_product.h"
#include "SingOpenStatePair.h"

GeneralPolyhedralAbstraction::GeneralPolyhedralAbstraction(
    PolyhedralSystemSharedPtr polyhedralSystem,
    std::vector<Observable>&& observables,
    const unsigned maxObservablePatchSequenceLength,
    const std::string_view name
) : PolyhedralAbstraction(polyhedralSystem, name)
{
    buildAbstraction(maxObservablePatchSequenceLength, std::move(observables));
}

GeneralPolyhedralAbstraction::GeneralPolyhedralAbstraction(
    PolyhedralSystemSharedPtr polyhedralSystem,
    const std::string_view name
) : PolyhedralAbstraction(polyhedralSystem, name)
{
    Log::log(Verbosity::veryVerbose, "[{}] Generating polyhedral system observables...", m_name);
    std::vector observables { m_polyhedralSystem->getOrGenerateObservables() };
    std::size_t totalObservables { observables.size() };
    buildAbstraction(totalObservables, std::move(observables));
}

void GeneralPolyhedralAbstraction::buildAbstraction(const unsigned maxObservablePatchSequenceLength, std::vector<Observable>&& observables)
{
    Log::log(Verbosity::verbose, "[{}] Polyhedral system total observables: {}.", m_name, observables.size());
    Log::log(Verbosity::verbose, "[{}] Sufficient horizon (maxObservablePatchSequenceLength): {}.", m_name, maxObservablePatchSequenceLength);

    const Poly& preFlow { m_polyhedralSystem->preFlow() };

    auto compareObservableByAtoms {
        [](const Observable& observable1, const Observable& observable2)
        { return observable1.atoms() < observable2.atoms(); }
    };

    std::unordered_map<int, std::unordered_map<ObservablePatchSequence, SingOpenStatePair>> singOpenStatePairBySequence {};
    for (unsigned k { 1 }; k <= maxObservablePatchSequenceLength; ++k)
    {
        std::vector kObservableCombinations {
            simpleCombinations<Observable>(
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
                    ObservableTraverseNode singTravNode { observablePatchSequence, preFlow, true };
                    ObservableTraverseNode openTravNode { observablePatchSequence, preFlow };
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
    const ObservableTraverseNode& singTravNode,
    const ObservableTraverseNode& openTravNode
)
{
    if (singTravNode.isEmpty() && openTravNode.isEmpty())
        return {};

    if (!singTravNode.isEmpty() && !openTravNode.isEmpty())
    {
        unsigned singState { m_automaton->new_state() };
        unsigned openState { m_automaton->new_state() };
        return SingOpenStatePair { singState, openState };
    }

    if (!singTravNode.isEmpty())
    {
        unsigned singState { m_automaton->new_state() };
        return SingOpenStatePair { singState, true };
    }

    unsigned openState { m_automaton->new_state() };
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
            m_automaton->new_acc_edge(singOpenStatePair.singState(), adjSingState, adjSingStateObservableBdd);
        }

        if (singOpenStatePair.containsOpenState())
        {
            m_automaton->new_acc_edge(singOpenStatePair.openState(), adjSingState, adjSingStateObservableBdd);
        }
    }

    if (adjSingOpenStatePair.containsOpenState())
    {
        unsigned adjOpenState { adjSingOpenStatePair.openState() };
        const Observable& adjOpenStateObservable { observable(adjOpenState) };
        bdd adjSingStateObservableBdd { observableAsBdd(adjOpenStateObservable) };

        if (singOpenStatePair.containsSingState())
        {
            m_automaton->new_acc_edge(singOpenStatePair.singState(), adjOpenState, adjSingStateObservableBdd);
        }

        if (singOpenStatePair.containsOpenState())
        {
            m_automaton->new_acc_edge(singOpenStatePair.openState(), adjOpenState, adjSingStateObservableBdd);
        }
    }
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

PowersetConstSharedPtr GeneralPolyhedralAbstraction::points(const unsigned state) const
{
    PolyhedralAbstraction::assertThatStateIsInRange(state);

    const ObservableTraverseNode& observableTraverseNode { m_nodeByState.at(state) };
    return observableTraverseNode.travPoints();
}

const Observable& GeneralPolyhedralAbstraction::observable(const unsigned state) const
{
    PolyhedralAbstraction::assertThatStateIsInRange(state);

    const ObservableTraverseNode& observableTraverseNode { m_nodeByState.at(state) };
    return observableTraverseNode.observable();
}
