#include "discretization.h"
#include "spot_constants.h"

using namespace SpotUtils;

namespace
{
    spot::formula toDiscretizedLtlFormula(spot::formula&& formula);
    spot::formula dsctF(spot::formula&& formula);
    spot::formula dsctG(spot::formula&& formula);
    spot::formula dsctU(spot::formula&& formula);
    spot::formula dsctR(spot::formula&& formula);
    spot::formula dsctS(spot::formula&& formula);
    spot::formula dsctM(spot::formula&& formula);
    spot::formula dsctW(spot::formula&& formula);
}


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


namespace
{
    spot::formula toDiscretizedLtlFormula(spot::formula&& formula)
    {
        switch (formula.kind())
        {
        case spot::op::F:
            return dsctF(std::move(formula));
        case spot::op::G:
            return dsctG(std::move(formula));
        case spot::op::U:
            return dsctU(std::move(formula));
        case spot::op::S:
            return dsctS(std::move(formula));
        case spot::op::W:
            return dsctW(std::move(formula));
        case spot::op::M:
            return dsctM(std::move(formula));
        case spot::op::R:
            return dsctR(std::move(formula));
        default:
            return formula.map([] (spot::formula child)
            {
                return toDiscretizedLtlFormula(std::move(child));
            });
        }
    }

    spot::formula dsctF(spot::formula&& formula)
    {
        spot::formula child { toDiscretizedLtlFormula(formula[0]) };
        return F(And({ std::move(child), alive() }));
    }

    spot::formula dsctG(spot::formula&& formula)
    {
        spot::formula child { toDiscretizedLtlFormula(formula[0]) };
        return G(Or({ std::move(child), notAlive() }));
    }

    spot::formula dsctU(spot::formula&& formula)
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

    spot::formula dsctR(spot::formula&& formula)
    {
        spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
        spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
        spot::formula child1Copy { child1 };

        return R(std::move(child1Copy), Or({ std::move(child2), And({ std::move(child1), notSing() }), notAlive() }));
    }

    spot::formula dsctS(spot::formula&& formula)
    {
        spot::formula p1 { toDiscretizedLtlFormula(formula[0]) };
        spot::formula p2 { toDiscretizedLtlFormula(formula[1]) };

        spot::formula p1Up2 { dsctU(U(std::move(p1), std::move(p2))) };
        spot::formula a { And({ sing(), X(And({ alive(), p1Up2 }))}) };
        spot::formula b { And({ notSing(), std::move(p1Up2) }) };

        // a := sing & X(alive & (p1 U (p2 & (p1 | sing) & alive)))
        // b := !sing & (p1 U (p2 & (p1 | sing) & alive))
        return Or({ std::move(a), std::move(b) });
    }

    spot::formula dsctM(spot::formula&& formula)
    {
        spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
        spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };
        spot::formula child1AndChild2 { And({ child1, child2 }) };

        return And({
            dsctR(R(std::move(child1), std::move(child2))),
            dsctF(F(std::move(child1AndChild2)))
        });
    }

    spot::formula dsctW(spot::formula&& formula)
    {
        spot::formula child1 { toDiscretizedLtlFormula(formula[0]) };
        spot::formula child2 { toDiscretizedLtlFormula(formula[1]) };

        spot::formula a { dsctU(U(child1, std::move(child2))) };
        spot::formula b { G(Or({ std::move(child1), notAlive() })) };

        return Or({ std::move(a), std::move(b) });
    }
}