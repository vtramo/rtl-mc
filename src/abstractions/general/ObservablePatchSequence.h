#pragma once

#include <vector>
#include <optional>

#include "ObservablePatch.h"
#include "utils.h"

class ObservablePatchSequence
{
public:
    ObservablePatchSequence(const std::vector<ObservablePatch>& sequence, const PPL::dimension_type spaceDimension)
        : m_sequence { sequence }
        , m_spaceDimension { spaceDimension }
    {
        computeHashCode();
    }

    ObservablePatchSequence(std::vector<ObservablePatch>&& sequence, const PPL::dimension_type spaceDimension)
        : m_sequence { std::move(sequence) }
        , m_spaceDimension { spaceDimension }
    {
        computeHashCode();
    }

    ObservablePatchSequence(const ObservablePatchSequence& other)
        : m_sequence { other.m_sequence }
        , m_spaceDimension { other.m_spaceDimension }
        , m_hashcode { other.m_hashcode }
    {}

    ObservablePatchSequence(ObservablePatchSequence&& other) noexcept
        : m_sequence { std::move(other.m_sequence) }
        , m_hashcode { std::move(other.m_hashcode) }
    {}

    [[nodiscard]] const std::vector<ObservablePatch>& sequence() const { return m_sequence; }
    [[nodiscard]] std::vector<ObservablePatch>::const_iterator cbegin() const { return m_sequence.cbegin(); }
    [[nodiscard]] std::vector<ObservablePatch>::const_iterator cend() const { return m_sequence.cend(); }
    [[nodiscard]] bool isEmpty() const { return m_sequence.empty(); }
    [[nodiscard]] int totalPatches() const { return static_cast<int>(m_sequence.size()); }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_spaceDimension; }
    [[nodiscard]] std::optional<const Observable* const> firstObservable() const
    {
        if (isEmpty()) return {};
        return &(*firstObservablePatch())->observable();
    }
    [[nodiscard]] std::optional<const ObservablePatch* const> firstObservablePatch() const
    {
        if (!m_sequence.empty())
            return &m_sequence[0];

        return {};
    }

    [[nodiscard]] std::size_t hashcode() const { return m_hashcode; }

private:
    std::vector<ObservablePatch> m_sequence {};
    PPL::dimension_type m_spaceDimension {};
    std::size_t m_hashcode {};

    void computeHashCode()
    {
        for (const auto& observablePatch: m_sequence)
            hashCombine(m_hashcode, observablePatch);
    }
};

inline bool operator== (const ObservablePatchSequence& seq1, const ObservablePatchSequence& seq2)
{
    return seq1.sequence() == seq2.sequence();
}

inline bool operator!= (const ObservablePatchSequence& seq1, const ObservablePatchSequence& seq2)
{
    return !(seq1 == seq2);
}

template<> struct std::hash<ObservablePatchSequence>
{
    std::size_t operator() (const ObservablePatchSequence& seq) const noexcept
    {
        return seq.hashcode();
    }
};
