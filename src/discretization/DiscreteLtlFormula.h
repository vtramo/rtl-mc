#pragma once

#include <spot/tl/formula.hh>
#include "discretization.h"

class DiscreteLtlFormula
{
public:
    static DiscreteLtlFormula discretizeToLtl(spot::formula&& formula)
    {
        spot::formula discretizedLtlFormula { toDiscretizedLtlFormula(std::move(formula)) };
        return DiscreteLtlFormula { imposeSingOpenLastFiniteProperty(std::move(discretizedLtlFormula)) };
    }
    const spot::formula& formula() const { return m_discreteLtlFormula; }
private:
    spot::formula m_discreteLtlFormula{};

    friend class BackwardNFA;
    friend class DiscreteFiniteLtlFormula;
    friend DiscreteLtlFormula discretizeToLtl(spot::formula&& formula);

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
