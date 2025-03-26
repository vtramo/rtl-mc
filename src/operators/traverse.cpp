#include "traverse.h"
#include "reach.h"

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

/*
 * This method creates a new slice that starts at `startIndex + 1` and ends at `endIndex`.
 * If the slice is already empty, the returned slice will also be empty.
 */
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


/*!
 * Given a finite sequence of patches \f$\Delta\f$ (finite sequence of convex polyhedra), this function computes
 * the set of points from which there exists a left-closed trajectories that traverse the sequence \f$\Delta\f$.
 *
 * The function is recursively defined as:
 *
 * \f[
 * \mathit{Trav}^0(\epsilon) = \mathbb{R}^n
 * \f]
 * \f[
 * \mathit{Trav}^0(P\Delta) = \mathit{reach}^0(P, \mathit{Trav}^+(\Delta))
 * \f]
 *
 * where \f$\epsilon\f$ is the empty sequence, \f$P\f$ is the first patch of the \ref ObservablePatchSequence,
 * and \f$\Delta\f$ is a sequence of patches without the first patch \f$P\f$.
 */
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


/*!
 * Given a finite sequence of patches \f$\Delta\f$ (finite sequence of convex polyhedra), this function computes
 * the set of points from which there exists a left-open admissible trajectory that traverse the sequence \f$\Delta\f$.
 *
 * For a patch \f$P \in \Delta\f$, we denote by \f$\alpha_P\f$ the observable to which it belongs.
 * The function is recursively defined as:
 *
 * \f[
 * \mathit{Trav}^+(\epsilon) = \mathbb{R}^n
 * \f]
 * \f[
 * \mathit{Trav}^+(P\Delta) = P\, \cap\, \mathit{reach}^+([\![\alpha_P]\!], \mathit{Trav}^+(\Delta))
 * \f]
 *
 * where \f$\epsilon\f$ is the empty sequence, \f$P\f$ is the first patch of the \ref ObservablePatchSequence,
 * and \f$\Delta\f$ is a sequence of patches without the first patch \f$P\f$.
 */
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
    PowersetUniquePtr reachPlusResult { reachPlus(*firstObservableInterpretation, *traversalZeroResult, preFlow) };
    return PPLUtils::intersect(firstObservablePatch->patch(), std::move(*reachPlusResult));
}
