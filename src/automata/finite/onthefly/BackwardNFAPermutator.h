#pragma once

#include <iterator>
#include "BackwardNFA.h"

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
        [[nodiscard]] const std::unordered_map<unsigned, unsigned>& permutationMap();

        PermutedBackwardNFAIterator begin() const;
        PermutedBackwardNFAIterator end() const;

    private:
        const BackwardNFAPermutator& m_backwardNfaPermutator;
        std::vector<unsigned> m_permutedStates {};
        std::vector<unsigned> m_originalStates {};
        std::unordered_map<unsigned, unsigned> m_permutationMap {};
        int m_totalPermutations {};
        std::unique_ptr<BackwardNFA> m_permutedBackwardNfa {};
        std::unique_ptr<PermutedBackwardNFAIterator> m_end {};
        bool m_isEnd { false };

        explicit PermutedBackwardNFAIterator(const BackwardNFAPermutator& backwardNfaPermutator, std::vector<unsigned> states);
        friend class BackwardNFAPermutator;

        void nextPermutedBackwardNfa();
        void prevPermutedBackwardNfa();
        std::unordered_map<unsigned, unsigned> createPermutationMap() const;

        static long factorial(int n);
    };

    explicit BackwardNFAPermutator(const BackwardNFA& backwardNfa);

    PermutedBackwardNFAIterator generatePermutations(const std::unordered_set<unsigned>& states) const;
    std::unique_ptr<BackwardNFA> swapStates(const std::unordered_map<unsigned, unsigned>& swapStateMap) const;
    std::unique_ptr<BackwardNFA> swapStates(std::unordered_map<unsigned, unsigned>&& swapStateMap) const;

private:
    const BackwardNFA& m_backwardNfa {};

    std::unique_ptr<BackwardNFA> swapStates(const std::unordered_map<unsigned, unsigned>& permutationStates, bool checkSwap) const;
    void checkSwapStateMapValidity(const std::unordered_map<unsigned, unsigned>& swapStateMap) const;
    void checkStates(const std::unordered_set<unsigned>& states) const;
    static unsigned replacingState(unsigned state, const std::unordered_map<unsigned, unsigned>& swapStateMap,
                                   const std::unordered_map<unsigned, unsigned>& inverseSwapStateMap);
    static std::unordered_map<unsigned, unsigned> invertSwapStateMap(
        const std::unordered_map<unsigned, unsigned>& swapStateMap);
};