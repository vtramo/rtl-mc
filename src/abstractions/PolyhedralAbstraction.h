/**
* \file PolyhedralAbstraction.h
* \brief Abstract base class representing a \c PolyhedralAbstraction of a \c PolyhedralSystem
*/

#pragma once

#include <spot/twa/formula2bdd.hh>

#include "logger.h"
#include "ObservableTraverseNode.h"
#include "PolyhedralSystem.h"
#include "Automaton.h"

class PolyhedralAbstraction;

/**
 * \typedef PolyhedralAbstractionConstSharedPtr
 * \brief Shared pointer to a const \c PolyhedralAbstraction
 */
using PolyhedralAbstractionConstSharedPtr = std::shared_ptr<const PolyhedralAbstraction>;

/**
 * \typedef PolyhedralAbstractionSharedPtr
 * \brief Shared pointer to a mutable \c PolyhedralAbstraction
 */
using PolyhedralAbstractionSharedPtr = std::shared_ptr<PolyhedralAbstraction>;

/**
 * \class PolyhedralAbstraction
 * \brief Abstract base class representing a polyhedral abstraction of a \c PolyhedralSystem
 *
 * A polyhedral abstraction is a finite graph representation of a polyhedral system's behavior,
 * formally defined as a tuple \f$G = (V, E, pts, obs)\f$, where:
 * - \f$(V, E)\f$ represents a finite graph;
 * - \f$obs(v) \subseteq \text{AP} \cup { sing }\f$ is a subset of atomic propositions (an \c Observable);
 * - \f$pts(v) \subseteq [\![ obs(v) \cap AP ]\!]\f$ is a possibly non-convex polyhedron that represents the
 *   interpretation of an \c Observable.
 *
 * Polyhedral abstractions are meant to represent the relevant behaviours of polyhedral systems.
 *
 * \note A \c PolyhedralAbstraction is represented as an automaton in which all states are accepting.
 *       This design simplifies the synchronous product of a \c PolyhedralAbstraction with another \c Automaton.
 *
 * \extends Automaton
 *
 * \see Observable
 * \see PolyhedralSystem
 * \see Automaton
 */
class PolyhedralAbstraction: public Automaton
{
public:
    /**
     * \brief Constructs a \c PolyhedralAbstraction
     * \param polyhedralSystem The \c PolyhedralSystem being abstracted
     * \param name Name identifier for the \c PolyhedralAbstraction
     */
    explicit PolyhedralAbstraction(PolyhedralSystemSharedPtr polyhedralSystem, const std::string_view name)
        : Automaton(name)
        , m_polyhedralSystem { polyhedralSystem }
    {
        m_spaceDimension = m_polyhedralSystem->spaceDimension();
        PolyhedralAbstraction::initialiseAutomaton();
        Log::log(Verbosity::verbose, "\n[{}] Construction started.", name);
    }

    ~PolyhedralAbstraction() override = default;

    /**
     * \brief Retrieves the polyhedron associated with a given \p state
     * \param state A state of the \c PolyhedralAbstraction
    *  \return A shared pointer to a constant \c Powerset representing the polyhedron associated with \p state
     * \throws std::invalid_argument if \p state is out of range
     */
    [[nodiscard]] virtual PowersetConstSharedPtr points(unsigned state) const = 0;

    /**
     * \brief Gets the \c Observable associated with a given \p state
     * \param state A state of the \c PolyhedralAbstraction
     * \return Const reference to the \c Observable associated with \p state
     * \throws std::invalid_argument if \p state is out of range
     */
    [[nodiscard]] virtual const Observable& observable(unsigned state) const = 0;

    /**
     * \brief Gets the space dimension of the \c PolyhedralAbstraction
     * \return The dimension of the \c PolyhedralAbstraction
     */
    [[nodiscard]] virtual PPL::dimension_type spaceDimension() const { return m_spaceDimension; }

    /**
     * \brief Gets the initial state
     * \return The initial state number
     */
    [[nodiscard]] virtual unsigned initialState() const
    {
        return m_automaton->get_init_state_number();
    }

    /**
     * \brief Sets the initial state of the \c PolyhedralAbstraction
     * \param state The state of the abstraction to set as initial state
     * \throws std::invalid_argument if \p state is out of range
     * \see PolyhedralAbstraction::initialState() to retrieve the current initial state
     */
    virtual void setInitialState(const unsigned state)
    {
        assertThatStateIsInRange(state);
        m_automaton->set_init_state(state);
    }

    /**
     * \brief Gets the total number of initial states
     * \return Always 1 for a \c PolyhedralAbstraction
     */
    [[nodiscard]] unsigned totalInitialStates() const override { return 1; }

    /**
     * \brief Gets the total number of accepting states
     * \return Total states (all states are accepting in a \c PolyhedralAbstraction)
     */
    [[nodiscard]] unsigned totalAcceptingStates() const override { return totalStates(); }

    /**
     * \brief Converts an \c Observable to its corresponding BDD representation (conjunction of the atomic propositions of the \c Observable)
     * \param observable An \c Observable contained within the \c PolyhedralAbstraction
     * \return A BDD representing the conjunction of the atomic propositions of the \c Observable (\ref Observable::atoms())
     * \see https://spot.lre.epita.fr/tut90.html
     */
    virtual bdd observableAsBdd(const Observable& observable) const
    {
        const spot::atomic_prop_set& observableAtoms { observable.atoms() };
        return spot::formula_to_bdd(andFormulae(observableAtoms), m_automaton->get_dict(), m_automaton);
    }

protected:
    PolyhedralSystemSharedPtr m_polyhedralSystem {}; ///< The \c PolyhedralSystem being abstracted
    PPL::dimension_type m_spaceDimension {}; //< Space dimension of the \c PolyhedralSystem

    /**
     * \brief Initializes the automaton structure
     *
     * Sets up:
     * - Shared BDD dictionary from the \c PolyhedralSystem
     * - State-based acceptance
     * - True acceptance condition (all states accepting)
     */
    void initialiseAutomaton() override
    {
        m_automaton = std::make_shared<spot::twa_graph>(m_polyhedralSystem->bddDict());
        m_automaton->prop_state_acc(spot::trival { true });
        m_automaton->set_acceptance(spot::acc_cond::acc_code::t());
    }
};

