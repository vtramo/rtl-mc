#pragma once

#include <spot/twa/fwd.hh>
#include <variant>
#include <spot/twa/formula2bdd.hh>
#include <spot/twaalgos/dot.hh>

#include "ObservableTraversalNode.h"
#include "PolyhedralSystem.h"

class PolyhedralAbstraction
{
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    explicit PolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem)
        : m_polyhedralSystem { polyhedralSystem }
    {}

    virtual ~PolyhedralAbstraction() = default;

    [[nodiscard]] virtual PowersetConstSharedPtr points(unsigned state) const = 0;

    [[nodiscard]] virtual const Observable& observable(unsigned state) const = 0;

    [[nodiscard]] virtual unsigned initialState() const
    {
        return m_graph->get_init_state_number();
    }

    [[nodiscard]] virtual unsigned totalStates() const
    {
        return m_graph->num_states();
    }

    [[nodiscard]] virtual unsigned totalEdges() const
    {
        return m_graph->num_edges();
    }

    [[nodiscard]] virtual EdgeIterator successors(const unsigned state)
    {
        assertThatStateIsInRange(state);

        return m_graph->out(state);
    }

    [[nodiscard]] virtual bool hasSuccessors(const unsigned state)
    {
        assertThatStateIsInRange(state);

        auto edgeStorages { m_graph->out(state) };
        return edgeStorages.begin() != edgeStorages.end();
    }

    [[nodiscard]] virtual int countSuccessors(const unsigned state)
    {
        assertThatStateIsInRange(state);

        const EdgeIterator& edgeIterator { m_graph->out(state) };
        return std::distance(edgeIterator.begin(), edgeIterator.end());
    }

    [[nodiscard]] virtual PPL::dimension_type spaceDimension() const { return m_spaceDimension; }

    [[nodiscard]] virtual spot::const_twa_graph_ptr twa() const { return m_graph; }

    void printDotFormat(std::ostream& os) const { spot::print_dot(os, m_graph); }

protected:
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    spot::twa_graph_ptr m_graph {};
    PPL::dimension_type m_spaceDimension {};

    virtual void initializeGraph()
    {
        m_graph = std::make_shared<spot::twa_graph>(m_polyhedralSystem->bddDict());
        m_graph->prop_state_acc(spot::trival { true });
        m_graph->set_acceptance(spot::acc_cond::acc_code::t());
    }

    virtual bdd observableAsBdd(const Observable& observable)
    {
        const spot::atomic_prop_set& observableAtoms { observable.atoms() };
        return spot::formula_to_bdd(SpotUtils::andAtoms(observableAtoms), m_graph->get_dict(), m_graph);
    }

    virtual void assertThatStateIsInRange(const unsigned state) const
    {
        if (state >= totalStates())
            throw std::invalid_argument("State is out of range!");
    }
};

