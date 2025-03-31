#include <memory>

#include "PolyhedralSystemFormulaDenotationMap.h"
#include "ppl_utils.h"
#include "ppl_output.h"
#include "formula.h"


PolyhedralSystemFormulaDenotationMap::PolyhedralSystemFormulaDenotationMap(PolyhedralSystemSharedPtr polyhedralSystem)
    : m_polyhedralSystem { polyhedralSystem }
{
}

PolyhedralSystemFormulaDenotationMap::PolyhedralSystemFormulaDenotationMap(PolyhedralSystemFormulaDenotationMap&& other) noexcept
    : m_polyhedralSystem { std::move(other.m_polyhedralSystem) }
    , m_powersetByFormula { std::move(other.m_powersetByFormula) }
{
}

PolyhedralSystemConstSharedPtr PolyhedralSystemFormulaDenotationMap::polyhedralSystem() const
{
    return m_polyhedralSystem;
}

bool PolyhedralSystemFormulaDenotationMap::containsDenotation(const spot::formula& formula) const
{
    return m_powersetByFormula.count(formula.id()) == 1;
}

PowersetConstSharedPtr PolyhedralSystemFormulaDenotationMap::getOrComputeDenotation(const spot::formula& formula)
{
    if (!isCnf(formula))
    {
        throw std::invalid_argument("Formula is not in CNF! " + toFormulaString(formula));
    }

    if (containsDenotation(formula))
    {
        const auto& [powerset, _] { m_powersetByFormula.at(formula.id()) };
        return powerset;
    }

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
            return std::make_shared<Powerset>(atomIntepretation->notInterpretation());
        }

        atomIntepretation = getAtomInterpretation(formula);
        return std::make_shared<Powerset>(atomIntepretation->interpretation());
    }

    PowersetConstSharedPtr powersetResult {};
    if (formula.is_tt())
    {
        powersetResult = std::make_shared<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::UNIVERSE);
    }
    else if (formula.is_ff())
    {
        powersetResult = std::make_shared<Powerset>(m_polyhedralSystem->spaceDimension(), PPL::EMPTY);
    }
    else
    {
        std::vector<PowersetConstSharedPtr> powersets {};
        powersets.reserve(formula.size());

        for (const auto& child: formula)
        {
            powersets.push_back(getOrComputeDenotation(child));
        }

        powersetResult =
            formula.is(spot::op::And)
                ? PPLUtils::intersect(powersets)
                : PPLUtils::fusion(powersets);
    }

    assert(powersetResult->space_dimension() == m_polyhedralSystem->spaceDimension());
    storeFormulaDenotation(formula, powersetResult);
    return powersetResult;
}

const AtomInterpretation* PolyhedralSystemFormulaDenotationMap::getAtomInterpretation(const spot::formula& formula) const
{
    assert(formula.is_literal() && !formula.is(spot::op::Not));

    const std::optional atomInterpretation { m_polyhedralSystem->getAtomInterpretation(formula) };

    if (!atomInterpretation)
    {
        throw std::invalid_argument("The atom " + formula.ap_name() + " has no interpretation!");
    }

    return *atomInterpretation;
}

void PolyhedralSystemFormulaDenotationMap::storeFormulaDenotation(const spot::formula& formula, PowersetConstSharedPtr denotation)
{
    m_powersetByFormula.insert(
        std::make_pair(
            formula.id(),
            std::move(std::make_tuple(denotation, toFormulaString(formula)))
        )
    );
}

std::ostream& operator<< (std::ostream& out, PolyhedralSystemFormulaDenotationMap& polyhedralSystemFormulaDenotationMap)
{
    out << "FORMULA DENOTATION MAP\n";
    out << "Total mappings " << polyhedralSystemFormulaDenotationMap.m_powersetByFormula.size() << "\n\n";

    for (const auto& [labels, powersetAndFormulaToString]: polyhedralSystemFormulaDenotationMap.m_powersetByFormula)
    {
        const auto& [powerset, formulaToString] = powersetAndFormulaToString;
        out << "Formula: " << formulaToString << '\n'
            << "Denotation: " << PPLOutput::toString(*powerset, polyhedralSystemFormulaDenotationMap.m_polyhedralSystem->symbolTable())
            << "\n\n";
    }

    return out;
}
