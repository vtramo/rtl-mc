#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>

spot::formula imposeSingOpenLastProperty(spot::formula&& formula);
spot::formula discretize(spot::formula&& formula);

spot::formula toDiscretizedLtlFormula(spot::formula&& formula);
spot::formula ndtF(spot::formula&& formula);
spot::formula ndtG(spot::formula&& formula);
spot::formula ndtU(spot::formula&& formula);
spot::formula ndtR(spot::formula&& formula);
spot::formula ndtS(spot::formula&& formula);
spot::formula ndtM(spot::formula&& formula);
spot::formula ndtW(spot::formula&& formula);

#endif //DISCRETIZATION_H
