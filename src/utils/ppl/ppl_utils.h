#pragma once
#include "ppl_aliases.h"

template<> struct std::hash<Poly>
{
    std::size_t operator() (const Poly& poly) const noexcept;
};

template<> struct std::hash<PPL::Pointset_Powerset<Poly>>
{
    std::size_t operator() (const PPL::Pointset_Powerset<Poly>& powerset) const noexcept;
};

namespace PPLUtils {
    typedef std::pair<Poly, Poly> ReachPair;
    typedef std::vector<ReachPair> ReachPairs;

    std::string toString(const Poly& poly);
    std::string toString(const Powerset& powerset);

    Poly& reflectionAffineImage(Poly& polyhedron);
    Powerset& reflectionAffineImage(Powerset& powerset);

    Poly poly(std::vector<PPL::Constraint>&& constraints);
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra, PPL::dimension_type powersetDimension);
    Poly poly(std::vector<PPL::Constraint>&& constraints, PPL::dimension_type polyDimension);

    PolyUniquePtr intersect(const Poly& a, const Poly& b);
    PolyUniquePtr intersect(const Poly& a, Poly&& b);
    PowersetUniquePtr intersect(const Powerset& a, const Powerset& b);
    PowersetUniquePtr intersect(const Powerset& a, Powerset&& b);
    PowersetUniquePtr intersect(Powerset&& a, Powerset&& b);
    PowersetUniquePtr intersect(const std::vector<Powerset>& powersets);
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
    PowersetUniquePtr fusion(const Poly& a, const Poly& b);

    Poly zeroPoint(PPL::dimension_type spaceDimension);
    bool haveSameSpaceDimension(const PPL::Variable& x, const PPL::Variable& y);

    bool containsDisjunct(const Powerset& powerset, const Poly& disjunct);
    PolyUniquePtr interior(const Poly& poly);
}
