#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystem.h"

namespace
{
    void assertDimensionEquality(
        PPL::dimension_type spaceDimension1,
        PPL::dimension_type spaceDimension2,
        const std::string& context
    );
}


PolyhedralSystemBuilder& PolyhedralSystemBuilder::flow(const Poly& flow)
{
    m_flow = std::make_unique<Poly>(flow);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::flow(Poly&& flow)
{
    m_flow = std::make_unique<Poly>();
    m_flow->m_swap(flow);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::invariant(Powerset&& invariant)
{
    m_invariant = std::make_unique<Powerset>();
    m_invariant->m_swap(invariant);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::invariant(const Powerset& invariant)
{
    m_invariant = std::make_unique<Powerset>(invariant);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable)
{
    m_symbolTable = std::make_unique<PolyhedralSystemSymbolTable>(polyhedralSystemSymbolTable);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::symbolTable(PolyhedralSystemSymbolTable&& polyhedralSystemSymbolTable)
{
    m_symbolTable = std::make_unique<PolyhedralSystemSymbolTable>(std::move(polyhedralSystemSymbolTable));
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::denotation(const std::unordered_map<std::string, Powerset>& denotation)
{
    m_denotation = std::make_unique<std::unordered_map<Atom, Powerset>>(denotation);
    return *this;
}

PolyhedralSystemBuilder& PolyhedralSystemBuilder::denotation(std::unordered_map<std::string, Powerset>&& denotation)
{
    m_denotation = std::make_unique<std::unordered_map<Atom, Powerset>>();
    m_denotation->swap(denotation);
    return *this;
}

void PolyhedralSystemBuilder::assertFlowIsNotNull() const
{
    if (!m_flow)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkFlow: The flow is not set. Please set it using the flow() method.");
    }
}

void PolyhedralSystemBuilder::assertInvariantIsNotNull() const
{
    if (!m_invariant)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkInvariant: The invariant is not set. Please set it using the invariant() method.");
    }
}

void PolyhedralSystemBuilder::assertSymbolTableIsNotNull() const
{
    if (!m_symbolTable)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkSymbolTable: The symbol table is not set. Please set it using the symbolTable() method.");
    }
}

void PolyhedralSystemBuilder::assertDenotationIsNotNull() const
{
    if (!m_denotation)
    {
        throw std::invalid_argument("PolyhedralSystemBuilder::checkDenotation: The denotation is not set. Please set it using the denotation() method.");
    }
}

void PolyhedralSystemBuilder::assertThatAllDimensionsAreEqual() const
{
    const PPL::dimension_type flowSpaceDimension = m_flow->space_dimension();

    assertDimensionEquality(flowSpaceDimension, m_invariant->space_dimension(), "Invariant");
    assertDimensionEquality(flowSpaceDimension, m_symbolTable->getSpaceDimension(), "Symbol Table");

    for (auto& [atomId, atomPowerset]: *m_denotation)
    {
        assertDimensionEquality(flowSpaceDimension, atomPowerset.space_dimension(), "Denotation entry: " + atomId);
    }
}

PolyhedralSystem PolyhedralSystemBuilder::build() const
{
    assertInvariantIsNotNull();
    assertFlowIsNotNull();
    assertDenotationIsNotNull();
    assertSymbolTableIsNotNull();

    assertThatAllDimensionsAreEqual();

    return buildPolyhedralSystem();
}

std::unordered_map<Atom, AtomInterpretation> PolyhedralSystemBuilder::buildDenotation() const
{
    std::unordered_map<Atom, AtomInterpretation> interpretations {};
    interpretations.reserve(m_denotation->size());

    for (auto& [atom, interpretation]: *m_denotation)
    {
        interpretations.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(atom),
            std::forward_as_tuple(std::move(interpretation), *m_invariant));
    }

    return interpretations;
}

PolyhedralSystem PolyhedralSystemBuilder::buildPolyhedralSystem() const
{
    assert(m_flow && "PolyhedralSystemBuilder::buildPolyhedralSystem(): Flow is not set!");
    assert(m_invariant && "PolyhedralSystemBuilder::buildPolyhedralSystem(): Invariant is not set!");
    assert(m_symbolTable && "PolyhedralSystemBuilder::buildPolyhedralSystem(): Symbol Table is not set!");
    assert(m_denotation && "PolyhedralSystemBuilder::buildPolyhedralSystem(): Denotation is not set!");

    auto denotation { buildDenotation() };

    return {
        std::move(*m_invariant),
        std::move(*m_flow),
        std::move(denotation),
        std::move(*m_symbolTable),
    };
}

namespace
{
    void assertDimensionEquality(
        const PPL::dimension_type spaceDimension1,
        const PPL::dimension_type spaceDimension2,
        const std::string& context
    )
    {
        if (spaceDimension1 != spaceDimension2)
        {
            throw std::invalid_argument("PolyhedralSystemBuilder::checkSpaceDimensions: Space dimension mismatch in " + context + ". Expected: "
                + std::to_string(spaceDimension1) + ", Found: " + std::to_string(spaceDimension2));
        }
    }
}

