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

  Poly nnc(const std::initializer_list<PPL::Constraint>& constraints)
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
    std::vector<Poly> nncPolyhedra { };
    PPL::dimension_type spaceDimension { };

    for (const std::initializer_list<PPL::Constraint>& polyhedron: polyhedra)
    {
      Poly nncPolyhedron { nnc(polyhedron) };
      nncPolyhedra.push_back(nncPolyhedron);
      spaceDimension = std::max(spaceDimension, nncPolyhedron.space_dimension());
    }

    Powerset powerset { spaceDimension };

    for (auto it = nncPolyhedra.rbegin(); it != nncPolyhedra.rend(); ++it)
    {
      powerset.add_disjunct(*it);
    }

    return powerset;
  }
}