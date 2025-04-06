/**
* \file Automaton.h
 * \brief Abstract base class for automata
 */

#pragma once

#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/sccinfo.hh>

#include "AutomatonStats.h"

/**
 * \class Automaton
 * \brief Base abstract class for automata
 *
 * The implementation builds upon `spot::twa_graph` graph
 * \see https://spot.lre.epita.fr/doxygen/classspot_1_1twa__graph.html
 */
class Automaton
{
public:
    /// Iterator type for state successors (Spot's internal representation)
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    /// Edge type (Spot's internal representation)
    using Edge = spot::internal::edge_storage<unsigned, unsigned, unsigned, spot::internal::boxed_label<spot::twa_graph_edge_data>>;

    /**
     * \brief Default constructor (creates empty automaton)
     */
    Automaton();

    /**
     * \brief Named constructor (delayed initialization)
     * \param name \c Automaton name
     */
    explicit Automaton(std::string_view name);

    /**
     * \brief Pre-initialized constructor
     * \param name Automaton identifier
     * \param automaton Pre-built graph
     */
    explicit Automaton(std::string_view name, spot::twa_graph_ptr automaton);

    virtual ~Automaton() = default;

    /**
     * \brief Gets the \c Automaton 's name
     * \return Const view of the name string
     */
    [[nodiscard]] std::string_view name() const { return m_name; }

    /**
     * \brief Gets the total number of states
     * \return Number of states in the \c Automaton
     */
    [[nodiscard]] virtual unsigned totalStates() const;

    /**
     * \brief Gets the total number of initial states
     * \return Number of initial states in the \c Automaton
     */
    [[nodiscard]] virtual unsigned totalInitialStates() const;

    /**
     * \brief Gets the total number of accepting states
     * \return Number of accepting states in the \c Automaton
     */
    [[nodiscard]] virtual unsigned totalAcceptingStates() const;

    /**
     * \brief Gets the total number of edges
     * \return Number of edges in the \c Automaton
     */
    [[nodiscard]] virtual unsigned totalEdges() const;

    /**
     * \brief Check if a state is initial
     * \param state State index to check
     * \throws std::out_of_range if state is out of range
     * \return `true` if the state is initial, `false` otherwise
     */
    [[nodiscard]] virtual bool isInitialState(unsigned state) const;

    /**
     * \brief Checks if a state is accepting
     * \param state State index to check
     * \throws std::out_of_range if state is out of range
     * \return `true` if the state is accepting, `false` otherwise
     */
    [[nodiscard]] virtual bool isAcceptingState(unsigned state) const;

    /**
     * \brief Gets successor iterator for a state
     * \param state Source state index
     * \throws std::out_of_range if state is out of range
     * \return Iterator range over outgoing edges
     */
    [[nodiscard]] virtual EdgeIterator successors(unsigned state) const;

    /**
     * \brief Checks if a state has successors
     * \param state State index to check
     * \throws std::out_of_range if state is out of range
     * \return `true` if state has outgoing edges, `false` otherwise
     */
    [[nodiscard]] virtual bool hasSuccessors(unsigned state) const;

    /**
     * \brief Counts state successors
     * \param state Source state index
     * \throws std::out_of_range if state is out of range
     * \return Number of outgoing edges
     */
    [[nodiscard]] virtual int countSuccessors(unsigned state) const;

    /**
     * \brief Gets the underlying Spot automaton
     * \return Shared pointer to const transition graph
     */
    [[nodiscard]] virtual spot::const_twa_graph_ptr twa() const;

    /**
     * \brief Computes transposed \C Automaton
     * \return New automaton with reversed edges
     */
    [[nodiscard]] virtual spot::twa_graph_ptr transpose() const;

    /**
     * \brief Gets \c Automaton statistics
     * \return Const reference to collected metrics
     */
    [[nodiscard]] virtual const AutomatonStats& stats() const;

    /**
     * \brief Outputs automaton in Graphviz DOT format
     * \param os Output stream to write to
     */
    void printDotFormat(std::ostream& os) const;

protected:
    spot::twa_graph_ptr m_automaton {}; ///< Core graph
    std::string m_name {}; ///< Descriptive identifier
    std::shared_ptr<AutomatonStats> m_automatonStats {}; ///< Analysis metrics

    /**
     * \brief Initializes the \c Automaton structure
     */
    virtual void initialiseAutomaton();

    /**
     * \brief Initializes statistics tracking
     */
    virtual void initialiseStats();

    /**
     * \brief Validates state index
     * \param state Index to check
     * \throws std::out_of_range if state is out of range
     */
    virtual void assertThatStateIsInRange(unsigned state) const;

    /**
     * \brief Updates \c Automaton statistics
     * \param executionTimeSeconds Total construction time
     * \post It should populate all \c Automaton statistics
     */
    virtual void setAutomatonStats(double executionTimeSeconds);
};
