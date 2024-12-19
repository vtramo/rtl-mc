//
// Created by vincenzo on 16/12/24.
//

#include "AtomInterpretation.h"
#include "ppl_aliases.h"

namespace
{
    const Powerset& computeInvariantInterpretation(
        Powerset& interpretation,
        const Powerset& invariant
    );

    Powerset computeNotInvariantInterpretation(
        const Powerset& interpretation,
        const Powerset& invariant
    );
}


AtomInterpretation::AtomInterpretation(const Powerset& interpretation)
    : AtomInterpretation(
        interpretation,
        Powerset { interpretation.space_dimension(), PPL::UNIVERSE }
    ) {}

AtomInterpretation::AtomInterpretation(
    Powerset interpretation,
    const Powerset& invariant
): m_interpretation { computeInvariantInterpretation(interpretation, invariant) }
 , m_notInterpretation { computeNotInvariantInterpretation(m_interpretation, invariant) } {}

const Powerset& AtomInterpretation::interpretation() const
{
    return m_interpretation;
}

const Powerset& AtomInterpretation::notInterpretation() const
{
    return m_notInterpretation;
}

bool AtomInterpretation::operator==(const AtomInterpretation& other) const
{
    return m_interpretation == other.m_interpretation;
}


namespace
{
    const Powerset& computeInvariantInterpretation(
        Powerset& interpretation,
        const Powerset& invariant
    )
    {
        if (invariant.is_universe())
        {
            return interpretation;
        }

        interpretation.intersection_assign(invariant);
        return interpretation;
    }

    Powerset computeNotInvariantInterpretation(
        const Powerset& interpretation,
        const Powerset& invariant
    )
    {
        Powerset notInterpretation { invariant };
        notInterpretation.difference_assign(interpretation);
        return notInterpretation;
    }
}
