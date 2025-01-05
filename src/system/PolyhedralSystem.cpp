#include "PolyhedralSystem.h"

#include "spot_utils.h"
#include <utility>
#include "PolyhedralSystemBuilderVisitor.h"
#include "PolyhedralSystemLexer.h"
#include "PolyhedralSystemParser.h"
#include "PolyhedralSystemParsingResult.h"
#include "PolyhedralSystemSymbolTableListener.h"
#include "PolyhedralSystemErrorListener.h"
#include "ppl_utils.h"

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

std::optional<const AtomInterpretation* const> PolyhedralSystem::getInterpretation(const std::string_view ap) const
{
    if (const auto it { m_denotation.find(std::string { ap }) }; it != m_denotation.end()) {
        return &it->second;
    }

     return {};
}

std::optional<const AtomInterpretation* const> PolyhedralSystem::getInterpretation(const spot::formula ap) const
{
    if (!ap.is(spot::op::ap))
    {
        return {};
    }

    if (const auto it { m_denotation.find(ap.ap_name()) }; it != m_denotation.end()) {
        return &it->second;
    }

     return {};
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

bool operator==(const PolyhedralSystem& polyhedralSystem1, const PolyhedralSystem& polyhedralSystem2)
{
    const bool flowEqual = polyhedralSystem1.m_flow == polyhedralSystem2.m_flow;
    const bool invariantEqual = polyhedralSystem1.m_invariant == polyhedralSystem2.m_invariant;
    const bool denotationEqual = polyhedralSystem1.m_denotation == polyhedralSystem2.m_denotation;
    const bool symbolTableEqual = polyhedralSystem1.m_symbolTable == polyhedralSystem2.m_symbolTable;

    return flowEqual && invariantEqual && denotationEqual && symbolTableEqual;
}

PolyhedralSystem::PolyhedralSystem(
    const Powerset& invariant,
    const Poly& flow,
    const std::unordered_map<Atom, AtomInterpretation>& denotation,
    const PolyhedralSystemSymbolTable& symbolTable
) : m_invariant { invariant }
  , m_flow { flow }
  , m_denotation { denotation }
  , m_symbolTable { symbolTable }
{
    computePreFlow();
}

PolyhedralSystem::PolyhedralSystem(
    Powerset&& invariant,
    Poly&& flow,
    std::unordered_map<Atom, AtomInterpretation>&& denotation,
    PolyhedralSystemSymbolTable&& symbolTable
) : m_denotation { std::move(denotation) }
  , m_symbolTable { std::move(symbolTable) }
{
    m_invariant.m_swap(invariant);
    m_flow.m_swap(flow);
    computePreFlow();
}

PolyhedralSystem::PolyhedralSystem(PolyhedralSystem&& polyhedralSystem) noexcept
    : m_denotation { std::move(polyhedralSystem.m_denotation) }
    , m_symbolTable { std::move(polyhedralSystem.m_symbolTable) }
{
    m_flow.m_swap(polyhedralSystem.m_flow);
    m_invariant.m_swap(polyhedralSystem.m_invariant);
    m_preFlow.m_swap(polyhedralSystem.m_preFlow);
}

PolyhedralSystem& PolyhedralSystem::operator=(PolyhedralSystem&& polyhedralSystem) noexcept
{
    m_denotation = std::move(polyhedralSystem.m_denotation);
    m_flow.m_swap(polyhedralSystem.m_flow);
    m_invariant.m_swap(polyhedralSystem.m_invariant);
    m_preFlow.m_swap(polyhedralSystem.m_preFlow);
    m_symbolTable = std::move(polyhedralSystem.m_symbolTable);
    return *this;
}


void PolyhedralSystem::computePreFlow()
{
    Poly preFlow { m_flow };
    m_preFlow.m_swap(PPLUtils::reflectionAffineImage(preFlow));
}

std::istream& operator>>(std::istream& istream, PolyhedralSystem&& polyhedralSystem)
{
    PolyhedralSystemParsingResult parsingResult { parsePolyhedralSystem(istream) };

    if (parsingResult.ok())
    {
        polyhedralSystem = std::move(*parsingResult);
        return istream;
    }

    istream.setstate(std::ios::failbit);
    polyhedralSystem = {};

    return istream;
}

PolyhedralSystemParsingResult parsePolyhedralSystem(const std::string_view input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parsePolyhedralSystem(&antlrInput);
}

PolyhedralSystemParsingResult parsePolyhedralSystem(std::istream& input)
{
    antlr4::ANTLRInputStream antlrInput { input };
    return parsePolyhedralSystem(&antlrInput);
}

PolyhedralSystemParsingResult parsePolyhedralSystem(antlr4::ANTLRInputStream* input)
{
    PolyhedralSystemLexer lexer { input };
    PolyhedralSystemErrorListener* errorListener { new PolyhedralSystemErrorListener {} };
    lexer.removeErrorListeners();
    lexer.addErrorListener(errorListener);
    antlr4::CommonTokenStream tokens { &lexer };
    PolyhedralSystemParser parser { &tokens };
    parser.removeErrorListeners();
    parser.addErrorListener(errorListener);
    PolyhedralSystemParser::SystemContext* parseTree = parser.system();
    if (errorListener->hasErrors())
    {
        std::vector errors { errorListener->getErrors() };
        delete errorListener;
        return PolyhedralSystemParsingResult { errors };
    }

    delete errorListener;
    antlr4::tree::ParseTreeWalker walker {};
    PolyhedralSystemSymbolTableListener symbolTableListener {};
    walker.walk(&symbolTableListener, parseTree);
    PolyhedralSystemSymbolTable symbolTable { symbolTableListener.getSymbolTable() };
    PolyhedralSystemBuilderVisitor polyhedralSystemBuilderVisitor { symbolTable };
    return PolyhedralSystemParsingResult { polyhedralSystemBuilderVisitor.buildPolyhedralSystem(parseTree) };
}