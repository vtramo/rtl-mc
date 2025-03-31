#pragma once

#include <spdlog/fmt/bundled/ranges.h>

#include "formula_constants.h"
#include "PolyhedralSystem.h"
#include "reach.h"
#include "spot_utils.h"
#include "cone.h"

using namespace SpotUtils;

enum class BrinkSemantics
{
    may,
    must
};

inline std::pair<spot::formula, PowersetUniquePtr> brinkMay(PolyhedralSystemConstSharedPtr polyhedralSystem)
{
    spot::formula brink { ap("brink") };
    const Powerset& invariant { polyhedralSystem->invariant() };
    PowersetUniquePtr invariantComplement { PPLUtils::complement(invariant) };
    PowersetUniquePtr brinkInterpretation { reach0(invariant, *invariantComplement, polyhedralSystem->preFlow()) };
    return std::make_pair<spot::formula, PowersetUniquePtr>(std::move(brink), std::move(brinkInterpretation));
}

inline std::pair<spot::formula, PowersetUniquePtr> brinkMust(PolyhedralSystemConstSharedPtr polyhedralSystem)
{
    spot::formula brink { ap("brink") };
    const Powerset& invariant { polyhedralSystem->invariant() };
    PowersetUniquePtr brinkInterpretation { PPLUtils::minus(invariant, *reachPlus(invariant, invariant, polyhedralSystem->preFlow())) };
    return std::make_pair<spot::formula, PowersetUniquePtr>(std::move(brink), std::move(brinkInterpretation));
}

inline std::pair<spot::formula, PowersetUniquePtr> stay(PolyhedralSystemSharedPtr polyhedralSystem)
{
    spot::formula stay { ap("stay") };

    if (polyhedralSystem->hasOmnidirectionalFlow())
    {
        return std::make_pair<spot::formula, PowersetUniquePtr>(
            std::move(stay),
            std::make_unique<Powerset>(polyhedralSystem->spaceDimension(), PPL::UNIVERSE)
        );
    }

    PowersetUniquePtr stayInterpretation { std::make_unique<Powerset>(polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    const std::vector<Observable>& observables { polyhedralSystem->getOrGenerateObservables() };
    for (Observable observable: observables)
    {
        PowersetConstSharedPtr observableInterpretation { observable.interpretation() };
        for (const auto& patch: *observableInterpretation)
        {
            PolyUniquePtr patchCone { characteristicCone(patch.pointset()) };
            PolyUniquePtr patchConeIntersectedWithFlow { PPLUtils::intersect(*patchCone, polyhedralSystem->flow()) };
            if (!patchConeIntersectedWithFlow->is_empty())
            {
                stayInterpretation->add_disjunct(patch.pointset());
            }
        }
    }
    return std::make_pair<spot::formula, PowersetUniquePtr>(std::move(stay), std::move(stayInterpretation));
}

inline spot::formula eventuallyBrinkAndLast(const spot::formula& formula)
{
    return andFormulae({ formula, F(andFormulae({ ap("brink"), ap("last") }))});
}

inline spot::formula eventuallyStayAndLast(const spot::formula& formula)
{
    return andFormulae({ formula, F(andFormulae({ ap("stay"), ap("last") }))});
}