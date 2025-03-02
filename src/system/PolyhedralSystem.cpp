#include <utility>
#include <spot/twa/bdddict.hh>

#include "utils.h"
#include "PolyhedralSystem.h"
#include "ppl_output.h"
#include "ppl_utils.h"
#include "systemparser.h"

const Poly& PolyhedralSystem::flow() const
{
    return m_flow;
}

const Poly& PolyhedralSystem::preFlow() const
{
    return m_preFlow;
}

bool PolyhedralSystem::isOmnidirectionalFlow() const
{
    return m_isOmnidirectionalFlow;
}

bool PolyhedralSystem::isClosedFlow() const
{
    return m_flow.is_topologically_closed();
}

bool PolyhedralSystem::isMovementForced() const
{
    return m_isMovementForced;
}

const Powerset& PolyhedralSystem::invariant() const
{
    return m_invariant;
}

bool PolyhedralSystem::isBoundedInvariant() const
{
    return m_invariant.is_bounded();
}

const PolyhedralSystemSymbolTable& PolyhedralSystem::symbolTable() const
{
    return m_symbolTable;
}

const spot::atomic_prop_set& PolyhedralSystem::atoms() const
{
    return m_symbolTable.atoms();
}

std::optional<const AtomInterpretation* const> PolyhedralSystem::interpretation(const std::string_view atom) const
{
    if (const auto it { m_denotation.find(std::string { atom }) }; it != m_denotation.end()) {
        return &it->second;
    }

     return {};
}

std::optional<const AtomInterpretation* const> PolyhedralSystem::interpretation(const spot::formula& atom) const
{
    if (!atom.is(spot::op::ap))
    {
        return {};
    }

    if (const auto it { m_denotation.find(atom.ap_name()) }; it != m_denotation.end())
        return &it->second;

     return {};
}

PPL::dimension_type PolyhedralSystem::spaceDimension() const
{
    return m_invariant.space_dimension();
}

int PolyhedralSystem::totalAtoms() const
{
    return static_cast<int>(m_denotation.size());
}

PolyhedralSystemBuilder PolyhedralSystem::builder()
{
    return PolyhedralSystemBuilder {};
}

bool operator== (const PolyhedralSystem& polyhedralSystem1, const PolyhedralSystem& polyhedralSystem2)
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
    evaluateFlowProperties();
    makeBddDict();
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
    assert(m_preFlow.space_dimension() == m_symbolTable.getSpaceDimension());
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
    evaluateFlowProperties();
    makeBddDict();
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
    assert(m_preFlow.space_dimension() == m_symbolTable.getSpaceDimension());
}

PolyhedralSystem::PolyhedralSystem(PolyhedralSystem&& polyhedralSystem) noexcept
    : m_isOmnidirectionalFlow { polyhedralSystem.m_isOmnidirectionalFlow }
    , m_isMovementForced { polyhedralSystem.m_isMovementForced }
    , m_bddDict { std::move(polyhedralSystem.m_bddDict) }
    , m_denotation { std::move(polyhedralSystem.m_denotation) }
    , m_symbolTable { std::move(polyhedralSystem.m_symbolTable) }
    , m_minimizeConstraintsOutput { polyhedralSystem.m_minimizeConstraintsOutput }
{
    m_flow.m_swap(polyhedralSystem.m_flow);
    m_invariant.m_swap(polyhedralSystem.m_invariant);
    m_preFlow.m_swap(polyhedralSystem.m_preFlow);
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
    assert(m_preFlow.space_dimension() == m_symbolTable.getSpaceDimension());
}

PolyhedralSystem& PolyhedralSystem::operator= (PolyhedralSystem&& polyhedralSystem) noexcept
{
    m_denotation = std::move(polyhedralSystem.m_denotation);
    m_flow.m_swap(polyhedralSystem.m_flow);
    m_invariant.m_swap(polyhedralSystem.m_invariant);
    m_preFlow.m_swap(polyhedralSystem.m_preFlow);
    m_symbolTable = std::move(polyhedralSystem.m_symbolTable);
    m_isOmnidirectionalFlow = polyhedralSystem.m_isOmnidirectionalFlow;
    m_isMovementForced = polyhedralSystem.m_isMovementForced;
    m_bddDict = std::move(polyhedralSystem.m_bddDict);
    m_minimizeConstraintsOutput = polyhedralSystem.m_minimizeConstraintsOutput;
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
    assert(m_preFlow.space_dimension() == m_symbolTable.getSpaceDimension());
    return *this;
}

void PolyhedralSystem::computePreFlow()
{
    Poly preFlow { m_flow };
    m_preFlow.m_swap(PPLUtils::reflectionAffineImage(preFlow));
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
}

void PolyhedralSystem::evaluateFlowProperties()
{
    const Poly zeroPoint { PPLUtils::zeroPoint(spaceDimension()) };
    PolyUniquePtr interiorFlow { PPLUtils::interior(m_flow) };
    Poly closureFlow { m_flow };
    closureFlow.topological_closure_assign();
    m_isOmnidirectionalFlow = interiorFlow->contains(zeroPoint);
    m_isMovementForced = !closureFlow.contains(zeroPoint);
}

void PolyhedralSystem::makeBddDict()
{
    m_bddDict = spot::make_bdd_dict();
}

std::istream& operator>> (std::istream& istream, PolyhedralSystem& polyhedralSystem)
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

void PolyhedralSystem::setConstraintOutputMinimized(const bool minimizeConstraintsOutput)
{
    m_minimizeConstraintsOutput = minimizeConstraintsOutput;
}

spot::bdd_dict_ptr PolyhedralSystem::bddDict() const
{
    return m_bddDict;
}

std::vector<Observable> PolyhedralSystem::generateObservables() const
{
    const spot::atomic_prop_set& polyhedralAtoms { atoms() };

    int totPolyhedralAtoms { totalAtoms() };
    std::vector<Observable> observables {};
    for (int k { 1 }; k <= totPolyhedralAtoms; k++)
    {
        std::vector kCombinations {
            combinations<spot::formula>(
                polyhedralAtoms.begin(),
                polyhedralAtoms.end(),
                totPolyhedralAtoms,
                k
            )
        };

        for (spot::atomic_prop_set kCombination: kCombinations)
        {
            PowersetSharedPtr observableDenotation { std::make_shared<Powerset>(spaceDimension(), PPL::UNIVERSE) };
            for (const spot::formula& polyhedralAtom: polyhedralAtoms)
            {
                const AtomInterpretation* atomInterpretation { *interpretation(polyhedralAtom) };
                observableDenotation->intersection_assign(
                    kCombination.count(polyhedralAtom) == 1
                        ? atomInterpretation->interpretation()
                        : atomInterpretation->notInterpretation()
                );
            }

#ifdef DEBUG
            Observable observable { kCombination, observableDenotation, PPLOutput::toString(*observableDenotation, symbolTable()) };
#else
            Observable observable { kCombination, observableDenotation };
#endif
            observables.push_back(observable);
        }
    }

    return observables;
}

std::ostream& operator<< (std::ostream& out, const PolyhedralSystem& polyhedralSystem)
{
    const bool minimizeConstraints { polyhedralSystem.m_minimizeConstraintsOutput };
    const PolyhedralSystemSymbolTable& symbolTable { polyhedralSystem.symbolTable() };
    out << "Inv " << PPLOutput::toString(polyhedralSystem.invariant(), symbolTable, minimizeConstraints) << '\n';
    out << "Flow " << PPLOutput::toString(polyhedralSystem.flow(), symbolTable, minimizeConstraints) << '\n';

    for (const auto& atom: symbolTable.atoms())
    {
        const AtomInterpretation* atomInterpretation { *polyhedralSystem.interpretation(atom) };
        out << atom << " " << PPLOutput::toString(atomInterpretation->interpretation(), symbolTable, minimizeConstraints) << '\n';
    }

    out << "Total atomic propositions: " << polyhedralSystem.totalAtoms() << ".\n";
    out << "PreFlow: " << PPLOutput::toString(polyhedralSystem.preFlow(), polyhedralSystem.symbolTable()) << '\n';

    out << "Space dimension: " << polyhedralSystem.spaceDimension() << ".\n";
    bool first { true };
    out << "Variables: [";
    for (PPL::dimension_type dim = 0; dim < polyhedralSystem.spaceDimension(); ++dim)
    {
        out << (first ? "" : ", ") << *polyhedralSystem.symbolTable().getVariableName(PPL::Variable { dim });
        first = false;
    }
    out << "]\n";
    return out;
}
