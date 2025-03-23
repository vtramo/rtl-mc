#pragma once

#include "interior.h"

/*!
 *  \brief Checks if a \f$ \mathit{Flow} \f$ is an omnidirectional flow.
 *
 * A convex polyhedron \f$ \mathit{Flow} \f$ is said to represent an omnidirectional flow if its interior contains the origin.
 * In other words, the flow is omnidirectional if the origin lies strictly inside the convex polyhedron \f$ \mathit{Flow} \f$.
 *
 * This function works as follows:
 * 1. Computes the interior of the polyhedron \f$ \mathit{Flow} \f$ using the \ref interior function.
 * 2. Constructs the zero point (origin) in the same space dimension as \f$ \mathit{Flow} \f$.
 * 3. Checks if the interior of \f$ \mathit{Flow} \f$ contains the zero point.
 *
 * \param flow The convex polyhedron \f$ \mathit{Flow} \f$ to check.
 * \return `true` if \f$ \mathit{Flow} \f$ represents an omnidirectional flow, `false` otherwise.
 *
 * \see interior(const Poly& poly)
 */
inline bool isOmnidirectionalFlow(const Poly& flow)
{
    PolyUniquePtr interiorFlow { interior(flow) };
    Poly zeroPoint { PPLUtils::zeroPoint(flow.space_dimension()) };
    return interiorFlow->contains(zeroPoint);
}