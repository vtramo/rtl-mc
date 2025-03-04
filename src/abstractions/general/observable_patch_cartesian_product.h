#pragma once

#include "ObservablePatch.h"
#include "Observable.h"

std::vector<std::vector<ObservablePatch>> observablePatchesCartesianProduct(const std::vector<Observable>& observables);

void observablePatchesCartesianProduct(
    const std::vector<Observable>& observables,
    std::vector<ObservablePatch>& combination,
    std::vector<std::vector<ObservablePatch>>& result,
    size_t depth
);

