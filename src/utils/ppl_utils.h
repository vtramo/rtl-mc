//
// Created by vincenzo on 16/12/24.
//

#ifndef PPL_UTILS_H
#define PPL_UTILS_H

#include <ppl.hh>

namespace PPL = Parma_Polyhedra_Library;

namespace PPLUtils {
    PPL::NNC_Polyhedron& reflectionAffineImage(PPL::NNC_Polyhedron& polyhedron);

    PPL::NNC_Polyhedron nnc(const std::initializer_list<PPL::Constraint>& constraints);

    PPL::Pointset_Powerset<PPL::NNC_Polyhedron> powerset(const std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);
}

#endif //PPL_UTILS_H
