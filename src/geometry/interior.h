#pragma once

#include "ppl_utils.h"

/*!
 *  \brief Computes the interior of a convex polyhedron \f$ P \f$ using its geometric definition.
 *
 * The interior of a convex polyhedron \f$ P \f$, denoted by \f$\mathit{int}(P)\f$, is defined as:
 * \f[
 * \mathit{int}(P) = P \, \setminus \, [\mathit{cl}(\top \, \setminus  \, P) \, \cap \, \mathit{cl}(P)]
 * \f]
 * where:
 * - \f$ \top \f$ is the universe (the entire space).
 * - \f$\mathit{cl}(X)\f$ denotes the topological closure of \f$ X \f$.
 *
 * If the interior is empty, the function returns an empty polyhedron.
 *
 * \param poly The input convex polyhedron \f$ P \f$.
 * \return A `PolyUniquePtr` representing the interior of \f$ P \f$.
 *
 * \note The result of this function is equal to the result of \ref interior.
 */
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

/*!
 *  \brief Computes the interior of a convex polyhedron \f$ P \f$.
 *
 * The interior of a polyhedron \f$ P \f$, denoted by \f$\mathit{int}(P)\f$, is obtained by replacing all non-strict inequalities
 * in the constraint system of \f$ P \f$ with their strict versions. Specifically:
 * - Non-strict inequalities of the form \f$ a \cdot x \geq b \f$ are replaced with \f$ a \cdot x > b \f$;
 * - Non-strict inequalities of the form \f$ a \cdot x \leq b \f$ are replaced with \f$ a \cdot x < b \f$;
 * - Strict inequalities and equalities are left unchanged.
 *
 * If the polyhedron \f$ P \f$ contains any equality constraints, its interior is empty.
 *
 *
 * \param poly The input convex polyhedron \f$ P \f$.
 * \return A `PolyUniquePtr` representing the interior of \f$ P \f$. If \f$ P \f$ contains equality constraints, returns an empty polyhedron.
 *
 * \note The result of this function is equal to the result of \ref interiorGeometric.
 *
 */
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