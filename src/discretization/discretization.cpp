#include "discretization.h"
#include "spot_constants.h"

using namespace SpotUtils;

spot::formula discretize(spot::formula&& formula)
{
    spot::formula discretizedLtlFormula { toDiscretizedLtlFormula(std::move(formula)) };
    return imposeSingOpenLastAndAliveProperty(std::move(discretizedLtlFormula));
}

spot::formula imposeSingOpenLastAndAliveProperty(spot::formula&& formula)
{
    return {
        And({
                std::move(formula),
                singOpenLastProperty(),
                alive(),
                aliveUntilGNotAlive(),
            })
    };
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
    return U(top(), And({ std::move(child), alive() }));
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
    spot::formula child1Copy { child1 };

    return U(std::move(child1Copy), And({
                                        std::move(child2),
                                        singOr(std::move(child1)),
                                        alive()
                                    }));
}

spot::formula ndtR(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
    spot::formula child1Copy { child1 };

    return Not(U(Not(std::move(child1Copy)), And({
                                                 Not(std::move(child2)),
                                                 singOrNot(std::move(child1)),
                                                 alive()
                                             })));
}

spot::formula ndtS(spot::formula&& formula)
{
    spot::formula p1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula p2 { toDiscretizedLtlFormula(formula[1]) };

    spot::formula x { And({ std::move(p2), singOr(p1), alive() }) }; // (p2 & (p1 | sing) & alive)
    spot::formula y { U(std::move(p1), std::move(x)) }; // (p1 U (p2 & (p1 | sing) & alive)

    spot::formula a { And({ sing(), X(And({ alive(), y }))}) };
    spot::formula b { And({ notSing(), std::move(y) }) };

    // a := sing & X(alive & (p1 U (p2 & (p1 | sing) & alive)))
    // b := Not(sing) & (p1 U (p2 & (p1 | sing) & alive))
    return Or({ std::move(a), std::move(b) });
}

spot::formula ndtM(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
    spot::formula child1AndChild2 { And({ child1, child2 }) };
    spot::formula child1Copy { child1 };

    return And({
        U(top(), And({ std::move(child1AndChild2), alive() })),

        Not(U(Not(std::move(child1)), And({
                                          Not(std::move(child1Copy)),
                                          singOrNot(std::move(child2)),
                                          alive()
                                      })))
    });
}

spot::formula ndtW(spot::formula&& formula)
{
    spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
    spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };

    spot::formula a { U(child1, And({ std::move(child2), singOr(child1), alive() })) };
    spot::formula b { Not(U(top(), singAndAliveAnd(Not(std::move(child1))))) };

    return Or({ std::move(a), std::move(b) });
}