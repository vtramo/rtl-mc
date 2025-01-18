#include "PolyhedralSystemFormulaDenotationMap.h"

#include <memory>
#include <ppl_utils.h>

#include "ppl_output.h"

using Parma_Polyhedra_Library::IO_Operators::operator<<;

PolyhedralSystemFormulaDenotationMap::PolyhedralSystemFormulaDenotationMap(const std::shared_ptr<PolyhedralSystem>& polyhedralSystem)
    : m_polyhedralSystem{polyhedralSystem}
{
}

PolyhedralSystemFormulaDenotationMap::PolyhedralSystemFormulaDenotationMap(PolyhedralSystemFormulaDenotationMap&& other) noexcept
    : m_polyhedralSystem { std::move(other.m_polyhedralSystem) }
    , m_powersetByFormula { std::move(other.m_powersetByFormula) }
{
}

const PolyhedralSystem& PolyhedralSystemFormulaDenotationMap::getPolyhedralSystem() const
{
    return *m_polyhedralSystem;
}

bool PolyhedralSystemFormulaDenotationMap::containsDenotation(const spot::formula& formula) const
{
    return m_powersetByFormula.count(formula.id()) == 1;
}

PowersetConstSharedPtr PolyhedralSystemFormulaDenotationMap::getOrComputeDenotation(const spot::formula& formula)
{
    if (containsDenotation(formula))
    {
        const auto& [powerset, _] { m_powersetByFormula.at(formula.id()) };
        return powerset;
    }

    assert(formula.is_sugar_free_boolean() && "Only AND, OR and NOT"); // Only AND, OR and NOT
    assert(formula.is_in_nenoform() && "Only NNF"); // in Negative Normal Form NNF
    return computeFormulaDenotation(formula);
}

PowersetConstSharedPtr PolyhedralSystemFormulaDenotationMap::computeFormulaDenotation(const spot::formula& formula)
{
    if (formula.is_literal())
    {
        const AtomInterpretation* atomIntepretation {};
        if (formula.is(spot::op::Not))
        {
            atomIntepretation = getAtomInterpretation(formula[0]);
            return std::make_shared<Powerset>(atomIntepretation->notInterpretation()); // TODO: evitare questa copia
        }

        atomIntepretation = getAtomInterpretation(formula);
        return std::make_shared<Powerset>(atomIntepretation->interpretation()); // TODO: evitare questa copia
    }

    PowersetConstSharedPtr powersetResult {};
    if (formula.is_tt())
    {
        powersetResult = std::make_shared<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::UNIVERSE);
    }
    else if (formula.is_ff())
    {
        powersetResult = std::make_shared<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::EMPTY);
    }
    else
    {
        PowersetConstSharedPtr powersetLeft { getOrComputeDenotation(formula[0]) };
        PowersetConstSharedPtr powersetRight { getOrComputeDenotation(formula[1]) };
        if (formula.is(spot::op::And))
            powersetResult = PPLUtils::intersect(*powersetLeft, *powersetRight);
        else
            powersetResult = PPLUtils::fusion(*powersetLeft, *powersetRight);
    }

    assert(powersetResult->space_dimension() == m_polyhedralSystem->getSpaceDimension());
    saveFormulaDenotation(formula, powersetResult);
    return powersetResult;
}

const AtomInterpretation* PolyhedralSystemFormulaDenotationMap::getAtomInterpretation(const spot::formula& formula) const
{
    assert(formula.is_literal() && !formula.is(spot::op::Not));

    const std::optional atomInterpretation { m_polyhedralSystem->getInterpretation(formula) };

    if (!atomInterpretation)
        throw std::invalid_argument("The atom " + formula.ap_name() + " has no interpretation!");

    return *atomInterpretation;
}

void PolyhedralSystemFormulaDenotationMap::saveFormulaDenotation(const spot::formula& formula, PowersetConstSharedPtr denotation)
{
    m_powersetByFormula.insert(
        std::make_pair(
            formula.id(),
            std::move(std::make_tuple(denotation, SpotUtils::toFormulaString(formula)))
        )
    );
}

std::ostream& operator<< (std::ostream& out, PolyhedralSystemFormulaDenotationMap& polyhedralSystemFormulaDenotationMap)
{
    out << "LABEL DENOTATION MAP\n";
    out << "Total mappings " << polyhedralSystemFormulaDenotationMap.m_powersetByFormula.size() << "\n\n";

    for (const auto& [labels, powersetAndAtomSetToString]: polyhedralSystemFormulaDenotationMap.m_powersetByFormula)
    {
        const auto& [powerset, formulaToString] = powersetAndAtomSetToString;
        out << "Formula: " << formulaToString << '\n'
            << "Denotation: " << PPLOutput::toString(*powerset, polyhedralSystemFormulaDenotationMap.m_polyhedralSystem->getSymbolTable())
            << "\n\n";
    }

    return out;
}
