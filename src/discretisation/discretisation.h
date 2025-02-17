#pragma once
#include <spot/tl/formula.hh>

spot::formula toDiscretisedLtlFormula(spot::formula&& formula);
spot::formula imposeSingOpenLastFiniteProperty(spot::formula&& formula);

spot::formula toDiscretisedFormula(spot::formula&& formula);
spot::formula imposeSingOpenLastProperty(spot::formula&& formula);