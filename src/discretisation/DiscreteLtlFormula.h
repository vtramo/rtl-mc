#pragma once

#include <spot/tl/formula.hh>
#include "discretisation.h"

class DiscreteLtlFormula
{
public:
    static DiscreteLtlFormula discretiseRtl(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyAlternationSingOpenObservables(std::move(discretisedLtlFormula)) };
    }

    static DiscreteLtlFormula discretiseRtlFinite(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedLtlFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyFiniteAlternationSingOpenObservablesOneStep(std::move(discretisedLtlFormula)) };
    }
    const spot::formula& formula() const { return m_discreteLtlFormula; }

    DiscreteLtlFormula() = default;
private:
    spot::formula m_discreteLtlFormula {};

    friend class DiscreteFiniteLtlFormula;

    explicit DiscreteLtlFormula(spot::formula&& formula)
        : m_discreteLtlFormula { std::move(formula) }
    {
        assert(m_discreteLtlFormula.is_ltl_formula() && "Must be a valid LTL formula!");
    }
};

inline bool operator== (const DiscreteLtlFormula& discreteLtlFormula, const spot::formula& formula)
{
    return discreteLtlFormula.formula() == formula;
}

inline std::ostream& operator<< (std::ostream& os, const DiscreteLtlFormula& formula)
{
    return os << formula.formula();
}
