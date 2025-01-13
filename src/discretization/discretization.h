#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>

spot::formula toDiscretizedLtlFormula(spot::formula&& formula);
spot::formula imposeSingOpenLastFiniteProperty(spot::formula&& formula);

spot::formula toDiscretizedFormula(spot::formula&& formula);
spot::formula imposeSingOpenLastProperty(spot::formula&& formula);

#endif //DISCRETIZATION_H
