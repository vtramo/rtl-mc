#pragma once
#include "ppl_aliases.h"

namespace PPLUtils {
    typedef std::pair<Poly, Poly> ReachPair;
    typedef std::vector<ReachPair> ReachPairs;

    Poly& reflectionAffineImage(Poly& polyhedron);
    Powerset& reflectionAffineImage(Powerset& polyhedron);
    Poly poly(std::vector<PPL::Constraint>&& constraints);
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra, PPL::dimension_type powersetDimension);
    Poly poly(std::vector<PPL::Constraint>&& constraints, PPL::dimension_type polyDimension);

    PowersetUniquePtr intersect(const Powerset& a, const Powerset& b);
    PowersetUniquePtr intersect(const Powerset& a, Powerset&& b);
    PowersetUniquePtr intersect(Powerset&& a, Powerset&& b);
    PowersetUniquePtr intersect(std::vector<Powerset>& powersets);
    PowersetUniquePtr intersect(const std::vector<PowersetConstSharedPtr>& powersets);

    PowersetUniquePtr complement(const Powerset& a);

    PowersetUniquePtr minus(const Powerset& a, const Powerset& b);
    PowersetUniquePtr minus(const Powerset& a, Powerset&& b);
    PowersetUniquePtr minus(Powerset&& a, const Powerset& b);
    PowersetUniquePtr minus(Powerset&& a, Powerset&& b);

    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr>& powersets);
    PowersetUniquePtr fusion(const PowersetConstUniquePtr& a, const PowersetConstUniquePtr& b);
    PowersetUniquePtr fusion(const Powerset& a, const Powerset& b);
    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr>& powersets);
    void fusion(Powerset& a, const Powerset& b);

    Poly zeroPoint(PPL::dimension_type spaceDimension);
    bool haveSameSpaceDimension(const PPL::Variable& x, const PPL::Variable& y);

    bool containsDisjunct(const Powerset& powerset, const Poly& disjunct);
    PowersetUniquePtr border(const Powerset& p, const Powerset& q);
    bool areAdjacent(const Powerset& p, const Powerset& q);
    PolyUniquePtr interior(const Poly& poly);
}
