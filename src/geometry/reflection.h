#pragma once

#include "ppl_aliases.h"

/*!
 *  \brief Applies a reflection transformation to a convex polyhedron.
 *
 * This function applies a reflection transformation to the input polyhedron \f$ P \f$. The reflection is performed
 * by negating each coordinate of the polyhedron. Specifically, for each dimension \f$ i\in\{0, \ldots, n-1\} \f$, the variable \f$ x_i \f$
 * is replaced with \f$ -x_i \f$.
 *
 * The transformation is applied in place, meaning the input polyhedron is modified directly.
 *
 * \param polyhedron The polyhedron \f$ P \f$ to transform.
 * \return A reference to the transformed polyhedron.
 *
 * \note The space dimension of the polyhedron remains unchanged after the transformation.
 */
inline Poly& reflectionAffineImage(Poly& polyhedron) {
    const PPL::dimension_type spaceDimension { polyhedron.space_dimension() };

    for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
        const PPL::Variable variable { dim };
        polyhedron.affine_image(variable, -variable);
    }

    assert(polyhedron.space_dimension() == spaceDimension);

    return polyhedron;
}

/*!
 *  \brief Applies a reflection transformation to a powerset.
 *  \param powerset The powerset \f$ S \f$ to transform.
 *  \return A reference to the transformed powerset.
 *  \see reflectionAffineImage(Poly& polyhedron)
 */
inline Powerset& reflectionAffineImage(Powerset& powerset) {
    const PPL::dimension_type spaceDimension { powerset.space_dimension() };

    for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
        const PPL::Variable variable { dim };
        powerset.affine_image(variable, -variable);
    }

    assert(powerset.space_dimension() == spaceDimension);

    return powerset;
}
