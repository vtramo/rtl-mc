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
    [[nodiscard]] std::optional<PowersetConstSharedPtr> firstObservableInterpretation() const;
    [[nodiscard]] ObservablePatchSequenceSlice advanceStartIndexByOne() const;
    [[nodiscard]] unsigned startIndex() const;
    [[nodiscard]] unsigned endIndex() const;

private:
    const ObservablePatchSequence& m_observablePatchSequence;
    const unsigned m_startIndex {};
    const unsigned m_endIndex {};
};

PowersetSharedPtr traverseZero(const ObservablePatchSequence& sequence, const Poly& preFlow);
PowersetSharedPtr traverseZero(ObservablePatchSequenceSlice slice, const Poly& preFlow);

PowersetSharedPtr traversePlus(const ObservablePatchSequence& sequence, const Poly& preFlow);
PowersetSharedPtr traversePlus(ObservablePatchSequenceSlice slice, const Poly& preFlow);