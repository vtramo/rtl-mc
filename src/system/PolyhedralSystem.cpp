#include <utility>
#include <spot/twa/bdddict.hh>

#include "utils.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "ppl_output.h"
#include "systemparser.h"
#include "reflection.h"
#include "omnidirectional.h"
#include "formula.h"

const Poly& PolyhedralSystem::flow() const
{
    return m_flow;
}

const Poly& PolyhedralSystem::preFlow() const
{
    return m_preFlow;
}

bool PolyhedralSystem::hasOmnidirectionalFlow() const
{
    return m_hasOmnidirectionalFlow;
}

bool PolyhedralSystem::hasClosedFlow() const
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

bool PolyhedralSystem::hasBoundedInvariant() const
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

std::optional<const AtomInterpretation* const> PolyhedralSystem::getAtomInterpretation(const std::string_view atom) const
{
    return getAtomInterpretation(ap(atom));
}

std::optional<const AtomInterpretation* const> PolyhedralSystem::getAtomInterpretation(const spot::formula& atom) const
{
    if (!atom.is(spot::op::ap))
        return {};

    if (const auto it { m_denotation.find(atom) }; it != m_denotation.end())
        return &it->second;

     return {};
}

const AtomInterpretation& PolyhedralSystem::addAtomInterpretation(const std::string_view atom, const Powerset& interpretation)
{
    return addAtomInterpretation(ap(atom), interpretation);
}

const AtomInterpretation& PolyhedralSystem::addAtomInterpretation(const spot::formula& atom, const Powerset& interpretation)
{
    if (!atom.is(spot::op::ap))
        throw std::invalid_argument("PolyhedralSystem::addAtomInterpretation: formula is not an atomic proposition!");

    if (containsAtom(atom))
        throw std::invalid_argument("PolyhedralSystem::addAtomInterpretation: atom " + atom.ap_name() + " already exists!");

    m_symbolTable.addAtom(atom);
    return m_denotation.emplace(atom, AtomInterpretation { interpretation, m_invariant }).first->second;
}

bool PolyhedralSystem::containsAtom(const std::string_view atom) const
{
    return containsAtom(ap(atom));
}

bool PolyhedralSystem::containsAtom(spot::formula atom) const
{
    if (!atom.is(spot::op::ap))
        return false;

    if (const auto it { m_denotation.find(atom) }; it != m_denotation.end())
        return true;

    return false;
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
    const std::unordered_map<spot::formula, AtomInterpretation>& denotation,
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
    std::unordered_map<spot::formula, AtomInterpretation>&& denotation,
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
    : m_hasOmnidirectionalFlow { polyhedralSystem.m_hasOmnidirectionalFlow }
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
    m_hasOmnidirectionalFlow = polyhedralSystem.m_hasOmnidirectionalFlow;
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
    m_preFlow.m_swap(reflectionAffineImage(preFlow));
    assert(m_preFlow.space_dimension() == m_flow.space_dimension());
    assert(m_preFlow.space_dimension() == spaceDimension());
}

void PolyhedralSystem::evaluateFlowProperties()
{
    m_hasOmnidirectionalFlow = isOmnidirectionalFlow(m_flow);

    Poly zeroPoint { PPLUtils::zeroPoint(spaceDimension()) };
    Poly closureFlow { m_flow };
    closureFlow.topological_closure_assign();
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

void PolyhedralSystem::setConstraintOutputMinimized(const bool minimize)
{
    m_minimizeConstraintsOutput = minimize;
}

spot::bdd_dict_ptr PolyhedralSystem::bddDict() const
{
    return m_bddDict;
}

/*!
 * This method computes all possible subsets of atomic propositions \f$\alpha \subseteq \mathit{AP}\f$ and their
 * interpretations \f$[\![\alpha]\!]\f$. Here, \f$\mathit{AP}\f$ represents the set of atomic propositions defined in the \ref PolyhedralSystem.
 *
 * The interpretation of a set \f$\alpha\f$ is defined as:
 *
 * \f[
 * [\![\alpha]\!] = \bigcap_{p \in \alpha} [p] \, \cap \, \bigcap_{p \in \mathit{AP} \, \setminus \, \alpha} \overline{[p]}
 * \f]
 *
 * where:
 * - \f$[p]\f$ is the interpretation of the atomic proposition \f$p\f$.
 * - \f$\overline{[p]}\f$ is the complement of \f$[p]\f$.
 *
 * The method iterates over all possible subset sizes \f$k\f$ (from 1 to the total number of atomic propositions of the
 * \ref PolyhedralSystem) and computes the interpretations for each subset.
 *
 * \param filterEmptyObservables If `true`, excludes observables with empty interpretations from the result.
 * \return A vector of \ref Observable objects, each representing a set of atomic propositions and its interpretation.
 *
 * The interpretation of each observable is computed by intersecting the interpretations of the propositions
 * in the subset and the complements of the interpretations of the propositions not in the subset. This ensures
 * that the resulting polyhedron represents the set of points where all and only the proposition in \f$\alpha\f$ hold.
 *
 * For any two sets of atomic propositions \f$\alpha_1, \alpha_2 \subseteq \mathit{AP}\f$, we have that either
 * \f$[\![\alpha_1]\!] = [\![\alpha_2]\!]\f$ or \f$[\![\alpha_1]\!] \, \cap \, [\![\alpha_2]\!] = \emptyset\f$.
 * Hence, the image of \f$2^{\mathit{AP}}\f$ under \f$[\![\cdot]\!]\f$ is a partition of \f$\mathbb{R}^n\f$ into polyhedra.
 *
 * \note If the \c DEBUG directive is enabled, each observable will include a string representation
 *       of its interpretation, where variable names match those defined in the \ref PolyhedralSystem.
 *       This is useful for debugging and visualizing the polyhedral interpretations
 *
 * \throws None
 *
 * \see Observable
 * \see AtomInterpretation
 */
std::vector<Observable> PolyhedralSystem::generateObservables(const bool filterEmptyObservables) const
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

        for (std::vector kCombination: kCombinations)
        {
            PowersetSharedPtr observableDenotation { std::make_shared<Powerset>(spaceDimension(), PPL::UNIVERSE) };
            spot::atomic_prop_set observableAtoms {};

            for (const spot::formula& observableAtom: kCombination)
            {
                const AtomInterpretation* atomInterpretation { *getAtomInterpretation(observableAtom) };
                observableDenotation->intersection_assign(atomInterpretation->interpretation());
                observableAtoms.insert(observableAtom);
            }

            for (const spot::formula& polyhedralAtom: polyhedralAtoms)
            {
                if (!observableAtoms.count(polyhedralAtom))
                {
                    const AtomInterpretation* atomInterpretation { *getAtomInterpretation(polyhedralAtom) };
                    observableDenotation->intersection_assign(atomInterpretation->notInterpretation());
                }
            }

            if (filterEmptyObservables && observableDenotation->is_empty()) continue;

#ifdef DEBUG
            Observable observable { observableAtoms, observableDenotation, PPLOutput::toString(*observableDenotation, symbolTable()) };
#else
            Observable observable { observableAtoms, observableDenotation };
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
        const AtomInterpretation* atomInterpretation { *polyhedralSystem.getAtomInterpretation(atom) };
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
