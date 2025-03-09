#pragma once

#include <spot/twa/formula2bdd.hh>

#include "ObservableTraversalNode.h"
#include "PolyhedralSystem.h"
#include "Automaton.h"

class PolyhedralAbstraction;
using PolyhedralAbstractionConstSharedPtr = std::shared_ptr<PolyhedralAbstraction>;

class PolyhedralAbstraction: public Automaton
{
public:
    explicit PolyhedralAbstraction(PolyhedralSystemConstSharedPtr polyhedralSystem, const std::string_view name)
        : Automaton(name)
        , m_polyhedralSystem { polyhedralSystem }
    {
        PolyhedralAbstraction::initializeAutomaton();
    }

    virtual ~PolyhedralAbstraction() override = default;

    [[nodiscard]] virtual PowersetConstSharedPtr points(unsigned state) const = 0;
    [[nodiscard]] virtual const Observable& observable(unsigned state) const = 0;
    [[nodiscard]] virtual PPL::dimension_type spaceDimension() const { return m_spaceDimension; }
    [[nodiscard]] virtual unsigned initialState() const
    {
        return m_automaton->get_init_state_number();
    }

protected:
    PolyhedralSystemConstSharedPtr m_polyhedralSystem {};
    PPL::dimension_type m_spaceDimension {};

    void initializeAutomaton() override
    {
        m_automaton = std::make_shared<spot::twa_graph>(m_polyhedralSystem->bddDict());
        m_automaton->prop_state_acc(spot::trival { true });
        m_automaton->set_acceptance(spot::acc_cond::acc_code::t());
    }

    virtual bdd observableAsBdd(const Observable& observable)
    {
        const spot::atomic_prop_set& observableAtoms { observable.atoms() };
        return spot::formula_to_bdd(SpotUtils::andAtoms(observableAtoms), m_automaton->get_dict(), m_automaton);
    }
};

