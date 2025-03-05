#pragma once

#include <spot/tl/formula.hh>
#include "discretisation.h"

class DiscreteLtlFormula
{
public:
    static DiscreteLtlFormula discretise(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedLtlFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyAlternationSingOpenObservablesOneStep(std::move(discretisedLtlFormula)) };
    }

    static DiscreteLtlFormula discretiseFromFiniteLtl(spot::formula&& formula)
    {
        spot::formula discretisedLtlFormula { toDiscretisedLtlFormula(std::move(formula)) };
        return DiscreteLtlFormula { applyFiniteAlternationSingOpenObservablesOneStep(std::move(discretisedLtlFormula)) };
    }
    const spot::formula& formula() const { return m_discreteLtlFormula; }
private:
    spot::formula m_discreteLtlFormula {};

    friend class BackwardNFA;
    friend class DiscreteFiniteLtlFormula;
    friend class FiniteLtlAutomaton;
    friend class LtlAutomaton;
    friend DiscreteLtlFormula discretiseToLtl(spot::formula&& formula);

    DiscreteLtlFormula() = default;
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
