#pragma once

#include <spot_constants.h>

#include "PolyhedralSystem.h"
#include "reach.h"
#include "spot_utils.h"

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

inline std::pair<spot::formula, PowersetUniquePtr> stay(PolyhedralSystemConstSharedPtr polyhedralSystem)
{
    spot::formula stay { ap("stay") };
    PowersetUniquePtr stayInterpretation { std::make_unique<Powerset>(polyhedralSystem->spaceDimension(), PPL::EMPTY) };

    if (!polyhedralSystem->isOmnidirectionalFlow())
    {
        std::vector observables { polyhedralSystem->generateObservables() };

        for (Observable observable: observables)
        {
            PowersetConstSharedPtr observableInterpretation { observable.interpretation() };
            for (auto patch { observableInterpretation->begin() }; patch != observableInterpretation->end(); ++patch)
            {
                PolyUniquePtr patchCone { PPLUtils::cone(patch->pointset()) };
                PolyUniquePtr patchConeIntersectFlow { PPLUtils::intersect(*patchCone, polyhedralSystem->flow()) };
                if (!patchConeIntersectFlow->is_empty())
                {
                    stayInterpretation->add_disjunct(patch->pointset());
                }
            }
        }
    }

    return std::make_pair<spot::formula, PowersetUniquePtr>(std::move(stay), std::move(stayInterpretation));
}

inline spot::formula eventuallyBrinkAndLast(const spot::formula& formula)
{
    return And({ formula, F(And({ ap("brink"), ap("last") }))});
}

inline spot::formula eventuallyStayAndLast(const spot::formula& formula)
{
    return And({ formula, F(And({ ap("stay"), ap("last") }))});
}