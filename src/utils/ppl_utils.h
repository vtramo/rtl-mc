//
// Created by vincenzo on 16/12/24.
//

#ifndef PPL_UTILS_H
#define PPL_UTILS_H

#include "ppl_aliases.h"

namespace PPLUtils {
    Poly& reflectionAffineImage(Poly& polyhedron);

    Poly poly(std::vector<PPL::Constraint> constraints);

    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);

    std::unique_ptr<Powerset> intersect(const Powerset& a, const Powerset& b);
    std::unique_ptr<Powerset> intersect(const Powerset& a, Powerset&& b);
    std::unique_ptr<Powerset> intersect(Powerset&& a, Powerset&& b);

    std::unique_ptr<Powerset> minus(const Powerset& a, const Powerset& b);
    std::unique_ptr<Powerset> minus(const Powerset& a, Powerset&& b);
    std::unique_ptr<Powerset> minus(Powerset&& a, const Powerset& b);
    std::unique_ptr<Powerset> minus(Powerset&& a, Powerset&& b);
}

#endif //PPL_UTILS_H
