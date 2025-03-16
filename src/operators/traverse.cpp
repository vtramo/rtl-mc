#include "traverse.h"

ObservablePatchSequenceSlice::ObservablePatchSequenceSlice(
    const ObservablePatchSequence& observablePatchSequence,
    const unsigned startIndex,
    const unsigned endIndex
)
    : m_observablePatchSequence { observablePatchSequence }
    , m_startIndex { startIndex }
    , m_endIndex { endIndex }
{
    const unsigned totalPatches { static_cast<unsigned>(observablePatchSequence.totalPatches()) };
    if (m_endIndex > totalPatches)
        throw std::invalid_argument("ObservablePatchSequenceSlice: 'endIndex' must be less than the total number of patches!");
}

ObservablePatchSequenceSlice ObservablePatchSequenceSlice::advanceStartIndexByOne() const
{
    return ObservablePatchSequenceSlice { m_observablePatchSequence, m_startIndex + 1, m_endIndex };
}

bool ObservablePatchSequenceSlice::isEmpty() const
{
    return m_startIndex > m_endIndex;
}

int ObservablePatchSequenceSlice::totalPatches() const
{
    if (isEmpty()) return 0;
    return m_endIndex - m_startIndex + 1;
}

std::optional<const ObservablePatch* const> ObservablePatchSequenceSlice::firstObservablePatch() const
{
    if (isEmpty()) return {};
    return &m_observablePatchSequence.sequence()[m_startIndex];
}

std::optional<PowersetConstSharedPtr> ObservablePatchSequenceSlice::firstObservableInterpretation() const
{
    if (isEmpty()) return {};
    return m_observablePatchSequence.sequence()[m_startIndex].observableInterpretation();
}

PPL::dimension_type ObservablePatchSequenceSlice::spaceDimension() const
{
    return m_observablePatchSequence.spaceDimension();
}

unsigned ObservablePatchSequenceSlice::startIndex() const
{
    return m_startIndex;
}

unsigned ObservablePatchSequenceSlice::endIndex() const
{
    return m_endIndex;
}

PowersetSharedPtr traverseZero(const ObservablePatchSequence& sequence, const Poly& preFlow)
{
    if (sequence.isEmpty())
    {
        return std::make_shared<Powerset>(sequence.spaceDimension(), PPL::UNIVERSE);
    }

    constexpr unsigned sliceStartIndex { 0 };
    const unsigned sliceEndIndex { static_cast<unsigned>(sequence.totalPatches() - 1) };
    ObservablePatchSequenceSlice slice { sequence, sliceStartIndex, sliceEndIndex };
    return traverseZero(slice, preFlow);
}

PowersetSharedPtr traverseZero(const ObservablePatchSequenceSlice slice, const Poly& preFlow)
{
    if (slice.isEmpty())
    {
        return std::make_shared<Powerset>(slice.spaceDimension(), PPL::UNIVERSE);
    }

    const ObservablePatch* firstObservablePatch { *slice.firstObservablePatch() };
    const Poly& firstPatch { firstObservablePatch->patch() };
    PowersetSharedPtr traversalPlusResult { traversePlus(slice.advanceStartIndexByOne(), preFlow) };
    return reach0(firstPatch, *traversalPlusResult, preFlow);
}

PowersetSharedPtr traversePlus(const ObservablePatchSequence& sequence, const Poly& preFlow)
{
    if (sequence.isEmpty())
    {
        return std::make_shared<Powerset>(sequence.spaceDimension(), PPL::UNIVERSE);
    }

    constexpr unsigned sliceStartIndex { 0 };
    const unsigned sliceEndIndex { static_cast<unsigned>(sequence.totalPatches() - 1) };
    ObservablePatchSequenceSlice slice { sequence, sliceStartIndex, sliceEndIndex };
    return traversePlus(slice, preFlow);
}

PowersetSharedPtr traversePlus(const ObservablePatchSequenceSlice slice, const Poly& preFlow)
{
    if (slice.isEmpty())
    {
        return std::make_shared<Powerset>(slice.spaceDimension(), PPL::UNIVERSE);
    }

    ObservablePatchSequenceSlice advancedSliceByOne { slice.advanceStartIndexByOne() };
    PowersetSharedPtr traversalZeroResult { traverseZero(advancedSliceByOne, preFlow) };

    const ObservablePatch* firstObservablePatch { *slice.firstObservablePatch() };
    PowersetConstSharedPtr firstObservableInterpretation { firstObservablePatch->observableInterpretation() };
    PowersetConstSharedPtr reachObservableInterpretation {
        advancedSliceByOne.isEmpty()
            ? firstObservableInterpretation
            : PPLUtils::fusion(
                *firstObservableInterpretation,
                (*advancedSliceByOne.firstObservablePatch())->patch()
              )
    };
    PowersetUniquePtr reachPlusResult { reachPlus(*reachObservableInterpretation, *traversalZeroResult, preFlow) };
    return PPLUtils::intersect(firstObservablePatch->patch(), std::move(*reachPlusResult));
}
