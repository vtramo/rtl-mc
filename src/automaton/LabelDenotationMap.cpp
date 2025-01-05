#include "LabelDenotationMap.h"
#include "spot_utils.h"

using Parma_Polyhedra_Library::IO_Operators::operator<<;

LabelDenotationMap::LabelDenotationMap(const PolyhedralSystem& polyhedralSystem)
    : m_polyhedralSystem { polyhedralSystem }
{
    m_denotation.insert({{}, Powerset { polyhedralSystem.getSpaceDimension(), PPL::EMPTY }});
}

bool LabelDenotationMap::containsDenotation(const AtomSet& labels) const
{
    return m_denotation.count(labels);
}

Powerset LabelDenotationMap::getDenotation(const AtomSet& labels)
{
    if (containsDenotation(labels))
    {
        return m_denotation.at(labels);
    }

    Powerset labelDenotation { m_polyhedralSystem.get().getSpaceDimension(), PPL::UNIVERSE };
    PolyhedralSystemSymbolTable symbolTable { m_polyhedralSystem.get().getSymbolTable() };
    for (const auto& atom: symbolTable.atoms())
    {
        const AtomInterpretation* const labelInterpretation { *m_polyhedralSystem.get().getInterpretation(atom) };
        if (labels.containsAtom(atom))
        {
            labelDenotation.intersection_assign(labelInterpretation->interpretation());
        } else
        {
            labelDenotation.intersection_assign(labelInterpretation->notInterpretation());
        }
    }

    m_denotation.insert({ labels, labelDenotation });
    return labelDenotation;
}

std::ostream& operator<<(std::ostream& out, LabelDenotationMap& atomSetDenotationMap)
{
    out << "LABEL DENOTATION MAP\n";

    for (const auto &[labels, powerset] : atomSetDenotationMap.m_denotation)
    {
        out << "Labels: " << labels << '\n';
        out << "Denotation: " << powerset << "\n\n";
    }

    return out;
}