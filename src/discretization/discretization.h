#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>
#include "DiscreteLtlFormula.h"

spot::formula imposeSingOpenLastAndAliveProperty(spot::formula&& formula);
DiscreteLtlFormula discretize(spot::formula&& formula);

#endif //DISCRETIZATION_H
