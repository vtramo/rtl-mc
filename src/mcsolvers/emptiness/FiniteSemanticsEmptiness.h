#pragma once

#include <spot/twaalgos/reachiter.hh>
#include "PolyhedralSynchronousProductAutomaton.h"
#include "EmptinessCheckDenotationResult.h"
#include "Timer.h"

class FiniteSemanticsEmptiness
{
public:
    explicit FiniteSemanticsEmptiness(PolyhedralSynchronousProductAutomatonConstSharedPtr synchronousProduct)
        : m_transposedSynchronousProduct { synchronousProduct->transpose() }
    {
        m_synchronousProduct = synchronousProduct;
    }

    EmptinessCheckDenotationResult operator() ()
    {
        return run();
    }

    EmptinessCheckDenotationResult run()
    {
        resetResult();

        Timer timer {};
        Dfs dfs { m_transposedSynchronousProduct, this };
        for (const unsigned acceptingState: m_synchronousProduct->acceptingStates())
        {
            m_transposedSynchronousProduct->set_init_state(acceptingState);
            dfs.run();
        }

        m_emptinessCheckDenotationResult.elapsedTimeInSeconds = timer.elapsedInSeconds();
        return m_emptinessCheckDenotationResult;
    }

    EmptinessCheckDenotationResult result() { return m_emptinessCheckDenotationResult; }

private:
    spot::twa_graph_ptr m_transposedSynchronousProduct {};
    PolyhedralSynchronousProductAutomatonConstSharedPtr m_synchronousProduct {};
    EmptinessCheckDenotationResult m_emptinessCheckDenotationResult {};

    void resetResult()
    {
        m_emptinessCheckDenotationResult = {};
        m_emptinessCheckDenotationResult.result = std::make_shared<Powerset>(m_synchronousProduct->spaceDimension(), PPL::EMPTY);
    }

    class Dfs : public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit Dfs(const spot::const_twa_ptr& a, FiniteSemanticsEmptiness* parent)
            : twa_reachable_iterator_depth_first(a)
            , m_parent { parent }
        {
        }

        void process_state(const spot::state* state, [[maybe_unused]] const int _, spot::twa_succ_iterator* si) override
        {
            unsigned productStateNumber { m_parent->m_transposedSynchronousProduct->state_number(state) };
            const bool isInitialState { m_parent->m_synchronousProduct->isInitialState(productStateNumber) };
            if (isInitialState)
            {
                m_parent->addInitialStateDenotationToResult(productStateNumber);
            }
        }
    protected:
        FiniteSemanticsEmptiness* m_parent {};
    };

    void addInitialStateDenotationToResult(const unsigned state)
    {
        PowersetConstSharedPtr points { m_synchronousProduct->points(state) };
        PPLUtils::fusion(*m_emptinessCheckDenotationResult.result, *points);
        m_emptinessCheckDenotationResult.initialStates.insert(state);
        m_emptinessCheckDenotationResult.totalAcceptingRuns++;
    }
};
