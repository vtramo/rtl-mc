//
// Created by vincenzo on 16/12/24.
//

#include <ppl_utils.h>
#include "AtomInterpretation.h"
#include "ppl_aliases.h"

AtomInterpretation::AtomInterpretation(const Powerset& interpretation)
    : AtomInterpretation(
        interpretation,
        Powerset { interpretation.space_dimension(), PPL::UNIVERSE }
    ) {}

AtomInterpretation::AtomInterpretation(const Powerset& interpretation, const Powerset& invariant)
    : m_interpretation { PPLUtils::intersect(interpretation, invariant) }
    , m_notInterpretation { PPLUtils::minus(invariant, m_interpretation) } {}

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
