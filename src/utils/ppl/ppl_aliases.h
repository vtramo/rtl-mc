#pragma once

#ifndef PPL_ppl_hh
#include <ppl.hh>
#endif

/*!
 *  \brief Type alias for a not necessarily closed polyhedron (NNC_Polyhedron) from the Parma Polyhedra Library (PPL).
 *
 * This type represents a convex polyhedron defined by constraints, where the constraints can include
 * strict inequalities (e.g., `x > 0`). It is a fundamental type in the Parma Polyhedra Library.
 */
using Poly = Parma_Polyhedra_Library::NNC_Polyhedron;

/*!
 *  \brief Type alias for a unique pointer to a not necessarily closed polyhedron (Poly).
 *
 * This type is used to manage the lifetime of a dynamically allocated `Poly` object, ensuring
 * that the object is automatically deleted when the pointer goes out of scope.
 */
using PolyUniquePtr = std::unique_ptr<Poly>;

/*!
 *  \brief Type alias for a shared pointer to a not necessarily closed polyhedron (Poly).
 *
 * This type is used to manage shared ownership of a dynamically allocated `Poly` object.
 * The object will be automatically deleted when the last shared pointer referencing it goes out of scope.
 */
using PolySharedPtr = std::shared_ptr<Poly>;

/*!
 *  \brief Type alias for a unique pointer to a constant not necessarily closed polyhedron (const Poly).
 *
 * This type is used to manage the lifetime of a dynamically allocated `const Poly` object, ensuring
 * that the object is automatically deleted when the pointer goes out of scope. The object cannot be
 * modified through this pointer.
 */
using PolyConstUniquePtr = std::unique_ptr<const Poly>;

/*!
 *  \brief Type alias for a set of convex polyhedra (Pointset_Powerset<Poly>).
 *
 * This type represents a set of convex polyhedra, where each convex polyhedron is an instance of `Poly`.
 */
using Powerset = Parma_Polyhedra_Library::Pointset_Powerset<Poly>;

/*!
 *  \brief Type alias for a shared pointer to a Powerset.
 *
 * This type is used to manage shared ownership of a dynamically allocated `Powerset` object.
 * The object will be automatically deleted when the last shared pointer referencing it goes out of scope.
 */
using PowersetSharedPtr = std::shared_ptr<Powerset>;

/*!
 *  \brief Type alias for a shared pointer to a constant Powerset.
 *
 * This type is used to manage shared ownership of a dynamically allocated `const Powerset` object.
 * The object cannot be modified through this pointer, and it will be automatically deleted when
 * the last shared pointer referencing it goes out of scope.
 */
using PowersetConstSharedPtr = std::shared_ptr<const Powerset>;

/*!
 *  \brief Type alias for a unique pointer to a Powerset.
 *
 * This type is used to manage the lifetime of a dynamically allocated `Powerset` object, ensuring
 * that the object is automatically deleted when the pointer goes out of scope.
 */
using PowersetUniquePtr = std::unique_ptr<Powerset>;

/*!
 *  \brief Type alias for a unique pointer to a constant Powerset.
 *
 * This type is used to manage the lifetime of a dynamically allocated `const Powerset` object, ensuring
 * that the object is automatically deleted when the pointer goes out of scope. The object cannot be
 * modified through this pointer.
 */
using PowersetConstUniquePtr = std::unique_ptr<const Powerset>;

/*!
 *  \brief Namespace alias for the Parma Polyhedra Library (PPL).
 *
 * This alias provides a shorter and more convenient way to refer to the `Parma_Polyhedra_Library` namespace.
 */
namespace PPL = Parma_Polyhedra_Library;