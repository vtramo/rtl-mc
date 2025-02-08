#include "BackwardNFAPermutator.h"

#include <catch2/generators/catch_generators.hpp>

BackwardNFAPermutator::BackwardNFAPermutator(const BackwardNFA& backwardNfa)
    : m_backwardNfa { backwardNfa }
{
}

BackwardNFAPermutator::PermutedBackwardNFAIterator::PermutedBackwardNFAIterator(const PermutedBackwardNFAIterator& other)
    : m_backwardNfaPermutator { other.m_backwardNfaPermutator }
    , m_permutedStates { other.m_permutedStates }
    , m_originalStates { other.m_originalStates }
    , m_totalPermutations { other.m_totalPermutations }
    , m_permutedBackwardNfa { std::make_unique<BackwardNFA>(*other.m_permutedBackwardNfa) }
    , m_end { other.m_end != nullptr ? std::make_unique<PermutedBackwardNFAIterator>(*other.m_end) : nullptr }
    , m_isEnd { other.m_isEnd }
{
}

BackwardNFAPermutator::PermutedBackwardNFAIterator::PermutedBackwardNFAIterator(
    const BackwardNFAPermutator& backwardNfaPermutator,
    std::vector<int> states
)
    : m_backwardNfaPermutator { backwardNfaPermutator }
    , m_permutedStates { states }
    , m_originalStates { std::move(states) }
{
    m_totalPermutations = factorial(states.size());
    m_permutationMap = createPermutationMap();
    m_permutedBackwardNfa = m_backwardNfaPermutator.swapStates(m_permutationMap, true);
    m_end = std::make_unique<PermutedBackwardNFAIterator>(*this);
    m_end->m_isEnd = true;
}

long BackwardNFAPermutator::PermutedBackwardNFAIterator::factorial(const int n)
{
    long f { 1 };

    for (int i { 1 }; i <= n; ++i)
        f *= i;

    return f;
}

const BackwardNFA& BackwardNFAPermutator::PermutedBackwardNFAIterator::operator*() const
{
    return *m_permutedBackwardNfa;
}

void BackwardNFAPermutator::PermutedBackwardNFAIterator::nextPermutedBackwardNfa()
{
    bool isNotEnd { std::next_permutation(m_permutedStates.begin(), m_permutedStates.end()) };
    m_permutationMap =  createPermutationMap();
    m_permutedBackwardNfa = m_backwardNfaPermutator.swapStates(m_permutationMap, false);
    if (!isNotEnd) m_isEnd = true;
}

void BackwardNFAPermutator::PermutedBackwardNFAIterator::prevPermutedBackwardNfa()
{
    std::prev_permutation(m_permutedStates.begin(), m_permutedStates.end());
    m_permutationMap = createPermutationMap();
    m_permutedBackwardNfa = std::move(m_backwardNfaPermutator.swapStates(m_permutationMap, false));
}

int BackwardNFAPermutator::PermutedBackwardNFAIterator::totalPermutations() const
{
    return m_totalPermutations;
}

const std::unordered_map<int, int>& BackwardNFAPermutator::PermutedBackwardNFAIterator::permutationMap()
{
    return m_permutationMap;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator& BackwardNFAPermutator::PermutedBackwardNFAIterator::operator++()
{
    nextPermutedBackwardNfa();
    return *this;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator BackwardNFAPermutator::PermutedBackwardNFAIterator::operator++(int)
{
    PermutedBackwardNFAIterator retVal { *this };
    nextPermutedBackwardNfa();
    return retVal;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator& BackwardNFAPermutator::PermutedBackwardNFAIterator::operator--()
{
    prevPermutedBackwardNfa();
    return *this;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator BackwardNFAPermutator::PermutedBackwardNFAIterator::operator--(int)
{
    PermutedBackwardNFAIterator retVal { *this };
    prevPermutedBackwardNfa();
    return retVal;
}

std::unordered_map<int, int> BackwardNFAPermutator::PermutedBackwardNFAIterator::createPermutationMap() const
{
    std::unordered_map<int, int> permutation {};
    int totalStates { static_cast<int>(m_originalStates.size()) };
    permutation.reserve(totalStates);
    for (int i { 0 }; i < totalStates; ++i) {
        const int state { m_originalStates[i] };
        const int replacingState { m_permutedStates[i] };
        permutation[state] = replacingState;
    }
    return permutation;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator BackwardNFAPermutator::PermutedBackwardNFAIterator::begin() const
{
    PermutedBackwardNFAIterator iterator { m_backwardNfaPermutator, m_originalStates };
    return ++iterator;
}

BackwardNFAPermutator::PermutedBackwardNFAIterator BackwardNFAPermutator::PermutedBackwardNFAIterator::end() const
{
    return *m_end;
}

bool BackwardNFAPermutator::PermutedBackwardNFAIterator::operator== (const PermutedBackwardNFAIterator& other) const
{
    return
        m_backwardNfaPermutator.m_backwardNfa.twa() == other.m_backwardNfaPermutator.m_backwardNfa.twa()
        && m_permutedStates == other.m_permutedStates
        && m_isEnd == other.m_isEnd;
}

bool BackwardNFAPermutator::PermutedBackwardNFAIterator::operator!= (const PermutedBackwardNFAIterator& other) const
{
    return !operator== (other);
}

BackwardNFAPermutator::PermutedBackwardNFAIterator BackwardNFAPermutator::generatePermutations(
    const std::unordered_set<int>& states
) const
{
    checkStates(states);
    std::vector<int> statesVector(states.size());
    std::copy(states.begin(), states.end(), statesVector.begin());
    return PermutedBackwardNFAIterator { *this, statesVector };
}

void BackwardNFAPermutator::checkStates(const std::unordered_set<int>& states) const
{
    if (states.size() < 2)
        throw std::invalid_argument("You must specify at least two states!");

    int firstState { *states.begin() };
    bool singularity { m_backwardNfa.stateDenotation(firstState).isSingular() };

    for (const auto& state: states)
    {
        if (m_backwardNfa.isInitialState(state))
            throw std::invalid_argument(fmt::format("You can't replace an initial state! (state: {})", state));

        if (state < 0 || state >= m_backwardNfa.totalStates())
        {
            throw std::invalid_argument(
                fmt::format(
                    "Invalid source state {}. Valid states range from 0 to {}.",
                    state,
                    m_backwardNfa.totalStates() - 1
                )
            );
        }
        //
        // const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        // if (stateDenotation.isSingular() != singularity)
        //     throw std::invalid_argument(fmt::format("The states must be either all singular or all non-singular!"));
    }
}

std::unique_ptr<BackwardNFA> BackwardNFAPermutator::swapStates(const std::unordered_map<int, int>& swapStatesMap) const
{
    return swapStates(swapStatesMap, true);
}

std::unique_ptr<BackwardNFA> BackwardNFAPermutator::swapStates(std::unordered_map<int, int>&& swapStateMap) const
{
    return swapStates(swapStateMap);
}

std::unique_ptr<BackwardNFA> BackwardNFAPermutator::swapStates(
    const std::unordered_map<int, int>& permutationStates,
    const bool checkSwap
) const
{
    if (checkSwap)
        checkSwapStateMapValidity(permutationStates);

    std::unordered_map inversePermutationStates { invertSwapStateMap(permutationStates) };

    spot::twa_graph_ptr permutedTwa {};
    const spot::bdd_dict_ptr backwardNfaDict { std::make_shared<spot::bdd_dict>() };
    permutedTwa = std::make_shared<spot::twa_graph>(backwardNfaDict);
    permutedTwa->prop_state_acc(spot::trival { true });
    permutedTwa->set_acceptance(m_backwardNfa.m_backwardNfa->get_acceptance());
    permutedTwa->new_states(m_backwardNfa.m_backwardNfa->num_states());

    std::unordered_set<int> initialStates {};
    std::unordered_set<int> finalStates {};
    std::unordered_map<int, StateDenotation> stateDenotationById {};
    stateDenotationById.reserve(m_backwardNfa.totalStates());
    for (int state { 0 }; state < m_backwardNfa.totalStates(); state++)
    {
        int targetState { replacingState(state, permutationStates, inversePermutationStates) };
        StateDenotation stateDenotation { m_backwardNfa.m_stateDenotationById.at(targetState) };
        stateDenotationById.emplace(state, stateDenotation);
        if (m_backwardNfa.isInitialState(targetState)) initialStates.insert(state);
        const bool isFinalState { m_backwardNfa.isFinalState(targetState) };
        if (isFinalState) finalStates.insert(state);
        for (auto& edge: m_backwardNfa.predecessors(targetState))
        {
            const int predecessor { static_cast<int>(edge.dst) };
            const int replacingPredecessor { replacingState(predecessor, permutationStates, inversePermutationStates) };
            permutedTwa->new_acc_edge(state, replacingPredecessor, bdd_true(), isFinalState);
        }
    }

    const int dummyInitialState { m_backwardNfa.m_dummyInitialState };
    for (const int finalState: finalStates)
        permutedTwa->new_acc_edge(dummyInitialState, finalState, bdd_true(), false);
    permutedTwa->set_init_state(dummyInitialState);

    std::unique_ptr permutedBackwardNfa { std::make_unique<BackwardNFA>(m_backwardNfa) };
    permutedBackwardNfa->m_backwardNfa = permutedTwa;
    permutedBackwardNfa->m_initialStates = initialStates;
    permutedBackwardNfa->m_finalStates = finalStates;
    permutedBackwardNfa->m_stateDenotationById = stateDenotationById;
    return permutedBackwardNfa;
}

int BackwardNFAPermutator::replacingState(
    const int state,
    const std::unordered_map<int, int>& swapStateMap,
    const std::unordered_map<int, int>& inverseSwapStateMap
)
{
    return swapStateMap.count(state) == 1
        ? swapStateMap.at(state)
        : inverseSwapStateMap.count(state) == 1
            ? inverseSwapStateMap.at(state)
            : state;
}

void BackwardNFAPermutator::checkSwapStateMapValidity(const std::unordered_map<int, int>& swapStateMap) const
{
    for (const auto& [state, replacingState]: swapStateMap)
    {
        if (m_backwardNfa.isInitialState(state))
            throw std::invalid_argument(fmt::format("You can't replace an initial state! (state: {})", state));

        if (state < 0 || state >= m_backwardNfa.totalStates())
        {
            throw std::invalid_argument(
                fmt::format(
                    "Invalid source state {}. Valid states range from 0 to {}.",
                    state,
                    m_backwardNfa.totalStates() - 1
                )
            );
        }

        if (replacingState < 0 || replacingState >= m_backwardNfa.totalStates())
        {
            throw std::invalid_argument(
                fmt::format(
                    "Invalid replacement state {}. Valid states range from 0 to {}.",
                    replacingState,
                    m_backwardNfa.totalStates() - 1
                )
            );
        }

        const StateDenotation& stateDenotation { m_backwardNfa.stateDenotation(state) };
        const StateDenotation& replacingStateDenotation { m_backwardNfa.stateDenotation(replacingState) };
        if (stateDenotation.isSingular() != replacingStateDenotation.isSingular())
            throw std::invalid_argument(
                fmt::format(
                    "it is impossible to replace a singular state with a non-singular one ({} -> {})",
                    state,
                    replacingState
                )
            );
    }
}

std::unordered_map<int, int> BackwardNFAPermutator::invertSwapStateMap(const std::unordered_map<int, int>& swapStateMap)
{
    std::unordered_map<int, int> inverseSwapStateMap {};
    inverseSwapStateMap.reserve(swapStateMap.size());
    for (auto [key, value]: swapStateMap)
        inverseSwapStateMap[value] = key;
    return inverseSwapStateMap;
}
