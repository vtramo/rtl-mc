#pragma once

#include "ppl_utils.h"

inline PolyUniquePtr interiorGeometric(const Poly& poly)
{
    Powerset universe { poly.space_dimension(), PPL::UNIVERSE };

    Powerset polyClosure { poly };
    polyClosure.topological_closure_assign();

    universe.difference_assign(polyClosure);
    universe.topological_closure_assign();
    universe.intersection_assign(polyClosure);

    Powerset result { poly };
    result.difference_assign(universe);

    if (result.is_empty()) return std::make_unique<Poly>(poly.space_dimension(), PPL::EMPTY);
    return std::make_unique<Poly>(result.begin()->pointset());
}

inline PolyUniquePtr interior(const Poly& poly)
{
    const PPL::Constraint_System& polyConstraintSystem { poly.constraints() };
    if (polyConstraintSystem.has_equalities())
    {
        return std::make_unique<Poly>(poly.space_dimension(), PPL::EMPTY);
    }

    PPL::Constraint_System strictConstraints {};
    strictConstraints.set_space_dimension(poly.space_dimension());

    for (auto constraint: polyConstraintSystem)
    {
        switch (constraint.type())
        {
        case PPL::Constraint::EQUALITY:
            break;
        case PPL::Constraint::STRICT_INEQUALITY:
            strictConstraints.insert(constraint);
            break;
        case PPL::Constraint::NONSTRICT_INEQUALITY:
            PPL::Linear_Expression expression { constraint.expression() };
            PPL::Constraint strictConstraint { expression > 0 };
            strictConstraints.insert(strictConstraint);
            break;
        }
    }

    return std::make_unique<Poly>(strictConstraints);
}