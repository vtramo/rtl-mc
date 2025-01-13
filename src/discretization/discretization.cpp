#include "discretization.h"
#include "spot_constants.h"

using namespace SpotUtils;

namespace
{
    spot::formula dsctF(spot::formula&& formula);
    spot::formula dsctG(spot::formula&& formula);
    spot::formula dsctX(spot::formula&& formula);
    spot::formula dsctU(spot::formula&& formula);
    spot::formula dsctR(spot::formula&& formula);
    spot::formula dsctM(spot::formula&& formula);
    spot::formula dsctW(spot::formula&& formula);

    spot::formula dscF(spot::formula&& formula);
    spot::formula dscG(spot::formula&& formula);
    spot::formula dscX(spot::formula&& formula);
    spot::formula dscU(spot::formula&& formula);
    spot::formula dscR(spot::formula&& formula);
    spot::formula dscM(spot::formula&& formula);
    spot::formula dscW(spot::formula&& formula);
}

spot::formula imposeSingOpenLastFiniteProperty(spot::formula&& formula)
{
    return And({
                std::move(formula),
                singOpenLastPropertyFinite(),
                alive(),
                aliveUntilGNotAlive(),
            });

}

spot::formula imposeSingOpenLastProperty(spot::formula&& formula)
{
    return And({
                std::move(formula),
                singOpenLastProperty(),
             });
}

spot::formula toDiscretizedLtlFormula(spot::formula&& formula)
{
    switch (formula.kind())
    {
    case spot::op::X:
        return dsctX(std::move(formula));
    case spot::op::F:
        return dsctF(std::move(formula));
    case spot::op::G:
        return dsctG(std::move(formula));
    case spot::op::U:
        return dsctU(std::move(formula));
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

spot::formula toDiscretizedFormula(spot::formula&& formula)
{
    switch (formula.kind())
    {
    case spot::op::X:
        return dscX(std::move(formula));
    case spot::op::F:
        return dscF(std::move(formula));
    case spot::op::G:
        return dscG(std::move(formula));
    case spot::op::U:
        return dscU(std::move(formula));
    case spot::op::W:
        return dscW(std::move(formula));
    case spot::op::M:
        return dscM(std::move(formula));
    case spot::op::R:
        return dscR(std::move(formula));
    default:
        return formula.map([] (spot::formula child)
        {
            return toDiscretizedFormula(std::move(child));
        });
    }
}

namespace
{
    spot::formula dsctX(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretizedLtlFormula(formula[0]) };
        spot::formula next { X(And({ childDsct, alive() })) };
        return Or({
            And({ sing(), std::move(next) }),
            And({ notSing(), std::move(childDsct) })
        });
    }

    spot::formula dsctF(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretizedLtlFormula(formula[0]) };
        return F(And({ std::move(childDsct), alive() }));
    }

    spot::formula dsctG(spot::formula&& formula)
    {
        spot::formula childDsct { toDiscretizedLtlFormula(formula[0]) };
        return G(Or({ std::move(childDsct), notAlive() }));
    }

    spot::formula dsctU(spot::formula&& formula)
    {
        spot::formula child1Dsct { toDiscretizedLtlFormula(formula[0]) };
        spot::formula child2Dsct { toDiscretizedLtlFormula(formula[1]) };
        spot::formula child1DsctCopy { child1Dsct };

        return U(std::move(child1DsctCopy), And({
                                            std::move(child2Dsct),
                                            singOr(std::move(child1Dsct)),
                                            alive()
                                        }));
    }

    spot::formula dsctR(spot::formula&& formula)
    {
        spot::formula child1Dsct { toDiscretizedLtlFormula(formula[0]) };
        spot::formula child2Dsct { toDiscretizedLtlFormula(formula[1]) };
        spot::formula child1DsctCopy { child1Dsct };

        return R(std::move(child1DsctCopy), Or({ std::move(child2Dsct), And({ std::move(child1Dsct), notSing() }), notAlive() }));
    }

    spot::formula dsctM(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };
        spot::formula child1AndChild2 { And({ child1, child2 }) };

        return And({
            dsctR(R(std::move(child1), std::move(child2))),
            dsctF(F(std::move(child1AndChild2)))
        });
    }

    spot::formula dsctW(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula untilDsct { dsctU(U(child1, std::move(child2))) };
        spot::formula globallyDsct { dsctG(G(Or({ std::move(child1), notAlive() }))) };

        return Or({ std::move(untilDsct), std::move(globallyDsct) });
    }



    spot::formula dscX(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretizedFormula(formula[0]) };
        spot::formula strongNext { strongX(childDsc) };
        return Or({
            And({ sing(), strongNext }),
            And({ notSing(), std::move(childDsc) })
        });
    }

    spot::formula dscF(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretizedFormula(formula[0]) };
        return F(std::move(childDsc));
    }

    spot::formula dscG(spot::formula&& formula)
    {
        spot::formula childDsc { toDiscretizedFormula(formula[0]) };
        return G(std::move(childDsc));
    }

    spot::formula dscU(spot::formula&& formula)
    {
        spot::formula child1Dsc { toDiscretizedFormula(formula[0]) };
        spot::formula child2Dsc { toDiscretizedFormula(formula[1]) };

        spot::formula child2AndSingOrChild1 { And({ std::move(child2Dsc), singOr(child1Dsc) }) };
        return U(std::move(child1Dsc), std::move(child2AndSingOrChild1));
    }

    spot::formula dscW(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula until { dscU(U(child1, std::move(child2))) };
        spot::formula globally { dscG(G(std::move(child1))) };
        return Or({ std::move(until), std::move(globally) });
    }

    spot::formula dscR(spot::formula&& formula)
    {
        spot::formula child1Dsc { toDiscretizedFormula(formula[0]) };
        spot::formula child2Dsc { toDiscretizedFormula(formula[1]) };

        spot::formula child1AndNotSing { And({ child1Dsc, notSing() }) };
        return R(std::move(child1Dsc), Or({ std::move(child2Dsc), std::move(child1AndNotSing) }));
    }

    spot::formula dscM(spot::formula&& formula)
    {
        spot::formula child1 { formula[0] };
        spot::formula child2 { formula[1] };

        spot::formula eventually { dscF(F(And({ child1, child2 }))) };
        spot::formula release { dscR(R(std::move(child1), std::move(child2))) };
        return And({ std::move(eventually), std::move(release) });
    }
}