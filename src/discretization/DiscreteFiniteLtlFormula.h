#pragma once

#include <spot/tl/formula.hh>
#include <spot/tl/ltlf.hh>
#include "DiscreteLtlFormula.h"

class DiscreteFiniteLtlFormula {
public:
    static DiscreteFiniteLtlFormula discretize(spot::formula&& formula) {
        spot::formula discreteLtlFormula { toDiscretizedFormula(std::move(formula)) };
        return DiscreteFiniteLtlFormula { imposeSingOpenLastProperty(std::move(discreteLtlFormula)) };
    }
    const spot::formula& formula() const { return m_discreteFiniteLtlFormula; }
    DiscreteLtlFormula toLtl() const { return DiscreteLtlFormula { spot::from_ltlf(m_discreteFiniteLtlFormula) }; }

private:
    spot::formula m_discreteFiniteLtlFormula {};

    friend DiscreteFiniteLtlFormula discretize(spot::formula&& formula);

    DiscreteFiniteLtlFormula() = default;
    explicit DiscreteFiniteLtlFormula(spot::formula&& formula)
        : m_discreteFiniteLtlFormula { std::move(formula) }
    {
        assert(m_discreteFiniteLtlFormula.is_ltl_formula() && "Must be a valid LTL formula!");
    }
};

inline bool operator== (const DiscreteFiniteLtlFormula& discreteFormula, const spot::formula& formula)
{
    return discreteFormula.formula() == formula;
}

inline std::ostream& operator<< (std::ostream& os, const DiscreteFiniteLtlFormula& formula)
{
    return os << formula.formula();;
}