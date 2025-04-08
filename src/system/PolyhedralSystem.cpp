#include <utility>
#include <spot/twa/bdddict.hh>

#include "utils.h"
#include "ppl_utils.h"
#include "PolyhedralSystem.h"
#include "cone.h"
#include "ppl_output.h"
#include "systemparser.h"
#include "reflection.h"
#include "omnidirectional.h"
#include "formula.h"
#include "closed.h"

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

bool PolyhedralSystem::hasBoundedFlow() const
{
    return m_flow.is_bounded();
}

/*!
 * The flow constraint is compact iff is a closed and bounded region.
 *
 * \see hasClosedFlow
 * \see hasBoundedFlow
 */
bool PolyhedralSystem::hasCompactFlow() const
{
    return hasClosedFlow() && hasBoundedFlow();
}

bool PolyhedralSystem::isMovementForced() const
{
    return m_isMovementForced;
}

bool PolyhedralSystem::hasFlowWithClosedCone() const
{
    return m_hasFlowWithClosedCone;
}

const Powerset& PolyhedralSystem::invariant() const
{
    return m_invariant;
}

bool PolyhedralSystem::hasBoundedInvariant() const
{
    return m_invariant.is_bounded();
}

bool PolyhedralSystem::hasClosedInvariant() const
{
    return isClosed(m_invariant);
}

/*!
 * The invariant is compact iff is a closed and bounded region.
 *
 * \see hasClosedInvariant
 * \see hasBoundedInvariant
 */
bool PolyhedralSystem::hasCompactInvariant() const
{
    return hasClosedInvariant() && hasBoundedInvariant();
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
    {
        return {};
    }

    if (const auto it { m_denotation.find(atom) }; it != m_denotation.end())
    {
        return &it->second;
    }

     return {};
}

const AtomInterpretation& PolyhedralSystem::addAtomInterpretation(const std::string_view atom, const Powerset& interpretation)
{
    return addAtomInterpretation(ap(atom), interpretation);
}

const AtomInterpretation& PolyhedralSystem::addAtomInterpretation(const spot::formula& atom, const Powerset& interpretation)
{
    if (!atom.is(spot::op::ap))
    {
        throw std::invalid_argument("PolyhedralSystem::addAtomInterpretation: formula is not an atomic proposition! " + toFormulaString(atom));
    }

    if (containsAtom(atom))
    {
        throw std::invalid_argument("PolyhedralSystem::addAtomInterpretation: atom " + atom.ap_name() + " already exists!");
    }

    m_symbolTable.addAtom(atom);
    return m_denotation.emplace(atom, AtomInterpretation { interpretation, m_invariant }).first->second;
}

/*!
 * This method creates a new instance of \ref PolyhedralSystem with the specified atomic propositions and their interpretations added.
 * It does not modify the current \ref PolyhedralSystem.
 */
PolyhedralSystemUniquePtr PolyhedralSystem::extend(const std::vector<std::pair<spot::formula, Powerset>>& atomInterpretations) const
{
    PolyhedralSystemUniquePtr extendedPolyhedralSystem { std::make_unique<PolyhedralSystem>(*this) };

    for (const auto& [atom, interpretation]: atomInterpretations)
    {
        extendedPolyhedralSystem->addAtomInterpretation(atom, interpretation);
    }

    return extendedPolyhedralSystem;
}

PolyhedralSystemUniquePtr PolyhedralSystem::extend(const std::vector<std::pair<std::string_view, Powerset>>& atomInterpretations) const
{
    PolyhedralSystemUniquePtr extendedPolyhedralSystem { std::make_unique<PolyhedralSystem>(*this) };
    for (const auto& [atom, interpretation]: atomInterpretations)
        extendedPolyhedralSystem->addAtomInterpretation(atom, interpretation);
    return extendedPolyhedralSystem;
}

PolyhedralSystemUniquePtr PolyhedralSystem::extend(const spot::formula& atom, const Powerset& interpretation) const
{
    PolyhedralSystemUniquePtr extendedPolyhedralSystem { std::make_unique<PolyhedralSystem>(*this) };
    extendedPolyhedralSystem->addAtomInterpretation(atom, interpretation);
    return extendedPolyhedralSystem;
}

PolyhedralSystemUniquePtr PolyhedralSystem::extend(const std::string_view atom, const Powerset& interpretation) const
{
    PolyhedralSystemUniquePtr extendedPolyhedralSystem { std::make_unique<PolyhedralSystem>(*this) };
    extendedPolyhedralSystem->addAtomInterpretation(atom, interpretation);
    return extendedPolyhedralSystem;
}

bool PolyhedralSystem::containsAtom(const std::string_view atom) const
{
    return containsAtom(ap(atom));
}

bool PolyhedralSystem::containsAtom(spot::formula atom) const
{
    if (!atom.is(spot::op::ap))
    {
        return false;
    }

    if (const auto it { m_denotation.find(atom) }; it != m_denotation.end())
    {
        return true;
    }

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

    PowersetUniquePtr flowCone { coneGeometric(m_flow) };
    m_hasFlowWithClosedCone = isClosed(*flowCone);

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

void PolyhedralSystem::setOutputExtraInformation(const bool value)
{
    m_outputExtraInformation = value;
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
 * The interpretation of each observable is computed by intersecting the interpretations of the propositions
 * in the subset and the complements of the interpretations of the propositions not in the subset. This ensures
 * that the resulting polyhedron represents the set of points where all and only the proposition in \f$\alpha\f$ hold.
 *
 * For any two sets of atomic propositions \f$\alpha_1, \alpha_2 \subseteq \mathit{AP}\f$, we have that either
 * \f$[\![\alpha_1]\!] = [\![\alpha_2]\!]\f$ or \f$[\![\alpha_1]\!] \, \cap \, [\![\alpha_2]\!] = \emptyset\f$.
 * Hence, the image of \f$2^{\mathit{AP}}\f$ under \f$[\![\cdot]\!]\f$ is a partition of \f$\mathbb{R}^n\f$ into polyhedra.
 *
 * \note \c Observable s are generated only once, then they are stored. Subsequent calls to \c getOrGenerateObservables
 *       return the stored result.
 *
 * \note If the \c DEBUG directive is enabled, each observable will include a string representation
 *       of its interpretation, where variable names match those defined in the \ref PolyhedralSystem.
 *       This is useful for debugging and visualizing the polyhedral interpretations.
 *
 * \see Observable
 * \see AtomInterpretation
 */
const std::vector<Observable>& PolyhedralSystem::getOrGenerateObservables(const bool filterEmptyObservables)
{
    if (m_observables.has_value())
    {
        return *m_observables;
    }

    // Creates a vector of references to the atoms in PolyhedralSystem
    using AtomRef = std::reference_wrapper<const spot::formula>;
    const int totalAtoms { PolyhedralSystem::totalAtoms() };
    std::vector<AtomRef> atoms {};
    atoms.reserve(totalAtoms);
    for (const spot::formula& polyhedralAtom: PolyhedralSystem::atoms())
    {
        atoms.push_back(polyhedralAtom);
    }

    const long totalSubsets { 1L << totalAtoms };
    m_observables = std::vector<Observable> {};
    m_observables->reserve(totalSubsets);

    // Separately handles the empty observable, as it is useful to store it.
    const Observable& emptyObservable { getOrGenerateEmptyObservable() };
    if (!filterEmptyObservables || !emptyObservable.isInterpretationEmpty())
    {
        m_observables->push_back(emptyObservable);
    }

    for (long subsetIndex { 1 }; subsetIndex < totalSubsets; ++subsetIndex)
    {
        PowersetSharedPtr observableDenotation { std::make_shared<Powerset>(spaceDimension(), PPL::UNIVERSE) };
        spot::atomic_prop_set observableAtoms {};

        for (long atomIndex { 0 }; atomIndex < totalAtoms; ++atomIndex)
        {
            const spot::formula& atom { atoms[atomIndex] };
            const AtomInterpretation* const atomInterpretation { *getAtomInterpretation(atom) };
            const bool isAtomInSubset { ((subsetIndex >> atomIndex & 1) == 1) };
            if (isAtomInSubset)
            {
                observableAtoms.insert(atom);
                observableDenotation->intersection_assign(atomInterpretation->interpretation());
            }
            else
            {
                observableDenotation->intersection_assign(atomInterpretation->notInterpretation());
            }
        }

        if (filterEmptyObservables && observableDenotation->is_empty())
        {
            continue;
        }

#ifdef DEBUG
        Observable observable { observableAtoms, observableDenotation, PPLOutput::toString(*observableDenotation, symbolTable()) };
#else
        Observable observable { observableAtoms, observableDenotation };
#endif
        m_observables->push_back(std::move(observable));
    }

    return *m_observables;
}

/*!
 * This method creates an empty \c Observable, where the interpretation is defined as the intersection of the
 * negations of all atomic propositions in the system.
 *
 * Specifically, for each atomic proposition \f$p\f$ in the set \f$\mathit{AP}\f$, the interpretation of the empty
 * observable is the intersection of the negated interpretations \f$\overline{[p]}\f$, where \f$[p]\f$ is
 * the interpretation of the atomic proposition \f$p\f$.
 *
 * The interpretation of the empty observable \f$\emptyset\f$ is computed as:
 *
 * \f[
 * [\![\emptyset]\!] = \bigcap_{p \in \mathit{AP}} \overline{[p]}
 * \f]
 *
 * This ensures that the resulting observable represents the set of points where none of the atomic propositions hold.
 *
 * \note The empty \c Observable is generated only once, then it is stored. Subsequent calls to
 *       \c getOrGenerateObservable return the stored result.
 */
const Observable& PolyhedralSystem::getOrGenerateEmptyObservable()
{
    if (m_emptyObservable.has_value())
    {
        return *m_emptyObservable;
    }

    PowersetSharedPtr observableDenotation { std::make_shared<Powerset>(spaceDimension(), PPL::UNIVERSE) };

    for (const auto& atom: atoms())
    {
        const AtomInterpretation* atomInterpretation { *getAtomInterpretation(atom) };
        observableDenotation->intersection_assign(atomInterpretation->notInterpretation());
    }

#ifdef DEBUG
    m_emptyObservable = { {}, observableDenotation, PPLOutput::toString(*observableDenotation, symbolTable()) };
#else
    m_emptyObservable = { {}, observableDenotation };
#endif

    return *m_emptyObservable;
}

std::ostream& operator<< (std::ostream& out, const PolyhedralSystem& polyhedralSystem)
{
    const bool minimizeConstraints { polyhedralSystem.m_minimizeConstraintsOutput };
    const PolyhedralSystemSymbolTable& symbolTable { polyhedralSystem.symbolTable() };
    out << "Inv " << PPLOutput::toString(polyhedralSystem.invariant(), symbolTable, minimizeConstraints) << '\n';
    out << "Flow " << PPLOutput::toString(polyhedralSystem.flow(), symbolTable, minimizeConstraints) << '\n';

    bool first { true };
    for (const auto& atom: symbolTable.atoms())
    {
        if (!first) out << std::endl;
        first = false;
        const AtomInterpretation* atomInterpretation { *polyhedralSystem.getAtomInterpretation(atom) };
        out << atom << " " << PPLOutput::toString(atomInterpretation->interpretation(), symbolTable, minimizeConstraints);
    }

    if (!polyhedralSystem.m_outputExtraInformation)
    {
        return out;
    }

    out << "Total atomic propositions: " << polyhedralSystem.totalAtoms() << ".\n";
    out << "PreFlow: " << PPLOutput::toString(polyhedralSystem.preFlow(), polyhedralSystem.symbolTable()) << '\n';

    out << "Space dimension: " << polyhedralSystem.spaceDimension() << ".\n";
    first = true;
    out << "Variables: [";
    for (PPL::dimension_type dim = 0; dim < polyhedralSystem.spaceDimension(); ++dim)
    {
        out << (first ? "" : ", ") << *polyhedralSystem.symbolTable().getVariableName(PPL::Variable { dim });
        first = false;
    }
    out << "]\n";
    return out;
}
