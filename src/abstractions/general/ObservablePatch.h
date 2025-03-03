#pragma once

#include "Observable.h"
#include "ppl_utils.h"
#include "utils.h"

class ObservablePatch
{
public:
    ObservablePatch(Observable observable, const Poly& patch)
        : m_patch { patch }
        , m_observable { observable }
    {
        assert(PPLUtils::containsDisjunct(*m_observable.interpretation(), m_patch));

        hashCombine(m_hashcode, m_observable, m_patch);
    }

    [[nodiscard]] const Observable& observable() const { return m_observable; }
    [[nodiscard]] const Poly& patch() const { return m_patch; }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_observable.spaceDimension(); }
    [[nodiscard]] std::size_t hashcode() const { return m_hashcode; }

private:
    const Poly& m_patch;
    Observable m_observable {};
    std::size_t m_hashcode {};
};

inline bool operator== (const ObservablePatch& observablePatch1, const ObservablePatch& observablePatch2)
{
    return observablePatch1.observable() == observablePatch2.observable() &&
           observablePatch1.patch()      == observablePatch2.patch();
}

inline bool operator!= (const ObservablePatch& observablePatch1, const ObservablePatch& observablePatch2)
{
    return !(observablePatch1 == observablePatch2);
}

template<> struct std::hash<ObservablePatch>
{
    std::size_t operator() (const ObservablePatch& observablePatch) const noexcept
    {
        return observablePatch.hashcode();
    }
};