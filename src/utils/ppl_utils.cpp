//
// Created by vincenzo on 16/12/24.
//

#include "ppl_utils.h"

#include <numeric>

namespace PPLUtils
{
    Poly& reflectionAffineImage(Poly& polyhedron)
    {
        const PPL::dimension_type spaceDimension { polyhedron.space_dimension() };

        for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim)
        {
            const PPL::Variable variable { dim };
            polyhedron.affine_image(variable, -variable);
        }

        return polyhedron;
    }

    Poly poly(std::vector<PPL::Constraint> constraints)
    {
        PPL::Constraint_System constraintSystem {};

        for (PPL::Constraint& constraint: constraints)
        {
            PPL::Constraint con {};
            con.m_swap(constraint);
            constraintSystem.insert(con);
        }

        return Poly { constraintSystem, PPL::Recycle_Input {} };
    }

    Powerset powerset(const std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra)
    {
        const size_t totalPolyhedra { polyhedra.size() };
        std::vector<Poly> polyhedraVector;
        polyhedraVector.reserve(totalPolyhedra);

        const PPL::dimension_type powersetDimension {
            std::transform_reduce(polyhedra.begin(), polyhedra.end(), PPL::dimension_type {},
                [](const PPL::dimension_type& dim1, const PPL::dimension_type& dim2) {
                    return std::max(dim1, dim2);
                },

                [&polyhedraVector](const auto& constraints)
                {
                    Poly polyhedron { poly(constraints) };
                    polyhedraVector.push_back(std::move(polyhedron));
                    return polyhedron.space_dimension();
                }
        )};

        Powerset powerset { powersetDimension, PPL::EMPTY };
        std::for_each(polyhedraVector.begin(), polyhedraVector.end(),
            [&](const auto& poly) { powerset.add_disjunct(poly); });

        return powerset;
    }

    std::unique_ptr<Powerset> intersect(const Powerset& a, const Powerset& b)
    {
        if (b.is_universe())
        {
            return std::make_unique<Powerset>(a);
        }

        if (a.is_universe())
        {
            return std::make_unique<Powerset>(b);
        }

        auto result { std::make_unique<Powerset>(a) };
        result->intersection_assign(b);
        return result;
    }

    std::unique_ptr<Powerset> intersect(const Powerset& a, Powerset&& b)
    {
        std::unique_ptr<Powerset> result { nullptr };

        if (b.is_universe())
        {
            return std::make_unique<Powerset>(a);
        }

        if (a.is_universe())
        {
            result = std::make_unique<Powerset>();
            result->m_swap(b);
            return result;
        }

        result = std::make_unique<Powerset>();
        result->m_swap(b);
        result->intersection_assign(a);
        return result;
    }

    std::unique_ptr<Powerset> intersect(Powerset&& a, Powerset&& b)
    {
        auto result { std::make_unique<Powerset>() };

        if (b.is_universe())
        {
            result->m_swap(a);
            return result;
        }

        if (a.is_universe())
        {
            result->m_swap(b);
            return result;
        }

        if (a.size() > b.size())
        {
            result->m_swap(a);
            result->intersection_assign(b);
        } else
        {
            result->m_swap(b);
            result->intersection_assign(a);
        }

        return result;
    }

    std::unique_ptr<Powerset> minus(const Powerset& a, const Powerset& b)
    {
        auto result { std::make_unique<Powerset>(a) };
        result->difference_assign(b);
        return result;
    }

    std::unique_ptr<Powerset> minus(Powerset&& a, Powerset&& b)
    {
        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }

    std::unique_ptr<Powerset> minus(Powerset&& a, const Powerset& b)
    {
        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }
}
