#ifndef PPL_UTILS_H
#define PPL_UTILS_H

#include "ppl_aliases.h"

namespace PPLUtils {
    typedef std::vector<std::pair<Poly, Poly>> ReachPairs;

    Poly& reflectionAffineImage(Poly& polyhedron);
    Poly poly(std::vector<PPL::Constraint>&& constraints);
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);

    PowersetUniquePtr intersect(const Powerset& a, const Powerset& b);
    PowersetUniquePtr intersect(const Powerset& a, Powerset&& b);
    PowersetUniquePtr intersect(Powerset&& a, Powerset&& b);

    PowersetUniquePtr minus(const Powerset& a, const Powerset& b);
    PowersetUniquePtr minus(const Powerset& a, Powerset&& b);
    PowersetUniquePtr minus(Powerset&& a, const Powerset& b);
    PowersetUniquePtr minus(Powerset&& a, Powerset&& b);

    PowersetUniquePtr fusion(const Powerset &a, const Powerset &b);
    void fusion(Powerset &a, const Powerset &b);

    bool haveSameSpaceDimension(const PPL::Variable& x, const PPL::Variable& y);
}

#endif //PPL_UTILS_H
