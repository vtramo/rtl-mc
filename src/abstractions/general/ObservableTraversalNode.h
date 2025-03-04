#pragma once

#include "ppl_utils.h"
#include "Observable.h"
#include "ObservablePatchSequence.h"
#include "traversal.h"
#include "spot_utils.h"

class ObservableTraversalNode
{
public:
    ObservableTraversalNode(ObservablePatchSequence sequence, const Poly& preFlow, bool isSing = false)
        : m_isSing { isSing }
    {
        if (sequence.isEmpty())
        {
            throw std::invalid_argument("ObservableTraversalNode: empty sequence!");
        }

        computeObservable(sequence);
        computeTravPoints(sequence, preFlow);
    }

    [[nodiscard]] const Observable& observable() const { return m_observable; }
    [[nodiscard]] PowersetConstSharedPtr travPoints() const { return m_travPoints; }
    [[nodiscard]] bool isSingural() const { return m_isSing; }
    [[nodiscard]] bool isEmpty() const { return m_travPoints->is_empty(); }
    [[nodiscard]] bool isUniverse() const { return m_travPoints->is_universe(); }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_observable.spaceDimension(); }

private:
    PowersetSharedPtr m_travPoints {};
    Observable m_observable {};
    bool m_isSing { false };

    void computeObservable(const ObservablePatchSequence& sequence)
    {
        const Observable& observable { **sequence.firstObservable() };

        if (!m_isSing)
        {
            m_observable = Observable { observable };
        }
        else
        {
            spot::atomic_prop_set atoms { observable.atoms() };
            atoms.insert(SpotUtils::sing());
            m_observable = Observable { atoms, *observable.interpretation() };
        }
    }

    void computeTravPoints(const ObservablePatchSequence& sequence, const Poly& preFlow)
    {
        m_travPoints = m_isSing
            ? traversalZero(sequence, preFlow)
            : traversalPlus(sequence, preFlow);
    }
};