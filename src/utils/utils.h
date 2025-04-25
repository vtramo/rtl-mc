#pragma once

#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>

/*!
 * \brief Generates all simple combinations of size `k` from a range of `n` elements.
 *
 * This function computes all possible simple combinations of size `k` from a given range of elements
 * defined by the iterators `begin` and `end`. A simple combination is a selection of items where
 * the order does not matter and each item can be selected only once.
 *
 * The function uses a bitmask-based approach to efficiently generate all simple combinations.
 *
 * \tparam T The type of elements in the input range.
 * \tparam Iterator The type of iterator for the input range (must support forward iteration).
 * \param begin Iterator to the beginning of the input range.
 * \param end Iterator to the end of the input range.
 * \param n The total number of elements in the input range.
 * \param k The size of each combination to generate.
 * \return A vector of vectors, where each inner vector represents a simple combination of size `k`.
 *
 * \throws std::invalid_argument If `n` or `k` is less than or equal to 0.
 * \throws std::invalid_argument If `n` is less than `k`.
 *
 * \note The function assumes that the input range contains at least `n` elements. If the range
 * is smaller, the behavior is undefined.
 *
 * Example usage:
 * \code
 * std::vector<int> elements = {1, 2, 3, 4};
 * auto result = simpleCombinations<int>(elements.begin(), elements.end(), 4, 2);
 * // result will contain: {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}
 * \endcode
 */
template <typename T, typename Iterator>
std::vector<std::vector<T>> simpleCombinations(Iterator begin, Iterator end, const int n, const int k)
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

    // Compute the total number of simple combinations
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

/*!
 *  \brief Combines multiple values into a single hash value.
 *
 * This function is a variadic template that combines multiple values into a single hash value.
 * It uses a hash combination technique to ensure that the resulting hash is well-distributed
 * and minimizes collisions. The function is particularly useful for combining multiple fields into a single hash value.
 *
 * The hash combination algorithm is based on the following formula:
 * \f[
 * seed = seed \oplus (\text{std::hash<T>\{\}(v)} + \text{0x9e3779b9} + (seed \ll 6) + (seed \gg 2))
 * \f]
 * where:
 * - \f$\oplus\f$ is the bitwise XOR operator.
 * - \f$\ll\f$ is the bitwise left shift operator.
 * - \f$\gg\f$ is the bitwise right shift operator.
 * - \f$\text{0x9e3779b9}\f$ is a magic constant derived from the Golden Ratio irrational number, used to improve hash distribution.
 *
 * \tparam T The type of the first value to hash.
 * \tparam Rest The types of the remaining values to hash.
 * \param seed The initial hash value, which will be updated with the combined hash of all input values.
 * \param v The first value to hash.
 * \param rest The remaining values to hash.
 *
 * \note This function modifies the `seed` parameter in place. It is designed to be used in custom hash
 * functions for user-defined types, where multiple fields need to be combined into a single hash value.
 *
 * \warning The type `T` and all types in `Rest` **must** provide a specialization of `std::hash`. If any type
 * does not support `std::hash`, a compilation error will occur.
 *
 * Example usage:
 * \code
 * struct MyStruct {
 *     int a;
 *     double b;
 *     std::string c;
 * };
 *
 * struct MyStructHash {
 *     std::size_t operator()(const MyStruct& s) const {
 *         std::size_t seed = 0;
 *         hashCombine(seed, s.a, s.b, s.c);
 *         return seed;
 *     }
 * };
 * \endcode
 */

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

/*!
 *  \brief Checks if a string ends with a specified suffix.
 *  \param str The string to check.
 *  \param suffix The suffix to compare with the end of the string.
 *  \return `true` if the string ends with the suffix, `false` otherwise.
 */
inline bool endsWith(const std::string_view str, const std::string_view suffix) {
    if (suffix.size() > str.size())
    {
        return false;
    }

    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

inline std::string nowToString(const std::string_view format = "%d-%m-%YT%H:%M:%S")
{
    auto t { std::time(nullptr) };
    auto tm { *std::localtime(&t) };
    std::ostringstream tmStringStream {};
    tmStringStream << std::put_time(&tm, std::string { format }.c_str());
    return tmStringStream.str();
}