#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>

spot::formula imposeSingOpenLastAndAliveProperty(spot::formula&& formula);
spot::formula discretize(spot::formula&& formula);

#endif //DISCRETIZATION_H
