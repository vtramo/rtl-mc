#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>

spot::formula imposeSingOpenLastAndAliveProperty(spot::formula&& formula);
spot::formula discretize(spot::formula&& formula);

spot::formula toDiscretizedLtlFormula(spot::formula&& formula);
spot::formula dsctF(spot::formula&& formula);
spot::formula dsctG(spot::formula&& formula);
spot::formula dsctU(spot::formula&& formula);
spot::formula dsctR(spot::formula&& formula);
spot::formula dsctS(spot::formula&& formula);
spot::formula dsctM(spot::formula&& formula);
spot::formula dsctW(spot::formula&& formula);

#endif //DISCRETIZATION_H
