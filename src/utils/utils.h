#pragma once

#include <vector>
#include <algorithm>

template <typename T, typename Iterator>
std::vector<std::vector<T>> combinations(Iterator begin, Iterator end, const int n, const int k)
{
    if (n <= 0 || k <= 0)
        throw std::invalid_argument("n and k must be greater than 0");

    if (n < k)
        throw std::invalid_argument("n must be >= k");

    if (n == k)
    {
        std::vector<T> combination {};
        for (Iterator it { begin }; it != end; ++it)
            combination.push_back(*it);
        return { combination };
    }

    std::vector<std::vector<T>> combinations {};
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
        std::vector<T> combination {};

        int idx { 0 };
        for (auto it = begin; it != end; ++it, ++idx)
            if (bitmask[idx])
                combination.push_back(*it);

        combinations.push_back(std::move(combination));
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return combinations;
}

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}