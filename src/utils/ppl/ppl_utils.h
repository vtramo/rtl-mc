#pragma once
#include "ppl_aliases.h"

/*!
 * \brief Specialization of `std::hash` for the `Poly` type (NNC_Polyhedron).
 *
 * This struct provides a hash function for `Poly` objects, allowing them to be used as keys in hash-based containers
 * such as `std::unordered_map` or `std::unordered_set`.
 */
template<> struct std::hash<Poly>
{
    /*!
     * \brief Computes the hash value for a `Poly` object.
     * \param poly The `Poly` object to hash.
     * \return A `std::size_t` value representing the hash of the `Poly` object.
     */
    std::size_t operator() (const Poly& poly) const noexcept;
};

/*!
 * \brief Specialization of `std::hash` for the `PPL::Pointset_Powerset<Poly>` type.
 *
 * This struct provides a hash function for `PPL::Pointset_Powerset<Poly>` objects, allowing them to be used as keys
 * in hash-based containers such as `std::unordered_map` or `std::unordered_set`.
 */
template<> struct std::hash<PPL::Pointset_Powerset<Poly>>
{
    /*!
     * \brief Computes the hash value for a `PPL::Pointset_Powerset<Poly>` object.
     * \param powerset The `PPL::Pointset_Powerset<Poly>` object to hash.
     * \return A `std::size_t` value representing the hash of the powerset.
     */
    std::size_t operator() (const PPL::Pointset_Powerset<Poly>& powerset) const noexcept;
};


/*!
 * \brief Specialization of `std::hash` for the `PPL::Variable` type.
 */
template<> struct std::hash<PPL::Variable>
{
    /*!
     * \brief Computes the hash value for a `PPL::Variable` object.
     * \param variable The `PPL::Variable` object to hash.
     * \return A `std::size_t` value representing the hash of the variable.
     */
    std::size_t operator() (const PPL::Variable& variable) const noexcept;
};

struct PPLVariableEqualTo {
    bool operator()(const PPL::Variable& lhs, const PPL::Variable& rhs) const {
        return lhs.id() == rhs.id();
    }
};

/*!
 *  \brief A namespace providing utility functions for working with Parma Polyhedra Library (PPL) objects.
 */
namespace PPLUtils {
    /*!
     * \brief Converts a `Poly` object to a string representation.
     * \param poly The `Poly` object to convert.
     * \return A string representation of the `Poly`.
     */
    std::string toString(const Poly& poly);

    /*!
     * \brief Converts a `Powerset` object to a string representation.
     * \param powerset The `Powerset` object to convert.
     * \return A string representation of the `Powerset`.
     */
    std::string toString(const Powerset& powerset);

    /*!
     * \brief Creates a `Poly` object from a vector of constraints.
     * \param constraints A vector of `PPL::Constraint` objects defining the polyhedron.
     * \return A `Poly` object representing the polyhedron.
     */
    Poly poly(std::vector<PPL::Constraint>&& constraints);

    /*!
     * \brief Creates a `Powerset` object from an initializer list of polyhedra.
     * \param polyhedra An initializer list of initializer lists of `PPL::Constraint` objects.
     * \return A `Powerset` object representing the set of polyhedra.
     */
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra);

    /*!
     * \brief Creates a `Powerset` object from an initializer list of polyhedra with a specified dimension.
     * \param polyhedra An initializer list of initializer lists of `PPL::Constraint` objects.
     * \param powersetDimension The dimension of the powerset.
     * \return A `Powerset` object representing the set of polyhedra.
     */
    Powerset powerset(std::initializer_list<std::initializer_list<PPL::Constraint>> polyhedra, PPL::dimension_type powersetDimension);

    /*!
     * \brief Creates a `Poly` object from a vector of constraints with a specified dimension.
     * \param constraints A vector of `PPL::Constraint` objects defining the polyhedron.
     * \param polyDimension The dimension of the polyhedron.
     * \return A `Poly` object representing the polyhedron.
     */
    Poly poly(std::vector<PPL::Constraint>&& constraints, PPL::dimension_type polyDimension);

    /*!
     * \brief Computes the intersection of two `Poly` objects.
     * \param a The first `Poly` object.
     * \param b The second `Poly` object.
     * \return A `PolyUniquePtr` representing the intersection of `a` and `b`.
     */
    PolyUniquePtr intersect(const Poly& a, const Poly& b);

    /*!
     * \brief Computes the intersection of a `Poly` object and a movable `Poly` object.
     * \param a The first `Poly` object.
     * \param b The second `Poly` object (movable).
     * \return A `PolyUniquePtr` representing the intersection of `a` and `b`.
     */
    PolyUniquePtr intersect(const Poly& a, Poly&& b);

    /*!
     * \brief Computes the intersection of two `Powerset` objects.
     * \param a The first `Powerset` object.
     * \param b The second `Powerset` object.
     * \return A `PowersetUniquePtr` representing the intersection of `a` and `b`.
     */
    PowersetUniquePtr intersect(const Powerset& a, const Powerset& b);

    /*!
     * \brief Computes the intersection of a `Powerset` object and a movable `Powerset` object.
     * \param a The first `Powerset` object.
     * \param b The second `Powerset` object (movable).
     * \return A `PowersetUniquePtr` representing the intersection of `a` and `b`.
     */
    PowersetUniquePtr intersect(const Powerset& a, Powerset&& b);

    /*!
     * \brief Computes the intersection of a `Poly` object and a movable `Powerset` object.
     * \param a The `Poly` object.
     * \param b The `Powerset` object (movable).
     * \return A `PowersetUniquePtr` representing the intersection of `a` and `b`.
     */
    PowersetUniquePtr intersect(const Poly& a, Powerset&& b);

    /*!
     * \brief Computes the intersection of two movable `Powerset` objects.
     * \param a The first `Powerset` object (movable).
     * \param b The second `Powerset` object (movable).
     * \return A `PowersetUniquePtr` representing the intersection of `a` and `b`.
     */
    PowersetUniquePtr intersect(Powerset&& a, Powerset&& b);

    /*!
     * \brief Computes the intersection of a vector of `Powerset` objects.
     * \param powersets A vector of `Powerset` objects.
     * \return A `PowersetUniquePtr` representing the intersection of all powersets.
     */
    PowersetUniquePtr intersect(const std::vector<Powerset>& powersets);

    /*!
     * \brief Computes the intersection of a vector of `PowersetConstSharedPtr` objects.
     * \param powersets A vector of `PowersetConstSharedPtr` objects.
     * \return A `PowersetUniquePtr` representing the intersection of all powersets.
     */
    PowersetUniquePtr intersect(const std::vector<PowersetConstSharedPtr>& powersets);

    /*!
     * \brief Computes the complement of a `Powerset` object.
     * \param a The `Powerset` object.
     * \return A `PowersetUniquePtr` representing the complement of `a`.
     */
    PowersetUniquePtr complement(const Powerset& a);

    /*!
     * \brief Computes the set difference between two `Powerset` objects.
     * \param a The first `Powerset` object.
     * \param b The second `Powerset` object.
     * \return A `PowersetUniquePtr` representing the set difference `a - b`.
     */
    PowersetUniquePtr minus(const Powerset& a, const Powerset& b);

    /*!
     * \brief Computes the set difference between a `Powerset` object and a movable `Powerset` object.
     * \param a The first `Powerset` object.
     * \param b The second `Powerset` object (movable).
     * \return A `PowersetUniquePtr` representing the set difference `a - b`.
     */
    PowersetUniquePtr minus(const Powerset& a, Powerset&& b);

    /*!
     * \brief Computes the set difference between a movable `Powerset` object and a `Powerset` object.
     * \param a The first `Powerset` object (movable).
     * \param b The second `Powerset` object.
     * \return A `PowersetUniquePtr` representing the set difference `a - b`.
     */
    PowersetUniquePtr minus(Powerset&& a, const Powerset& b);

    /*!
     * \brief Computes the set difference between two movable `Powerset` objects.
     * \param a The first `Powerset` object (movable).
     * \param b The second `Powerset` object (movable).
     * \return A `PowersetUniquePtr` representing the set difference `a - b`.
     */
    PowersetUniquePtr minus(Powerset&& a, Powerset&& b);

    /*!
     * \brief Computes the fusion of a vector of `PowersetConstSharedPtr` objects.
     * \param powersets A vector of `PowersetConstSharedPtr` objects.
     * \return A `PowersetUniquePtr` representing the fusion of all powersets.
     */
    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr>& powersets);

    /*!
     * \brief Computes the fusion of two `PowersetConstUniquePtr` objects.
     * \param a The first `PowersetConstUniquePtr` object.
     * \param b The second `PowersetConstUniquePtr` object.
     * \return A `PowersetUniquePtr` representing the fusion of `a` and `b`.
     */
    PowersetUniquePtr fusion(const PowersetConstUniquePtr& a, const PowersetConstUniquePtr& b);

    /*!
     * \brief Computes the fusion of two `Powerset` objects.
     * \param a The first `Powerset` object.
     * \param b The second `Powerset` object.
     * \return A `PowersetUniquePtr` representing the fusion of `a` and `b`.
     */
    PowersetUniquePtr fusion(const Powerset& a, const Powerset& b);

    /*!
     * \brief Computes the fusion of a `Powerset` object and a `Poly` object.
     * \param a The `Powerset` object.
     * \param b The `Poly` object.
     * \return A `PowersetUniquePtr` representing the fusion of `a` and `b`.
     */
    PowersetUniquePtr fusion(const Powerset& a, const Poly& b);

     /*!
     * \brief Computes the fusion of a vector of `PowersetConstSharedPtr` objects.
     * \param powersets A vector of `PowersetConstSharedPtr` objects.
     * \return A `PowersetUniquePtr` representing the fusion of all powersets.
     */
    PowersetUniquePtr fusion(const std::vector<PowersetConstSharedPtr>& powersets);

    /*!
     * \brief Computes the fusion of a `Powerset` object and another `Powerset` object, modifying the first.
     * \param a The first `Powerset` object (modified in place).
     * \param b The second `Powerset` object.
     */
    void fusion(Powerset& a, const Powerset& b);

    /*!
     * \brief Computes the fusion of two `Poly` objects.
     * \param a The first `Poly` object.
     * \param b The second `Poly` object.
     * \return A `PowersetUniquePtr` representing the fusion of `a` and `b`.
     */
    PowersetUniquePtr fusion(const Poly& a, const Poly& b);

    /*!
     * \brief Creates a linear expression representing the zero point in a given space dimension.
     * \param spaceDimension The dimension of the space.
     * \return A `PPL::Linear_Expression` representing the zero point.
     */
    PPL::Linear_Expression zeroPointLinearExpression(PPL::dimension_type spaceDimension);

    /*!
     * \brief Creates a `Poly` object representing the zero point in a given space dimension.
     * \param spaceDimension The dimension of the space.
     * \return A `Poly` object representing the zero point.
     */
    Poly zeroPoint(PPL::dimension_type spaceDimension);

    /*!
     * \brief Creates a `Poly` object representing a point defined by a linear expression.
     * \param pointLinearExpression The linear expression defining the point.
     * \return A `Poly` object representing the point.
     */
    Poly point(PPL::Linear_Expression pointLinearExpression);

    /*!
     * \brief Checks if two variables have the same space dimension.
     * \param x The first variable.
     * \param y The second variable.
     * \return `true` if the variables have the same space dimension, `false` otherwise.
     */
    bool haveSameSpaceDimension(const PPL::Variable& x, const PPL::Variable& y);

    /*!
     * \brief Checks if a `Powerset` contains a specific polyhedron as a disjunct.
     * \param powerset The `Powerset` object to check.
     * \param disjunct The `Poly` object to search for.
     * \return `true` if the `Powerset` contains the `Poly` as a disjunct, `false` otherwise.
     */
    bool containsDisjunct(const Powerset& powerset, const Poly& disjunct);

    /*!
     * \brief Checks if a constraint is a single-variable equality constraint of the form `x = 0`.
     * \param constraint The `PPL::Constraint` object to check.
     * \return `true` if the constraint is of the form `x = 0`, `false` otherwise.
     */
    bool isSingleVariableZeroEqualityConstraint(const PPL::Constraint& constraint);

    /*!
     * \brief Removes single-variable equality constraints of the form `x = 0` from a `Poly` object.
     * \param poly The `Poly` object to process.
     * \return A `PolyUniquePtr` representing the polyhedron with the constraints removed.
     */
    PolyUniquePtr removeSingleVariableZeroEqualityConstraints(const Poly& poly);
}
