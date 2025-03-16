#pragma once

#include "ppl_aliases.h"

inline Poly& reflectionAffineImage(Poly& polyhedron) {
    const PPL::dimension_type spaceDimension { polyhedron.space_dimension() };

    for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
        const PPL::Variable variable { dim };
        polyhedron.affine_image(variable, -variable);
    }

    assert(polyhedron.space_dimension() == spaceDimension);

    return polyhedron;
}

inline Powerset& reflectionAffineImage(Powerset& powerset) {
    const PPL::dimension_type spaceDimension { powerset.space_dimension() };

    for (PPL::dimension_type dim {}; dim < spaceDimension; ++dim) {
        const PPL::Variable variable { dim };
        powerset.affine_image(variable, -variable);
    }

    assert(powerset.space_dimension() == spaceDimension);

    return powerset;
}
