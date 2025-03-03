#pragma once

#include "reach.h"
#include "ppl_utils.h"
#include "ObservablePatchSequence.h"

class ObservablePatchSequenceSlice
{
public:
    ObservablePatchSequenceSlice(const ObservablePatchSequence& observablePatchSequence, unsigned startIndex, unsigned endIndex);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int totalPatches() const;
    [[nodiscard]] PPL::dimension_type spaceDimension() const;
    [[nodiscard]] std::optional<const ObservablePatch* const> firstObservablePatch() const;
    [[nodiscard]] ObservablePatchSequenceSlice advanceStartIndex() const;

private:
    const ObservablePatchSequence& m_observablePatchSequence;
    const unsigned m_startIndex {};
    const unsigned m_endIndex {};
};

PowersetSharedPtr traversalZero(const ObservablePatchSequence& sequence, const Poly& preFlow);
PowersetSharedPtr traversalZero(ObservablePatchSequenceSlice slice, const Poly& preFlow);

PowersetSharedPtr traversalPlus(const ObservablePatchSequence& sequence, const Poly& preFlow);
PowersetSharedPtr traversalPlus(ObservablePatchSequenceSlice slice, const Poly& preFlow);