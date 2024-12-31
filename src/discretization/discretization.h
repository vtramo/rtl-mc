#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>

spot::formula imposeSingOpenLastProperty(spot::formula&& formula);
spot::formula discretize(spot::formula&& formula);

static spot::formula toDiscretizedLtlFormula(spot::formula&& formula);
static spot::formula ndtF(spot::formula&& formula);
static spot::formula ndtG(spot::formula&& formula);
static spot::formula ndtU(spot::formula&& formula);
static spot::formula ndtR(spot::formula&& formula);
static spot::formula ndtS(spot::formula&& formula);
static spot::formula ndtM(spot::formula&& formula);
static spot::formula ndtW(spot::formula&& formula);

#endif //DISCRETIZATION_H
