#ifndef DISCRETIZELTLFORMULA_H
#define DISCRETIZELTLFORMULA_H

#include <spot/tl/formula.hh>

class DiscreteLtlFormula
{
public:
    const spot::formula& formula() const { return m_discreteLtlFormula; }
private:
    spot::formula m_discreteLtlFormula{};

    friend class BackwardNFA;
    friend DiscreteLtlFormula discretize(spot::formula&& formula);

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
    return os << formula.formula();;
}

#endif //DISCRETIZELTLFORMULA_H
