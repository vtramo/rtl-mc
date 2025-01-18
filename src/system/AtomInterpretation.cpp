#include "ppl_utils.h"
#include "AtomInterpretation.h"
#include "ppl_aliases.h"

using namespace PPL::IO_Operators;

AtomInterpretation::AtomInterpretation(const Powerset& interpretation)
    : AtomInterpretation(
        interpretation,
        std::move(Powerset { interpretation.space_dimension(), PPL::UNIVERSE })
    ) {}

AtomInterpretation::AtomInterpretation(Powerset&& interpretation)
    : AtomInterpretation(
        std::move(interpretation),
        std::move(Powerset { interpretation.space_dimension(), PPL::UNIVERSE })
    ) {}

AtomInterpretation::AtomInterpretation(const Powerset& interpretation, const Powerset& invariant)
{
    m_interpretation.m_swap(*PPLUtils::intersect(interpretation, invariant));
    m_notInterpretation.m_swap(*PPLUtils::minus(invariant, m_interpretation));
    assert(m_interpretation.space_dimension() == m_notInterpretation.space_dimension());
}

AtomInterpretation::AtomInterpretation(const Powerset& interpretation, Powerset&& invariant)
{
    m_interpretation.m_swap(*PPLUtils::intersect(interpretation, invariant));
    m_notInterpretation.m_swap(*PPLUtils::minus(std::move(invariant), m_interpretation));
    assert(m_interpretation.space_dimension() == m_notInterpretation.space_dimension());
}

AtomInterpretation::AtomInterpretation(Powerset&& interpretation, Powerset&& invariant)
{
    m_interpretation.m_swap(*PPLUtils::intersect(invariant, std::move(interpretation)));
    m_notInterpretation.m_swap(*PPLUtils::minus(std::move(invariant), m_interpretation));
    assert(m_interpretation.space_dimension() == m_notInterpretation.space_dimension());
}

AtomInterpretation::AtomInterpretation(Powerset&& interpretation, const Powerset& invariant)
{
    m_interpretation.m_swap(*PPLUtils::intersect(invariant, std::move(interpretation)));
    m_notInterpretation.m_swap(*PPLUtils::minus(invariant, m_interpretation));
    assert(m_interpretation.space_dimension() == m_notInterpretation.space_dimension());
}

AtomInterpretation::AtomInterpretation(AtomInterpretation&& interpretation) noexcept
{
    m_interpretation.m_swap(interpretation.m_interpretation);
    m_notInterpretation.m_swap(interpretation.m_notInterpretation);
    assert(m_interpretation.space_dimension() == m_notInterpretation.space_dimension());
}

const Powerset& AtomInterpretation::interpretation() const
{
    return m_interpretation;
}

const Powerset& AtomInterpretation::notInterpretation() const
{
    return m_notInterpretation;
}

bool operator==(const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2)
{
    return interpretation1.interpretation() == interpretation2.interpretation();
}

bool operator!=(const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2)
{
    return !(operator==(interpretation1, interpretation2));
}

std::ostream& operator<<(std::ostream& out, const AtomInterpretation& interpretation)
{
    out << "Interpretation: " << interpretation.interpretation() << '\n';
    out << "Not Interpretation: " << interpretation.notInterpretation() << '\n';
    return out;
}