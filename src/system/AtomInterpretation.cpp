//
// Created by vincenzo on 16/12/24.
//

#include "AtomInterpretation.h"

namespace
{
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& computeInvariantInterpretation(
        PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& interpretation,
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
    );

    PPL::Pointset_Powerset<PPL::NNC_Polyhedron> computeNotInvariantInterpretation(
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariantInterpretation,
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
    );
}


AtomInterpretation::AtomInterpretation(const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& interpretation)
    : AtomInterpretation(
        interpretation,
        PPL::Pointset_Powerset<PPL::NNC_Polyhedron> { interpretation.space_dimension() }
    ) {}

AtomInterpretation::AtomInterpretation(
    PPL::Pointset_Powerset<PPL::NNC_Polyhedron> interpretation,
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
): m_interpretation { computeInvariantInterpretation(interpretation, invariant) }
 , m_notInterpretation { computeNotInvariantInterpretation(m_interpretation, invariant) } {}

const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& AtomInterpretation::interpretation() const
{
    return m_interpretation;
}

const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& AtomInterpretation::notInterpretation() const
{
    return m_notInterpretation;
}

bool AtomInterpretation::operator==(const AtomInterpretation& other) const
{
    return m_interpretation == other.m_interpretation;
}


namespace
{
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& computeInvariantInterpretation(
    PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& interpretation,
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
)
    {
        if (invariant.is_universe())
        {
            return interpretation;
        }

        interpretation.intersection_assign(invariant);
        return interpretation;
    }

    PPL::Pointset_Powerset<PPL::NNC_Polyhedron> computeNotInvariantInterpretation(
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariantInterpretation,
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
    )
    {
        PPL::Pointset_Powerset notInterpretation { invariant };
        notInterpretation.difference_assign(invariantInterpretation);
        return notInterpretation;
    }
}
