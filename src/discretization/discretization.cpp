#include "discretization.h"
#include "spot_constants.h"

spot::formula discretize(spot::formula&& formula)
{
    spot::formula discretizedLtlFormula { toDiscretizedLtlFormula(std::move(formula)) };
    return imposeSingOpenLastProperty(std::move(discretizedLtlFormula));
}

spot::formula imposeSingOpenLastProperty(spot::formula&& formula)
{
    spot::formula singOpenLastProperty { spot::constants::singOpenLastProperty };
    return  { And({ std::move(formula), std::move(singOpenLastProperty) }) };
}

spot::formula toDiscretizedLtlFormula(spot::formula&& formula)
{
    switch (formula.kind())
    {
    case spot::op::F:
        return ndtF(std::move(formula));
    case spot::op::G:
        return ndtG(std::move(formula));
    case spot::op::U:
        return ndtU(std::move(formula));
    case spot::op::S:
        return ndtS(std::move(formula));
    case spot::op::W:
        return ndtW(std::move(formula));
    case spot::op::M:
        return ndtM(std::move(formula));
    default:
        return formula.map([] (spot::formula child)
        {
            return toDiscretizedLtlFormula(std::move(child));
        });
    }
}

spot::formula ndtF(spot::formula&& formula)
{
    spot::formula child { toDiscretizedLtlFormula(formula[0]) };
    return U(top(), singAndAliveAnd(std::move(child)));
}

spot::formula ndtG(spot::formula&& formula)
{
    spot::formula child { toDiscretizedLtlFormula(formula[0]) };
    return Not(U(top(), singAndAliveAnd(Not(std::move(child)))));
}

spot::formula ndtU(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
    return U(child1, And({
                         std::move(child2),
                         singOr(std::move(child1)),
                         alive()
                     }));
}

spot::formula ndtR(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
    return Not(U(Not(child1), And({
                                  Not(std::move(child2)),
                                  singOrNot(std::move(child1)),
                                  alive()
                              })));
}

spot::formula ndtS(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) }; // p1
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) }; // p2

    // a | b
    return Or({

        // a := sing & X(alive & (p1 U (p2 & (p1 | sing) & alive)))
        And({
            sing(),
            X(And({
                alive(),
                U(child1, And({
                              child2,
                              singOr(child1),
                              alive()
                          }))}))}),

        // b := Not(sing) & (p1 U (p2 & (p1 | sing) & alive))
        And({
            notSing(),
            U(std::move(child1), And({
                          std::move(child2),
                          singOr(child1),
                          alive()
                      }))})
    });
}

spot::formula ndtM(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
    spot::formula child1AndChild2 { And({ child1, child2 }) };

    return And({
        U(top(), singAndAliveAnd(std::move(child1AndChild2))),

        Not(U(Not(child1), And({
                               Not(std::move(child1)),
                               singOrNot(std::move(child2)),
                               alive()
                            })))
    });
}

spot::formula ndtW(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };

    return Or({
        U(child1, And({
                      std::move(child2),
                      singOr(child1),
                      alive()
                  })),

        Not(U(top(), singAndAliveAnd(Not(std::move(child1)))))
    });
}