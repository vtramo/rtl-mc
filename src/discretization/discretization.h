#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <spot/tl/formula.hh>
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"

DiscreteLtlFormula discretizeToLtl(spot::formula&& formula);
DiscreteFiniteLtlFormula discretize(spot::formula&& formula);

#endif //DISCRETIZATION_H
