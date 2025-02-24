#include "ppl_utils.h"

#include <numeric>

using PPL::IO_Operators::operator<<;

namespace PPLUtils {
    Poly & reflectionAffineImage(Poly & polyhedron) {
        const PPL::dimension_type spaceDimension { polyhedron.space_dimension() };

        for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
            const PPL::Variable variable { dim };
            polyhedron.affine_image(variable, -variable);
        }

        assert(polyhedron.space_dimension() == spaceDimension);

        return polyhedron;
    }

    Powerset & reflectionAffineImage(Powerset & powerset) {
        const PPL::dimension_type spaceDimension { powerset.space_dimension() };

        for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
            const PPL::Variable variable { dim };
            powerset.affine_image(variable, -variable);
        }

        assert(powerset.space_dimension() == spaceDimension);

        return powerset;
    }

    Poly poly(std::vector<PPL::Constraint> && constraints) {
        PPL::Constraint_System constraintSystem {};

        for (PPL::Constraint & constraint : constraints) {
            PPL::Constraint con {};
            con.m_swap(constraint);
            constraintSystem.insert(con);
        }

        return Poly { constraintSystem, PPL::Recycle_Input {} };
    }

    Poly poly(std::vector<PPL::Constraint> && constraints, const PPL::dimension_type polyDimension) {
        PPL::Constraint_System constraintSystem {};

        for (PPL::Constraint & constraint : constraints) {
            PPL::Constraint con {};
            con.m_swap(constraint);
            constraintSystem.insert(con);
        }

        Poly poly { polyDimension, PPL::EMPTY };
        poly.add_constraints(constraintSystem);
        return poly;
    }

    Powerset powerset(const std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra) {
        const size_t totalPolyhedra { polyhedra.size() };
        std::vector<Poly> polyhedraVector;
        polyhedraVector.reserve(totalPolyhedra);

        const PPL::dimension_type powersetDimension {
            std::transform_reduce(polyhedra.begin(), polyhedra.end(), PPL::dimension_type {},
                [](const PPL::dimension_type & dim1, const PPL::dimension_type & dim2) {
                    return std::max(dim1, dim2);
                },

                [&polyhedraVector](const auto & constraints) {
                    Poly polyhedron { poly(constraints) };
                    polyhedraVector.push_back(std::move(polyhedron));
                    return polyhedron.space_dimension();
                }
        ) };

        Powerset powerset { powersetDimension, PPL::EMPTY };
        std::for_each(polyhedraVector.begin(), polyhedraVector.end(),
            [&](const auto & poly) { powerset.add_disjunct(poly); });

        return powerset;
    }

    Powerset powerset(const std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra, const PPL::dimension_type powersetDimension) {
        const size_t totalPolyhedra { polyhedra.size() };
        std::vector<Poly> polyhedraVector;
        polyhedraVector.reserve(totalPolyhedra);

        for (const auto & constraints : polyhedra) {
            Poly polyhedron { poly(constraints, powersetDimension) };
            polyhedraVector.push_back(std::move(polyhedron));
        }

        Powerset powerset { powersetDimension, PPL::EMPTY };
        std::for_each(polyhedraVector.begin(), polyhedraVector.end(),
            [&](const auto & poly) { powerset.add_disjunct(poly); });

        return powerset;
    }

    PowersetUniquePtr fusion(const Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        PowersetUniquePtr result { std::make_unique<Powerset>(a.space_dimension(), PPL::EMPTY) };

        for (Powerset::const_iterator it { a.begin() }; it != a.end(); ++it)
            if (!it->pointset().is_empty())
                result->add_disjunct(it->pointset());

        for (Powerset::const_iterator it { b.begin() }; it != b.end(); ++it)
            if (!it->pointset().is_empty())
                result->add_disjunct(it->pointset());

        return result;
    }

    PowersetUniquePtr fusion(const PowersetConstUniquePtr & a, const PowersetConstUniquePtr & b) {
        assert(a->space_dimension() == b->space_dimension());

        PowersetUniquePtr result { std::make_unique<Powerset>(a->space_dimension(), PPL::EMPTY) };

        for (Powerset::const_iterator it { a->begin() }; it != a->end(); ++it)
            if (!it->pointset().is_empty())
                result->add_disjunct(it->pointset());

        for (Powerset::const_iterator it { b->begin() }; it != b->end(); ++it)
            if (!it->pointset().is_empty())
                result->add_disjunct(it->pointset());

        return result;
    }

    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr> & powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(*powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            fusion(*result, *powersets[i]);
        return result;
    }

    void fusion(Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        for (Powerset::const_iterator it { b.begin() }; it != b.end(); ++it)
            if (!it->pointset().is_empty())
                a.add_disjunct(it->pointset());
    }

    PowersetUniquePtr intersect(const Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        if (b.is_universe()) {
            return std::make_unique<Powerset>(a);
        }

        if (a.is_universe()) {
            return std::make_unique<Powerset>(b);
        }

        auto result { std::make_unique<Powerset>(a) };
        result->intersection_assign(b);
        return result;
    }

    PowersetUniquePtr intersect(const Powerset & a, Powerset && b) {
        assert(a.space_dimension() == b.space_dimension());

        PowersetUniquePtr result { nullptr };

        if (b.is_universe()) {
            return std::make_unique<Powerset>(a);
        }

        if (a.is_universe()) {
            result = std::make_unique<Powerset>();
            result->m_swap(b);
            return result;
        }

        result = std::make_unique<Powerset>();
        result->m_swap(b);
        result->intersection_assign(a);
        return result;
    }

    PowersetUniquePtr intersect(Powerset && a, Powerset && b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>() };

        if (b.is_universe()) {
            result->m_swap(a);
            return result;
        }

        if (a.is_universe()) {
            result->m_swap(b);
            return result;
        }

        if (a.size() > b.size()) {
            result->m_swap(a);
            result->intersection_assign(b);
        } else {
            result->m_swap(b);
            result->intersection_assign(a);
        }

        return result;
    }

    PowersetUniquePtr intersect(const std::vector<PowersetConstSharedPtr> & powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(*powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            result->intersection_assign(*powersets[i]);
        return result;
    }

    PowersetUniquePtr intersect(std::vector<Powerset> & powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            result->intersection_assign(powersets[i]);
        return result;
    }

    PowersetUniquePtr complement(const Powerset & a) {
        PowersetUniquePtr result { std::make_unique<Powerset>(a.space_dimension() , PPL::UNIVERSE) };
        result->difference_assign(a);
        return result;
    }

    PowersetUniquePtr minus(const Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>(a) };
        result->difference_assign(b);
        return result;
    }

    PowersetUniquePtr minus(Powerset && a, Powerset && b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }

    PowersetUniquePtr minus(Powerset && a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }

    Poly zeroPoint(const PPL::dimension_type spaceDimension) {
        PPL::Linear_Expression zeroPointLinearExpression {};
        for (PPL::dimension_type dim = 0; dim < spaceDimension; ++dim)
            zeroPointLinearExpression += 0 * PPL::Variable { dim };
        return Poly { PPL::Generator_System { PPL::point(zeroPointLinearExpression) } };
    }

    bool haveSameSpaceDimension(const PPL::Variable & x, const PPL::Variable & y) {
        return x.space_dimension() == y.space_dimension();
    }

    bool containsDisjunct(const Powerset & powerset, const Poly & disjunct) {
        for (Powerset::const_iterator it { powerset.begin() }; it != powerset.end(); ++it)
            if (it->pointset() == disjunct)
                return true;
        return false;
    }

    bool areAdjacent(const Powerset& p, const Powerset& q)
    {
        Powerset closureP { p };
        closureP.topological_closure_assign();

        Powerset closureQ { q };
        closureQ.topological_closure_assign();

        PowersetUniquePtr pIntersectClosureQ { intersect(p, std::move(closureQ)) };
        PowersetUniquePtr qIntersectClosureP { intersect(q, std::move(closureP)) };
        fusion(*pIntersectClosureQ, *qIntersectClosureP);

        return !pIntersectClosureQ->is_empty();
    }
}
