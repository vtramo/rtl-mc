#pragma once

#include <optional>
#include <spot/tl/formula.hh>

#include "ppl_aliases.h"
#include "PolyhedralSystemBuilder.h"
#include "PolyhedralSystemSymbolTable.h"
#include "AtomInterpretation.h"
#include "Observable.h"

class PolyhedralSystem;

/*!
 *  \brief Type alias for a shared pointer to a \ref PolyhedralSystem.
 *
 * This type is used to manage shared ownership of a dynamically allocated \ref PolyhedralSystem object.
 * The object will be automatically deleted when the last shared pointer referencing it goes out of scope.
 */
using PolyhedralSystemSharedPtr = std::shared_ptr<PolyhedralSystem>;

/*!
 *  \brief Type alias for a shared pointer to a const \ref PolyhedralSystem.
 *
 * This type is used to manage shared ownership of a dynamically allocated const \ref PolyhedralSystem object.
 * The object will be automatically deleted when the last shared pointer referencing it goes out of scope.
 */
using PolyhedralSystemConstSharedPtr = std::shared_ptr<const PolyhedralSystem>;

/*!
 *  \brief Type alias for a unique pointer to a \ref PolyhedralSystem.
 *
 * This type is used to manage unique ownership of a dynamically allocated \ref PolyhedralSystem object.
 * The object will be automatically deleted when the pointer referencing it goes out of scope.
 */
using PolyhedralSystemUniquePtr = std::unique_ptr<PolyhedralSystem>;


/*!
 * \class PolyhedralSystem
 * \brief Represents a polyhedral system, which is a dynamic system governed by differential inclusion constraints.
 *
 * A polyhedral system is defined by a triple \f$\mathcal{P} = (\mathit{Flow}, \mathit{Inv}, [\cdot])\f$, where:
 * - \f$\mathit{Flow} \subseteq \mathbb{R}^n\f$ is a convex polyhedron representing the flow constraint.
 * - \f$\mathit{Inv}\f$ is a closed polyhedron representing the invariant region.
 * - \f$[\cdot]\f$ is a mapping from atomic propositions to their interpretations (general polyhedra, a.k.a. Powerset).
 *
 * This class provides methods to query and manipulate the properties of the polyhedral system, such as its flow,
 * invariant, and interpretations of atomic propositions.
 */
class PolyhedralSystem {
public:
    PolyhedralSystem() = default;

    /*!
     * \brief Returns the dimension of the space in which the polyhedral system operates.
     * \return The space dimension of the \ref PolyhedralSystem
     */
    [[nodiscard]] PPL::dimension_type spaceDimension() const;

    /*!
     * \brief Returns the flow constraint of the polyhedral system (a convex polyhedron).
     * \return A reference to the polyhedron representing the flow constraint.
     */
    [[nodiscard]] const Poly& flow() const;

    /*!
     * \brief Returns the pre-flow polyhedron, which is the reflection of the flow constraint.
     * \return A reference to the pre-flow polyhedron.
     */
    [[nodiscard]] const Poly& preFlow() const;

    /*!
     * \brief Checks if the flow constraint is omnidirectional.
     * \return `true` if the flow constraint is omnidirectional, `false` otherwise.
     * \see isOmnidirectionalFlow(const Poly& flow)
     */
    [[nodiscard]] bool hasOmnidirectionalFlow() const;

    /*!
     * \brief Checks if the flow constraint is bounded.
     * \return `true` if the flow constraint is bounded, `false` otherwise.
     */
    [[nodiscard]] bool hasBoundedFlow() const;

    /*!
     * \brief Checks if the flow constraint is topologically closed.
     * \return `true` if the flow is closed, `false` otherwise.
     */
    [[nodiscard]] bool hasClosedFlow() const;

    /*!
     * \brief Checks if the flow constraint is compact.
     * \return `true` if the flow is compact, `false` otherwise.
     */
    [[nodiscard]] bool hasCompactFlow() const;

    /*!
     * \brief Checks if the cone of the flow is closed.
     * \return `true` if the cone of the flow is closed, `false` otherwise.
     * \see coneGeometric
     * \see coneGenerator
     */
    [[nodiscard]] bool hasFlowWithClosedCone() const;

    /*!
     * \brief Checks if movement is forced in the system (i.e., the origin is not in the flow closure).
     * \return `true` if movement is forced, `false` otherwise.
     */
    [[nodiscard]] bool isMovementForced() const;

    /*!
     * \brief Returns the invariant region of the polyhedral system.
     * \return A reference to the polyhedron representing the invariant region.
     */
    [[nodiscard]] const Powerset& invariant() const;

    /*!
     * \brief Checks if the invariant region is bounded.
     * \return `true` if the invariant is bounded, `false` otherwise.
     */
    [[nodiscard]] bool hasBoundedInvariant() const;

    /*!
     * \brief Checks if the invariant region is topologically closed.
     * \return `true` if the invariant is closed, `false` otherwise.
     */
    [[nodiscard]] bool hasClosedInvariant() const;

    /*!
     * \brief Checks if the invariant region is compact.
     * \return `true` if the invariant is compact, `false` otherwise.
     */
    [[nodiscard]] bool hasCompactInvariant() const;

    /*!
     * \brief Returns the symbol table associated with the polyhedral system.
     * \return A reference to the \ref PolyhedralSystemSymbolTable.
     */
    [[nodiscard]] const PolyhedralSystemSymbolTable& symbolTable() const;

    /*!
     * \brief Returns the set of atomic propositions defined in the system.
     * \return A reference to the set of atomic propositions.
     */
    [[nodiscard]] const spot::atomic_prop_set& atoms() const;

    /*!
     * \brief Checks if the system contains a specific atomic proposition.
     * \param atom The atomic proposition to check.
     * \return `true` if the atom exists in the system, `false` otherwise.
     */
    [[nodiscard]] bool containsAtom(spot::formula atom) const;

    /*!
     * \overload
     * \see containsAtom(spot::formula atom) const
     */
    [[nodiscard]] bool containsAtom(std::string_view atom) const;

    /*!
     * \brief Returns the total number of atomic propositions in the system.
     * \return The number of atomic propositions.
     */
    [[nodiscard]] int totalAtoms() const;

    /*!
     * \brief Retrieves the interpretation of an atomic proposition by name.
     * \param atom The name of the atomic proposition.
     * \return An optional containing a pointer to the \ref AtomInterpretation if the atom exists, or `std::nullopt` otherwise.
     */
    [[nodiscard]] std::optional<const AtomInterpretation* const> getAtomInterpretation(std::string_view atom) const;

    /*!
     * \brief Retrieves the interpretation of an atomic proposition.
     * \param atom The atomic proposition.
     * \return An optional containing a pointer to the \ref AtomInterpretation if the atom exists, or `std::nullopt` otherwise.
     */
    [[nodiscard]] std::optional<const AtomInterpretation* const> getAtomInterpretation(const spot::formula& atom) const;

    /*!
     * \brief Create a new \c PolyhedralSystem by extending it with new atomic propositions and their interpretations.
     * \param atomInterpretations A vector of pairs, each consisting of an atomic proposition and its corresponding interpretation.
     * \return A new \ref PolyhedralSystem instance that extends the original system with the added atomic proposition interpretations.
     * \throws std::invalid_argument If any of the atoms already exists or are not valid atomic propositions.
     */
    [[nodiscard]] PolyhedralSystemUniquePtr extend(const std::vector<std::pair<spot::formula, Powerset>>& atomInterpretations) const;

    /*!
     * \overload
     * \see PolyhedralSystem::extend(const std::vector<std::pair<spot::formula, Powerset>>& atomInterpretations)
     */
    [[nodiscard]] PolyhedralSystemUniquePtr extend(const std::vector<std::pair<std::string_view, Powerset>>& atomInterpretations) const;

    /*!
     * \overload
     */
    [[nodiscard]] PolyhedralSystemUniquePtr extend(const spot::formula& atom, const Powerset& interpretation) const;

    /*!
     * \overload
     */
    [[nodiscard]] PolyhedralSystemUniquePtr extend(std::string_view atom, const Powerset& interpretation) const;

    /*!
     * \brief Returns the BDD dictionary used by the system.
     * \return A shared pointer to the BDD dictionary.
     * \see https://spot.lre.epita.fr/tut90.html
     */
    [[nodiscard]] spot::bdd_dict_ptr bddDict() const;

    /*!
     * \brief Generates all possible observables for the polyhedral system.
     * \param filterEmptyObservables If `true`, excludes observables with empty interpretations.
     * \return A vector of \ref Observable objects.
     */
    const std::vector<Observable>& getOrGenerateObservables(bool filterEmptyObservables = true);

    /*!
     * \brief Creates an empty \c Observable, with no atomic propositions.
     * \return An empty \c Observable instance.
     * \see generateObservables
     */
    const Observable& getOrGenerateEmptyObservable();

    /*!
     * \brief Sets whether constraint output should be minimized.
     * \param minimize If `true`, minimizes the output of constraints.
     */
    void setConstraintOutputMinimized(bool minimize);

    /*!
     * \brief Determines whether the \c PolyhedralSystem output should include additional details.
     * \param value If `false`, limits the output to essential, standard-compliant elements (system specification).
     *              If `true`, allows the inclusion of supplementary information that
     *              might not adhere to the standard.
     */
    void setOutputExtraInformation(bool value);

    /*!
     * \brief Creates a builder for constructing \ref PolyhedralSystem objects.
     * \return A \ref PolyhedralSystemBuilder instance.
     */
    [[nodiscard]] static PolyhedralSystemBuilder builder();

    PolyhedralSystem(const PolyhedralSystem&) = default;

    PolyhedralSystem& operator= (const PolyhedralSystem&) = delete;

     /*!
     * \brief Move constructor for \ref PolyhedralSystem.
     * \param polyhedralSystem The polyhedral system to move.
     */
    PolyhedralSystem(PolyhedralSystem&& polyhedralSystem) noexcept;

    /*!
     * \brief Compares two polyhedral systems for equality.
     * \param polyhedralSystem1 The first polyhedral system.
     * \param polyhedralSystem2 The second polyhedral system.
     * \return `true` if the systems are equal, `false` otherwise.
     */
    friend bool operator== (const PolyhedralSystem& polyhedralSystem1, const PolyhedralSystem& polyhedralSystem2);
    friend std::istream& operator>> (std::istream&, PolyhedralSystem&);

protected:
    Powerset m_invariant { 0, PPL::EMPTY };
    Poly m_flow { 0, PPL::EMPTY };
    bool m_hasOmnidirectionalFlow { false };
    bool m_hasFlowWithClosedCone { false };
    bool m_isMovementForced { false };
    Poly m_preFlow { 0, PPL::EMPTY };
    std::optional<std::vector<Observable>> m_observables {};
    std::optional<Observable> m_emptyObservable {};
    spot::bdd_dict_ptr m_bddDict {};
    std::unordered_map<spot::formula, AtomInterpretation> m_denotation {};
    PolyhedralSystemSymbolTable m_symbolTable {};
    bool m_minimizeConstraintsOutput { false };
    bool m_outputExtraInformation { true };

    /*!
     * \brief Constructs a polyhedral system with the given invariant, flow, denotation, and symbol table.
     * \param invariant The invariant region.
     * \param flow The flow constraint.
     * \param denotation The mapping from atomic propositions to their interpretations.
     * \param symbolTable The symbol table.
     */
    PolyhedralSystem(
        const Powerset& invariant,
        const Poly& flow,
        const std::unordered_map<spot::formula, AtomInterpretation>& denotation,
        const PolyhedralSystemSymbolTable& symbolTable
    );

    /*!
     * \brief Constructs a polyhedral system by moving the given invariant, flow, denotation, and symbol table.
     * \param invariant The invariant region.
     * \param flow The flow constraint.
     * \param denotation The mapping from atomic propositions to their interpretations.
     * \param symbolTable The symbol table.
     */
    PolyhedralSystem(
        Powerset&& invariant,
        Poly&& flow,
        std::unordered_map<spot::formula, AtomInterpretation>&& denotation,
        PolyhedralSystemSymbolTable&& symbolTable
    );

    /*!
     * \brief Move assignment operator for \ref PolyhedralSystem.
     * \param polyhedralSystem The polyhedral system to move.
     * \return A reference to the current object.
     */
    PolyhedralSystem& operator= (PolyhedralSystem&& polyhedralSystem) noexcept;

    friend std::ostream& operator<< (std::ostream&, const PolyhedralSystem&);

    /*!
     * \brief Declares \c PolyhedralSystemBuilder as a friend class.
     *
     * The \c PolyhedralSystemBuilder class is granted friend access to \c PolyhedralSystem because
     * it is responsible for constructing \c PolyhedralSystem objects. Since \c PolyhedralSystem
     * can only be constructed through \c PolyhedralSystemBuilder, the builder needs access to the
     * private members of \c PolyhedralSystem to properly initialize them.
     *
     * \see PolyhedralSystemBuilder
     */
    friend class PolyhedralSystemBuilder;

    /*!
     * \brief Initializes the BDD dictionary used by the system.
     * \see https://spot.lre.epita.fr/tut90.html
     */
    void makeBddDict();

    /*!
     * \brief Computes the pre-flow polyhedron by reflecting the flow constraint.
     * \see Poly& reflectionAffineImage(Poly& polyhedron)
     */
    void computePreFlow();

    /*!
     * \brief Evaluates properties of the flow, such as whether it is omnidirectional or forces movement.
     */
    void evaluateFlowProperties();

    /*!
     * \brief Adds a new atomic proposition with its interpretation.
     * \param atom The atomic proposition.
     * \param interpretation The polyhedron representing the interpretation of the atomic proposition.
     * \return A reference to the newly added \ref AtomInterpretation.
     * \throws std::invalid_argument If the atom already exists or is not an atomic proposition.
     */
    const AtomInterpretation& addAtomInterpretation(const spot::formula& atom, const Powerset& interpretation);

    /*!
     * \overload
     * \see addAtomInterpretation(const spot::formula& atom, const Powerset& interpretation)
     */
    const AtomInterpretation& addAtomInterpretation(std::string_view atom, const Powerset& interpretation);
};

/*!
 * \brief Outputs the polyhedral system to a stream.
 * \param out The output stream.
 * \param polyhedralSystem The polyhedral system to output.
 * \return The output stream.
 */
std::ostream& operator<< (std::ostream& out, const PolyhedralSystem& polyhedralSystem);
