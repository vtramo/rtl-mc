//
// Created by vincenzo on 16/12/24.
//

#include "PolyhedralSystem.h"

#include <utility>
#include "PolyhedralSystemBuilderVisitor.h"
#include "PolyhedralSystemLexer.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemSymbolTableListener.h"
#include "ppl_utils.h"


PolyhedralSystem buildPolyhedralSystem(const std::string_view input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return buildPolyhedralSystem(&antlrInput);
}

PolyhedralSystem buildPolyhedralSystem(std::istream& input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return buildPolyhedralSystem(&antlrInput);
}

PolyhedralSystem buildPolyhedralSystem(antlr4::ANTLRInputStream* input)
{
    PolyhedralSystemLexer lexer { input };
    antlr4::CommonTokenStream tokens { &lexer };
    PolyhedralSystemParser parser { &tokens };
    PolyhedralSystemParser::SystemContext* parseTree = parser.system();
    antlr4::tree::ParseTreeWalker walker {};
    PolyhedralSystemSymbolTableListener symbolTableListener {};
    walker.walk(&symbolTableListener, parseTree);
    PolyhedralSystemSymbolTable symbolTable { symbolTableListener.getSymbolTable() };
    PolyhedralSystemBuilderVisitor polyhedralSystemBuilderVisitor { symbolTable };
    return polyhedralSystemBuilderVisitor.buildPolyhedralSystem(parseTree);
}

const Poly& PolyhedralSystem::getFlow() const
{
    return m_flow;
}

const Poly& PolyhedralSystem::getPreFlow() const
{
    return m_preFlow;
}

const Powerset& PolyhedralSystem::getInvariant() const
{
    return m_invariant;
}

const PolyhedralSystemSymbolTable& PolyhedralSystem::getSymbolTable() const
{
    return m_symbolTable;
}

std::optional<const AtomInterpretation*> PolyhedralSystem::getInterpretation(const std::string_view atomId) const
{
    if (const auto it { m_denotation.find(std::string(atomId)) }; it != m_denotation.end()) {
        return &it->second;
    }

    return std::nullopt;
}

PPL::dimension_type PolyhedralSystem::getSpaceDimension() const
{
    return m_invariant.space_dimension();
}

int PolyhedralSystem::getTotalAtoms() const
{
    return static_cast<int>(m_denotation.size());
}

PolyhedralSystemBuilder PolyhedralSystem::builder()
{
    return PolyhedralSystemBuilder {};
}

bool PolyhedralSystem::operator==(const PolyhedralSystem& other) const
{
    const bool flowEqual = m_flow == other.m_flow;
    const bool invariantEqual = m_invariant == other.m_invariant;
    const bool denotationEqual = m_denotation == other.m_denotation;
    const bool symbolTableEqual = m_symbolTable == other.m_symbolTable;

    return flowEqual && invariantEqual && denotationEqual && symbolTableEqual;
}

PolyhedralSystem::PolyhedralSystem(
    const Powerset& invariant,
    const Poly& flow,
    const std::map<std::string, AtomInterpretation>& denotation,
    PolyhedralSystemSymbolTable& symbolTable
) : m_invariant { invariant }
  , m_flow { flow }
  , m_denotation { denotation }
  , m_symbolTable { std::move(symbolTable) } {}


void PolyhedralSystem::computeReflectedFlow()
{
    Poly preFlow { m_flow };
    m_preFlow = PPLUtils::reflectionAffineImage(preFlow);
}

std::istream& operator>>(std::istream& istream, PolyhedralSystem& polyhedralSystem)
{
    const PolyhedralSystem buildedPolyhedralSystem { buildPolyhedralSystem(istream) };
    polyhedralSystem.m_denotation = buildedPolyhedralSystem.m_denotation;
    polyhedralSystem.m_symbolTable = buildedPolyhedralSystem.m_symbolTable;
    polyhedralSystem.m_flow = buildedPolyhedralSystem.m_flow;
    polyhedralSystem.m_invariant = buildedPolyhedralSystem.m_invariant;
    polyhedralSystem.m_preFlow = buildedPolyhedralSystem.m_preFlow;
    return istream;
}