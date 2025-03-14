#pragma once
#include <spot/tl/formula.hh>

spot::formula toDiscretisedLtlFormula(spot::formula&& formula);
spot::formula applyFiniteAlternationSingOpenObservablesOneStep(spot::formula&& formula);

spot::formula toDiscretisedFormula(spot::formula&& formula);
spot::formula applyFiniteAlternationSingOpenObservables(spot::formula&& formula);
spot::formula applyAlternationSingOpenObservables(spot::formula&& formula);