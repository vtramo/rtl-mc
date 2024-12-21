//
// Created by vincenzo on 16/12/24.
//

#include "ppl_utils.h"

namespace PPLUtils
{
  Poly& reflectionAffineImage(Poly& polyhedron)
  {
    const PPL::dimension_type spaceDimension { polyhedron.space_dimension() };

    for (PPL::dimension_type dim = 0; dim < spaceDimension; ++dim)
    {
      const PPL::Variable variable { dim };
      polyhedron.affine_image(variable, -variable);
    }

    return polyhedron;
  }

  Poly poly(const std::initializer_list<PPL::Constraint>& constraints)
  {
    PPL::Constraint_System constraintSystem {};
    for (const PPL::Constraint& constraint : constraints)
    {
      constraintSystem.insert(constraint);
    }
    return Poly { constraintSystem };
  }

  Powerset powerset(const std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra)
  {
    std::list<Poly> nncPolyhedra {  };
    PPL::dimension_type spaceDimension { };

    for (const std::initializer_list<PPL::Constraint>& polyhedron: polyhedra)
    {
      Poly nncPolyhedron { poly(polyhedron) };
      nncPolyhedra.push_back(nncPolyhedron);
      spaceDimension = std::max(spaceDimension, nncPolyhedron.space_dimension());
    }

    Powerset powerset { spaceDimension, PPL::EMPTY };
    for (const Poly& polyhedron: nncPolyhedra)
    {
      powerset.add_disjunct(polyhedron);
    }

    return powerset;
  }

  Powerset intersect(const Powerset& a, const Powerset& b)
  {
    if (b.is_universe())
    {
      return a;
    }

    if (a.is_universe())
    {
      return b;
    }

    Powerset aCopy { a };
    aCopy.intersection_assign(b);
    return aCopy;
  }

  Powerset minus(const Powerset& a, const Powerset& b)
  {
    Powerset aCopy { a };
    aCopy.difference_assign(b);
    return aCopy;
  }
}
