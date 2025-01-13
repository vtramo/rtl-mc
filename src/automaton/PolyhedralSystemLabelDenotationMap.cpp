#include "PolyhedralSystemLabelDenotationMap.h"
#include "ppl_output.h"

using Parma_Polyhedra_Library::IO_Operators::operator<<;

PolyhedralSystemLabelDenotationMap::PolyhedralSystemLabelDenotationMap(
    const std::shared_ptr<PolyhedralSystem>& polyhedralSystem)
    : m_polyhedralSystem{polyhedralSystem}
{
}

PolyhedralSystemLabelDenotationMap::PolyhedralSystemLabelDenotationMap(PolyhedralSystemLabelDenotationMap&& other) noexcept
    : m_polyhedralSystem { std::move(other.m_polyhedralSystem) }
    , m_powersetByLabelsHash { std::move(other.m_powersetByLabelsHash) }
{
}

const PolyhedralSystem& PolyhedralSystemLabelDenotationMap::getPolyhedralSystem() const
{
    return *m_polyhedralSystem;
}

bool PolyhedralSystemLabelDenotationMap::containsDenotation(const AtomSet& labels) const
{
    return m_powersetByLabelsHash.count(labels.hash());
}

PowersetUniquePtr PolyhedralSystemLabelDenotationMap::getOrComputeDenotation(const AtomSet& labels)
{
    if (containsDenotation(labels))
    {
        return std::make_unique<Powerset>(*m_powersetByLabelsHash.at(labels.hash()));
    }

    return std::make_unique<Powerset>(*computeLabelDenotation(labels));
}

PowersetSharedPtr PolyhedralSystemLabelDenotationMap::computeLabelDenotation(const AtomSet& labels)
{
    PowersetSharedPtr powerset { std::make_shared<Powerset>(m_polyhedralSystem->getSpaceDimension(), PPL::UNIVERSE) };
    const PolyhedralSystemSymbolTable& symbolTable { m_polyhedralSystem->getSymbolTable() };

    const bool containsSing { labels.containsAtom(SpotUtils::sing()) };
    const LabelsHash labelsHash { labels.hash() };
    std::string labelsHashWithoutSing { };
    for (const auto& atom : symbolTable.atoms())
    {

        const AtomInterpretation* const atomInterpretation { getAtomInterpretation(atom) };
        if (labels.containsAtom(atom))
        {
            if (containsSing) labelsHashWithoutSing += atom.ap_name();

            powerset->intersection_assign(atomInterpretation->interpretation());
        }
        else
        {
            powerset->intersection_assign(atomInterpretation->notInterpretation());
        }
    }

    if (containsSing)
        insertLabelDenotation(labelsHash, std::hash<std::string>{}(labelsHashWithoutSing), powerset);
    else
        insertLabelDenotation(labelsHash, powerset);

    return powerset;
}

const AtomInterpretation* PolyhedralSystemLabelDenotationMap::getAtomInterpretation(const spot::formula& atom) const
{
    const std::optional atomInterpretation { m_polyhedralSystem->getInterpretation(atom) };
    if (!atomInterpretation)
    {
        throw std::invalid_argument("The atom " + atom.ap_name() + " has no interpretation!");
    }

    return *atomInterpretation;
}

void PolyhedralSystemLabelDenotationMap::insertLabelDenotation(LabelsHash labelsHash, LabelsHash labelsHashWithoutSing,
                                                               PowersetSharedPtr denotation)
{
    m_powersetByLabelsHash.insert(
        std::make_pair(
            labelsHashWithoutSing,
            denotation
        )
    );

    m_powersetByLabelsHash.insert(
        std::make_pair(
            labelsHash,
            denotation
        )
    );
}

void PolyhedralSystemLabelDenotationMap::insertLabelDenotation(LabelsHash labelsHash, PowersetSharedPtr denotation)
{
    m_powersetByLabelsHash.insert(
        std::make_pair(
            labelsHash,
            denotation
        )
    );
}

std::ostream& operator<< (std::ostream& out, PolyhedralSystemLabelDenotationMap& PolyhedralSystemLabelDenotationMap)
{
    out << "LABEL DENOTATION MAP\n";

    for (const auto& [labels, powerset] : PolyhedralSystemLabelDenotationMap.m_powersetByLabelsHash)
    {
        out << "Labels hash code: " << labels << '\n';
        out << "Denotation: " << PPLOutput::toString(*powerset, PolyhedralSystemLabelDenotationMap.m_polyhedralSystem->getSymbolTable())
            << "\n\n";
    }

    return out;
}
