#pragma once

#include <spot/twaalgos/reachiter.hh>
#include "PolyhedralSynchronousProductAutomaton.h"

class ExistentialDenotationFiniteSemanticsDfs
{
public:
    explicit ExistentialDenotationFiniteSemanticsDfs(PolyhedralSynchronousProductAutomatonConstSharedPtr synchronousProduct)
        : m_transposedSynchronousProduct { synchronousProduct->transpose() }
    {
        m_synchronousProduct = synchronousProduct;
        m_result = std::make_shared<Powerset>(synchronousProduct->spaceDimension(), PPL::EMPTY);
    }

    PowersetSharedPtr operator() ()
    {
        return run();
    }

    PowersetSharedPtr run()
    {
        if (!m_result->is_empty())
        {
            m_result = std::make_shared<Powerset>(m_synchronousProduct->spaceDimension(), PPL::EMPTY);
        }

        Dfs dfs { m_transposedSynchronousProduct, this };
        for (const unsigned acceptingState: m_synchronousProduct->acceptingStates())
        {
            m_transposedSynchronousProduct->set_init_state(acceptingState);
            dfs.run();
        }

        return m_result;
    }

    PowersetSharedPtr result() { return m_result; }
private:
    spot::twa_graph_ptr m_transposedSynchronousProduct {};
    PolyhedralSynchronousProductAutomatonConstSharedPtr m_synchronousProduct {};
    PowersetSharedPtr m_result {};

    class Dfs : public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit Dfs(const spot::const_twa_ptr& a, ExistentialDenotationFiniteSemanticsDfs* parent)
            : twa_reachable_iterator_depth_first(a)
            , m_parent { parent }
        {

        }

        void process_state(const spot::state* state, const int _, spot::twa_succ_iterator* __) override
        {
            unsigned productStateNumber { m_parent->m_synchronousProduct->twa()->state_number(state) };
            if (m_parent->m_synchronousProduct->isInitialState(productStateNumber))
                m_parent->addInitialStateDenotationToResult(productStateNumber);
        }
    protected:
        ExistentialDenotationFiniteSemanticsDfs* m_parent {};
    };

    void addInitialStateDenotationToResult(const unsigned state)
    {
        PowersetConstSharedPtr points { m_synchronousProduct->points(state) };
        PPLUtils::fusion(*m_result, *points);
    }
};
