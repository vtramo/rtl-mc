#include "PolyhedralSystemLabelDenotationMap.h"

#include <memory>

#include "ppl_output.h"

using Parma_Polyhedra_Library::IO_Operators::operator<<;

PolyhedralSystemLabelDenotationMap::PolyhedralSystemLabelDenotationMap(
    const std::shared_ptr<PolyhedralSystem>& polyhedralSystem)
    : m_polyhedralSystem{polyhedralSystem}
{
}

PolyhedralSystemLabelDenotationMap::PolyhedralSystemLabelDenotationMap(PolyhedralSystemLabelDenotationMap&& other) noexcept
    : m_polyhedralSystem { std::move(other.m_polyhedralSystem) }
    , m_powersetByAtomSet { std::move(other.m_powersetByAtomSet) }
{
}

const PolyhedralSystem& PolyhedralSystemLabelDenotationMap::getPolyhedralSystem() const
{
    return *m_polyhedralSystem;
}

bool PolyhedralSystemLabelDenotationMap::containsDenotation(const AtomSet& labels) const
{
    return m_powersetByAtomSet.count(labels.hash()) == 1;
}

PowersetConstSharedPtr PolyhedralSystemLabelDenotationMap::getOrComputeDenotation(const AtomSet& labels)
{
    if (containsDenotation(labels))
    {
        const auto& [powerset, _] = m_powersetByAtomSet.at(labels.hash());
        return powerset;
    }

    return computeLabelDenotation(labels);
}

PowersetConstSharedPtr PolyhedralSystemLabelDenotationMap::computeLabelDenotation(const AtomSet& labels)
{
    PowersetSharedPtr powerset { std::make_shared<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::UNIVERSE) };
    const PolyhedralSystemSymbolTable& symbolTable { m_polyhedralSystem->getSymbolTable() };

    for (const auto& atom: symbolTable.atoms())
    {
        const AtomInterpretation* const atomInterpretation { getAtomInterpretation(atom) };
        if (labels.containsAtom(atom))
            powerset->intersection_assign(atomInterpretation->interpretation());
        else
            powerset->intersection_assign(atomInterpretation->notInterpretation());
    }

    insertLabelDenotation(labels, powerset);

    return powerset;
}

const AtomInterpretation* PolyhedralSystemLabelDenotationMap::getAtomInterpretation(const spot::formula& atom) const
{
    const std::optional atomInterpretation { m_polyhedralSystem->getInterpretation(atom) };

    if (!atomInterpretation)
        throw std::invalid_argument("The atom " + atom.ap_name() + " has no interpretation!");

    return *atomInterpretation;
}

void PolyhedralSystemLabelDenotationMap::insertLabelDenotation(const AtomSet& labels, PowersetConstSharedPtr denotation)
{
    m_powersetByAtomSet.insert(
        std::make_pair(
            labels.hash(),
            std::move(std::make_tuple(denotation, labels.toString()))
        )
    );
    std::cout << "Insert size " << m_powersetByAtomSet.size() << std::endl;
}

std::ostream& operator<< (std::ostream& out, PolyhedralSystemLabelDenotationMap& polyhedralSystemLabelDenotationMap)
{
    out << "LABEL DENOTATION MAP\n";
    out << "Total mappings " << polyhedralSystemLabelDenotationMap.m_powersetByAtomSet.size() << "\n\n";

    for (const auto& [labels, powersetAndAtomSetToString]: polyhedralSystemLabelDenotationMap.m_powersetByAtomSet)
    {
        const auto& [powerset, labelsToString] = powersetAndAtomSetToString;
        out << "Labels: " << labelsToString << '\n'
            << "Denotation: " << PPLOutput::toString(*powerset, polyhedralSystemLabelDenotationMap.m_polyhedralSystem->getSymbolTable())
            << "\n\n";
    }

    return out;
}
