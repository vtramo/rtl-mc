#pragma once

#include <vector>
#include <algorithm>

template <typename T, typename Iterator>
std::vector<std::set<T>> combinations(Iterator begin, Iterator end, const int n, const int k)
{
    if (n <= 0 || k <= 0)
        throw std::invalid_argument("n and k must be greater than 0");

    if (n < k)
        throw std::invalid_argument("n must be >= k");

    if (n == k)
    {
        std::set<T> combination {};
        for (Iterator it { begin }; it != end; ++it)
            combination.insert(*it);
        return { combination };
    }

    std::vector<std::set<T>> combinations {};
    long long numCombinations { 1 };
    for (int i = 0; i < k; ++i)
    {
        numCombinations *= (n - i);
        numCombinations /= (i + 1);
    }

    combinations.reserve(numCombinations);

    std::vector bitmask(k, true);
    bitmask.resize(n, false);
    do {
        std::set<T> combination {};

        int idx { 0 };
        for (auto it = begin; it != end; ++it, ++idx)
            if (bitmask[idx])
                combination.insert(*it);

        combinations.push_back(std::move(combination));
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return combinations;
}