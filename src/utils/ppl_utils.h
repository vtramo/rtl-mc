//
// Created by vincenzo on 16/12/24.
//

#ifndef PPL_UTILS_H
#define PPL_UTILS_H

#include <ppl.hh>
#include "ppl_aliases.h"

namespace PPLUtils {
    Poly& reflectionAffineImage(Poly& polyhedron);

    Poly poly(const std::initializer_list<PPL::Constraint>& constraints);

    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);
}

#endif //PPL_UTILS_H
