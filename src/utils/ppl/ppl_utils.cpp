#include "ppl_utils.h"

#include <numeric>

using PPL::IO_Operators::operator<<;

/*!
 * This function computes a hash value for the given `Poly` object based on its constraints. The hash value is
 * computed in a way that ensures consistency and minimizes collisions.
 */
std::size_t std::hash<Parma_Polyhedra_Library::NNC_Polyhedron>::operator()(const Poly& poly) const noexcept
{
    return poly.hash_code();
}

/*!
 * This function computes a hash value for the given `PPL::Pointset_Powerset<Poly>` object based on its constituent
 * polyhedra. The hash value is computed in a way that ensures consistency and minimizes collisions.
 */
std::size_t
std::hash<Parma_Polyhedra_Library::Pointset_Powerset<Parma_Polyhedra_Library::NNC_Polyhedron>>::operator()
(
    const PPL::Pointset_Powerset<Poly>& powerset
) const noexcept
{
    return powerset.hash_code();
}

std::size_t std::hash<Parma_Polyhedra_Library::Variable>::operator()
(
    const PPL::Variable& variable
) const noexcept
{
    return variable.space_dimension();
}

/*!
 * This struct provides an alternative hash function for `Poly` objects that first converts
 * the polyhedron to a string representation and then hashes the resulting string.
 */
std::size_t StringPolyHasher::operator()(const Poly& poly) const noexcept
{
    std::ostringstream iss;
    iss << poly;
    return std::hash<std::string>{}(iss.str());
}

/*!
 * The `PPLUtils` namespace contains functions for converting PPL objects to strings, performing operations
 * such as intersection, complement, and fusion on convex polyhedra and powersets, and creating specific types of
 * polyhedra (e.g., zero points or points defined by linear expressions). It also provides helper functions
 * for checking properties of constraints and variables.
 */
namespace PPLUtils {

    /*!
     * This function converts a `Poly` (polyhedron) into a human-readable string, showing its constraints.
     */
    std::string toString(const Poly& poly)
    {
        std::ostringstream iss { };
        iss << poly;
        return iss.str();
    }

    /*!
     * This function converts a `Powerset` (set of polyhedra) into a human-readable string, showing the constraints
     * of each polyhedron in the set.
     */
    std::string toString(const Powerset& powerset)
    {
        std::ostringstream iss { };
        iss << powerset;
        return iss.str();
    }

    /*!
     * This function constructs a `Poly` (polyhedron) from a given set of constraints.
     */
    Poly poly(std::vector<PPL::Constraint> && constraints) {
        PPL::Constraint_System constraintSystem {};

        for (PPL::Constraint & constraint : constraints) {
            PPL::Constraint con {};
            con.m_swap(constraint);
            constraintSystem.insert(con);
        }

        return Poly { constraintSystem, PPL::Recycle_Input {} };
    }

    /*!
     * This function constructs a `Poly` (polyhedron) from a given set of constraints and explicitly specifies
     * the dimension of the polyhedron.
     */
    Poly poly(std::vector<PPL::Constraint> && constraints, const PPL::dimension_type polyDimension) {
        PPL::Constraint_System constraintSystem {};
        constraintSystem.set_space_dimension(polyDimension);

        for (PPL::Constraint & constraint : constraints) {
            PPL::Constraint con {};
            con.m_swap(constraint);
            constraintSystem.insert(con);
        }

        return Poly { constraintSystem };
    }

    /*!
     * This function constructs a `Powerset` (set of polyhedra) from an initializer list of constraints,
     * where each inner list represents the constraints of a single polyhedron.
     */
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

    /*!
     * This function constructs a `Powerset` (set of polyhedra) from an initializer list of constraints,
     * where each inner list represents the constraints of a single polyhedron. The dimension of the powerset
     * is explicitly specified.
     */
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

    /*!
     * This function computes the fusion (union) of two powersets (`a` and `b`) and returns the result as a `PowersetUniquePtr`.
     */
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

    /*!
     * This function computes the fusion (union) of a powerset (`a`) and a polyhedron (`b`) and returns the result
     * as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr fusion(const Powerset& a, const Poly& b)
    {
        assert(a.space_dimension() == b.space_dimension());

        PowersetUniquePtr result { std::make_unique<Powerset>(a) };
        result->add_disjunct(b);
        return result;
    }

    /*!
     * This function computes the fusion (union) of two powersets (`a` and `b`) and returns the result as a `PowersetUniquePtr`.
     */
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

    /*!
     * This function computes the fusion (union) of all powersets in the given vector of shared pointers and returns the result
     * as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr> & powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(*powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            fusion(*result, *powersets[i]);
        return result;
    }

    /*!
     * This function computes the fusion (union) of two powersets (`a` and `b`) and modifies `a` to contain the result.
     */
    void fusion(Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        for (Powerset::const_iterator it { b.begin() }; it != b.end(); ++it)
            if (!it->pointset().is_empty())
                a.add_disjunct(it->pointset());
    }

    /*!
     * This function computes the fusion (union) of two polyhedra (`a` and `b`) and returns the result as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr fusion(const Poly& a, const Poly& b)
    {
        assert(a.space_dimension() == b.space_dimension());

        PowersetUniquePtr result { std::make_unique<Powerset>(a) };
        result->add_disjunct(b);
        return result;
    }

    /*!
     * This function computes the intersection of two polyhedra (`a` and `b`) and returns the result as a `PolyUniquePtr`.
     */
    PolyUniquePtr intersect(const Poly& a, const Poly& b)
    {
        assert(a.space_dimension() == b.space_dimension());

        if (b.is_universe()) {
            return std::make_unique<Poly>(a);
        }

        if (a.is_universe()) {
            return std::make_unique<Poly>(b);
        }

        auto result { std::make_unique<Poly>(a) };
        result->intersection_assign(b);
        return result;
    }

    /*!
     * This function computes the intersection of a polyhedron (`a`) and a movable polyhedron (`b`) and returns the result
     * as a `PolyUniquePtr`.
     */
    PolyUniquePtr intersect(const Poly& a, Poly&& b)
    {
        assert(a.space_dimension() == b.space_dimension());

        PolyUniquePtr result { nullptr };

        if (b.is_universe()) {
            return std::make_unique<Poly>(a);
        }

        if (a.is_universe()) {
            result = std::make_unique<Poly>();
            result->m_swap(b);
            return result;
        }

        result = std::make_unique<Poly>();
        result->m_swap(b);
        result->intersection_assign(a);
        return result;
    }

    /*!
     * This function computes the intersection of two powersets (`a` and `b`) and returns the result as a `PowersetUniquePtr`.
     */
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

    /*!
     * This function computes the intersection of a powerset (`a`) and a movable powerset (`b`) and returns the result
     * as a `PowersetUniquePtr`.
     */
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

    /*!
     * This function computes the intersection of a polyhedron (`a`) and a movable powerset (`b`) and returns the result
     * as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr intersect(const Poly& a, Powerset&& b)
    {
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
        result->intersection_assign(Powerset { a });
        return result;
    }

    /*!
     * This function computes the intersection of two movable powersets (`a` and `b`) and returns the result
     * as a `PowersetUniquePtr`.
     */
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

    /*!
     * This function computes the intersection of all powersets in the given vector and returns the result
     * as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr intersect(const std::vector<PowersetConstSharedPtr> & powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(*powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            result->intersection_assign(*powersets[i]);
        return result;
    }

    /*!
     * This function computes the intersection of all powersets in the given vector of shared pointers and returns the result
     * as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr intersect(const std::vector<Powerset>& powersets) {
        if (powersets.empty())
            return std::make_unique<Powerset>(Powerset { PPL::EMPTY });

        PowersetUniquePtr result { std::make_unique<Powerset>(powersets[0]) };
        for (int i = 1; i < static_cast<int>(powersets.size()); ++i)
            result->intersection_assign(powersets[i]);
        return result;
    }

    /*!
     * This function computes the complement of a powerset (`a`) and returns the result as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr complement(const Powerset & a) {
        PowersetUniquePtr result { std::make_unique<Powerset>(a.space_dimension() , PPL::UNIVERSE) };
        result->difference_assign(a);
        return result;
    }

    /*!
     * This function computes the set difference (`a - b`) and returns the result as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr minus(const Powerset & a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>(a) };
        result->difference_assign(b);
        return result;
    }

    /*!
     * This function computes the set difference (`a - b`) and returns the result as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr minus(Powerset && a, Powerset && b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }

    /*!
     * This function computes the set difference (`a - b`) and returns the result as a `PowersetUniquePtr`.
     */
    PowersetUniquePtr minus(Powerset && a, const Powerset & b) {
        assert(a.space_dimension() == b.space_dimension());

        auto result { std::make_unique<Powerset>() };
        result->m_swap(a);
        result->difference_assign(b);
        return result;
    }

    /*!
     * This function creates a `Poly` (polyhedron) representing the zero point (origin) in a space of the specified dimension.
     */
    Poly zeroPoint(const PPL::dimension_type spaceDimension) {
        PPL::Linear_Expression zeroPointLinearExpression { PPLUtils::zeroPointLinearExpression(spaceDimension) };
        return PPLUtils::point(zeroPointLinearExpression);
    }

    /*!
     * This function creates a `PPL::Linear_Expression` representing the zero point (origin) in a space of the specified dimension.
     */
    PPL::Linear_Expression zeroPointLinearExpression(const PPL::dimension_type spaceDimension)
    {
        PPL::Linear_Expression zeroPointLinearExpression {};
        for (PPL::dimension_type dim = 0; dim < spaceDimension; ++dim)
            zeroPointLinearExpression += 0 * PPL::Variable { dim };
        return zeroPointLinearExpression;
    }

    /*!
     * This function creates a `Poly` (polyhedron) representing a point defined by the given linear expression.
     */
    Poly point(PPL::Linear_Expression pointLinearExpression)
    {
        return Poly { PPL::Generator_System { PPL::point(pointLinearExpression) } };
    }

    /*!
     * This function checks if two `PPL::Variable` objects (`x` and `y`) belong to the same space dimension.
     */
    bool haveSameSpaceDimension(const PPL::Variable & x, const PPL::Variable & y) {
        return x.space_dimension() == y.space_dimension();
    }

    /*!
     * This function checks if the given `Powerset` contains the specified `Poly` object as one of its disjuncts.
     */
    bool containsDisjunct(const Powerset & powerset, const Poly & disjunct) {
        for (Powerset::const_iterator it { powerset.begin() }; it != powerset.end(); ++it)
            if (it->pointset() == disjunct)
                return true;
        return false;
    }

    /*!
     * This function removes all single-variable equality constraints of the form `x = 0` from the given `Poly` object
     * and returns the result as a `PolyUniquePtr`.
     */
    PolyUniquePtr removeSingleVariableZeroEqualityConstraints(const Poly& poly)
    {
        Poly copy { poly };
        const PPL::Constraint_System& polyConstraintSystem { poly.constraints() };
        if (!polyConstraintSystem.has_equalities())
        {
            return std::make_unique<Poly>(poly);
        }

        PPL::Constraint_System newConstraints {};
        newConstraints.set_space_dimension(poly.space_dimension());

        for (auto constraint: polyConstraintSystem)
        {
            if (isSingleVariableZeroEqualityConstraint(constraint))
            {
                continue;
            }

            newConstraints.insert(constraint);
        }

        return newConstraints.empty()
            ? std::make_unique<Poly>(poly.space_dimension(), PPL::EMPTY)
            : std::make_unique<Poly>(newConstraints);
    }

    /*!
     * This function checks if the given `PPL::Constraint` is a single-variable equality constraint of the form `x = 0`.
     */
    bool isSingleVariableZeroEqualityConstraint(const PPL::Constraint& constraint)
    {
        const auto& expression { constraint.expression() };
        if (!constraint.is_equality() || expression.inhomogeneous_term() != 0)
        {
            return false;
        }

        bool nonZeroCoefficientFound { false };
        for (PPL::dimension_type dim { 0 }; dim < constraint.space_dimension(); ++dim)
        {
            PPL::GMP_Integer varCoefficient { expression.coefficient(PPL::Variable { dim }) };
            if (varCoefficient != 0)
            {
                if (nonZeroCoefficientFound) return false;
                nonZeroCoefficientFound = true;
            }
        }

        return nonZeroCoefficientFound;
    }
}
