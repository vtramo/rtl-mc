#pragma once

#include "BackwardNFA.h"
#include <iterator>

class BackwardNFAPermutator
{
public:
    class PermutedBackwardNFAIterator
    {
    public:
        using value_type = BackwardNFA;
        using pointer = BackwardNFA*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        PermutedBackwardNFAIterator(const PermutedBackwardNFAIterator& other);

        const BackwardNFA& operator* () const;
        PermutedBackwardNFAIterator& operator++ ();
        PermutedBackwardNFAIterator operator++ (int);
        PermutedBackwardNFAIterator& operator-- ();
        PermutedBackwardNFAIterator operator-- (int);
        bool operator== (const PermutedBackwardNFAIterator& other) const;
        bool operator!= (const PermutedBackwardNFAIterator& other) const;

        [[nodiscard]] int totalPermutations() const;
        [[nodiscard]] const std::unordered_map<int, int>& permutationMap();

        PermutedBackwardNFAIterator begin() const;
        PermutedBackwardNFAIterator end() const;

    private:
        const BackwardNFAPermutator& m_backwardNfaPermutator;
        std::vector<int> m_permutedStates {};
        std::vector<int> m_originalStates {};
        std::unordered_map<int, int> m_permutationMap {};
        int m_totalPermutations {};
        std::unique_ptr<BackwardNFA> m_permutedBackwardNfa {};
        std::unique_ptr<PermutedBackwardNFAIterator> m_end {};
        bool m_isEnd { false };

        explicit PermutedBackwardNFAIterator(const BackwardNFAPermutator& backwardNfaPermutator, std::vector<int> states);
        friend class BackwardNFAPermutator;

        void nextPermutedBackwardNfa();
        void prevPermutedBackwardNfa();
        std::unordered_map<int, int> createPermutationMap() const;

        static long factorial(int n);
    };

    explicit BackwardNFAPermutator(const BackwardNFA& backwardNfa);

    PermutedBackwardNFAIterator generatePermutations(const std::unordered_set<int>& states) const;
    std::unique_ptr<BackwardNFA> swapStates(const std::unordered_map<int, int>& swapStateMap) const;
    std::unique_ptr<BackwardNFA> swapStates(std::unordered_map<int, int>&& swapStateMap) const;

private:
    const BackwardNFA& m_backwardNfa {};

    std::unique_ptr<BackwardNFA> swapStates(const std::unordered_map<int, int>& permutationStates, bool checkSwap) const;
    void checkSwapStateMapValidity(const std::unordered_map<int, int>& swapStateMap) const;
    void checkStates(const std::unordered_set<int>& states) const;
    static int replacingState(int state, const std::unordered_map<int, int>& swapStateMap, const std::unordered_map<int, int>& inverseSwapStateMap);
    static std::unordered_map<int, int> invertSwapStateMap(const std::unordered_map<int, int>& swapStateMap);
};