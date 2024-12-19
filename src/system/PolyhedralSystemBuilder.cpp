//
// Created by vincenzo on 16/12/24.
//

#include "PolyhedralSystemBuilder.h"

#include "PolyhedralSystem.h"


namespace
{
    void checkDimensionEquality(
        const PPL::dimension_type flowSpaceDimension,
        const PPL::dimension_type otherDimension,
        const std::string& context
    )
    {
        if (flowSpaceDimension != otherDimension)
        {
            throw std::invalid_argument("PolyhedralSystemBuilder::checkSpaceDimensions: Space dimension mismatch in " + context + ". Expected: "
                + std::to_string(flowSpaceDimension) + ", Found: " + std::to_string(otherDimension));
        }
    }
}


PolyhedralSystemBuilder& PolyhedralSystemBuilder::flow(const Poly& flow)
{
    m_flow = new Poly { flow };
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::invariant(const Powerset& invariant)
{
    m_invariant = new Powerset { invariant };
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable)
{
    m_symbolTable = new PolyhedralSystemSymbolTable { polyhedralSystemSymbolTable };
    return *this;
}

PolyhedralSystemBuilder&
PolyhedralSystemBuilder::denotation(const std::map<std::string, Powerset>& denotation)
{
    m_denotation = new std::map { denotation };
    return *this;
}

void PolyhedralSystemBuilder::checkFlow() const
{
    if (!m_flow)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkFlow: The flow is not set. Please set it using the flow() method.");
    }
}

void PolyhedralSystemBuilder::checkInvariant() const
{
    if (!m_invariant)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkInvariant: The invariant is not set. Please set it using the invariant() method.");
    }
}

void PolyhedralSystemBuilder::checkSymbolTable() const
{
    if (!m_symbolTable)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkSymbolTable: The symbol table is not set. Please set it using the symbolTable() method.");
    }
}

void PolyhedralSystemBuilder::checkDenotation() const
{
    if (!m_denotation)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkDenotation: The denotation is not set. Please set it using the denotation() method.");
    }
}

void PolyhedralSystemBuilder::checkSpaceDimensions() const
{
    const PPL::dimension_type flowSpaceDimension = m_flow->space_dimension();

    checkDimensionEquality(flowSpaceDimension, m_invariant->space_dimension(), "Invariant");
    checkDimensionEquality(flowSpaceDimension, m_symbolTable->getSpaceDimension(), "Symbol Table");

    for (auto& [atomId, atomPowerset]: *m_denotation)
    {
        checkDimensionEquality(flowSpaceDimension, atomPowerset.space_dimension(), "Denotation entry: " + atomId);
    }
}

PolyhedralSystem PolyhedralSystemBuilder::build() const
{
    checkInvariant();
    checkFlow();
    checkDenotation();
    checkSymbolTable();

    checkSpaceDimensions();

    return buildPolyhedralSystem();
}

PolyhedralSystem PolyhedralSystemBuilder::buildPolyhedralSystem() const
{
    std::map<std::string, AtomInterpretation> interpretations {};
    for (auto& [atomId, powerset]: *m_denotation)
    {
        const AtomInterpretation atomInterpretation { powerset, *m_invariant };
        interpretations.insert({ atomId, atomInterpretation });
    }

    return {
        *m_invariant,
        *m_flow,
        interpretations,
        *m_symbolTable,
    };
}

