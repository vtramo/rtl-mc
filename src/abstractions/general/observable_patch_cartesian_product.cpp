#include "observable_patch_cartesian_product.h"

std::vector<std::vector<ObservablePatch>> observablePatchesCartesianProduct(const std::vector<Observable>& observables)
{
    if (observables.empty())
        return {};

    std::vector<ObservablePatch> combination {};
    combination.reserve(observables.size());
    std::vector<std::vector<ObservablePatch>> result {};
    result.reserve(observables.size() * 3);

    observablePatchesCartesianProduct(observables, combination, result, 0);

    return result;
}

void observablePatchesCartesianProduct(
    const std::vector<Observable>& observables,
    std::vector<ObservablePatch>& combination,
    std::vector<std::vector<ObservablePatch>>& result,
    const size_t depth
)
{
    if (depth == observables.size())
    {
        result.push_back(combination);
        return;
    }

    const Observable& observable { observables[depth] };
    for (auto it { observable.begin() }; it != observable.end(); ++it)
    {
        const Poly& patch { it->pointset() };
        ObservablePatch observablePatch { observable, patch };
        combination.push_back(observablePatch);
        observablePatchesCartesianProduct(observables, combination, result, depth + 1);
        combination.pop_back();
    }
}
