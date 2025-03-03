#include "FiniteSynchronousProduct.h"

FiniteSynchronousProduct::FiniteSynchronousProduct(
    FiniteLtlAutomatonConstSharedPtr nfa,
    OmnidirectionalPolyhedralAbstractionConstSharedPtr abstraction
)
    : m_nfa { nfa }
    , m_abstraction { abstraction }
{
    spot::const_twa_graph_ptr automatonTwa { nfa->twa() };
    spot::const_twa_graph_ptr abstractionTwa { abstraction->twa() };
    m_synchronousProduct = spot::product(automatonTwa, abstractionTwa);
    m_productStatePair = m_synchronousProduct->get_named_prop<spot::product_states>("product-states");
    buildAutomaton();
}

void FiniteSynchronousProduct::buildAutomaton()
{
    class Dfs: public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit Dfs(FiniteSynchronousProduct* finiteSynchronousProduct)
            : twa_reachable_iterator_depth_first(finiteSynchronousProduct->twa())
        {
            m_finiteSynchronousProduct = finiteSynchronousProduct;
        }

        void process_state(const spot::state* state, const int _, spot::twa_succ_iterator* __) override
        {
            unsigned productStateNumber { m_finiteSynchronousProduct->twa()->state_number(state) };
            auto [s, v] { m_finiteSynchronousProduct->m_productStatePair->at(productStateNumber) };
            PowersetConstSharedPtr productStateDenotation { m_finiteSynchronousProduct->m_abstraction->points(v) };
            m_finiteSynchronousProduct->m_denotationByState[productStateNumber] = productStateDenotation;

            if (m_finiteSynchronousProduct->m_nfa->isInitialState(s))
            {
                m_finiteSynchronousProduct->m_initialStates.insert(productStateNumber);
            }

            if (m_finiteSynchronousProduct->m_nfa->isFinalState(s))
            {
                m_finiteSynchronousProduct->m_finalStates.insert(productStateNumber);
            }
        }
    protected:
        void pop() override
        {
            spot::twa_reachable_iterator_depth_first::pop();
        }

    private:
        FiniteSynchronousProduct* m_finiteSynchronousProduct {};
    };

    Dfs dfs { this };
    dfs.run();
}

int FiniteSynchronousProduct::totalStates() const
{
    return static_cast<int>(m_synchronousProduct->num_states());
}

int FiniteSynchronousProduct::totalFinalStates() const
{
    return m_finalStates.size();
}

int FiniteSynchronousProduct::totalInitialStates() const
{
    return m_initialStates.size();
}

int FiniteSynchronousProduct::totalEdges() const
{
    return static_cast<int>(m_synchronousProduct->num_edges());
}

const std::unordered_set<int>& FiniteSynchronousProduct::initialStates() const
{
    return m_initialStates;
}

bool FiniteSynchronousProduct::isInitialState(const unsigned state) const
{
    return m_synchronousProduct->get_init_state_number() == state;
}

bool FiniteSynchronousProduct::isFinalState(const unsigned state) const
{
    return m_finalStates.count(state) > 0;
}

const std::unordered_set<int>& FiniteSynchronousProduct::finalStates() const
{
    return m_finalStates;
}

bool FiniteSynchronousProduct::hasSuccessors(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    auto edgeStorages { m_synchronousProduct->out(state) };
    return edgeStorages.begin() != edgeStorages.end();
}

FiniteSynchronousProduct::EdgeIterator FiniteSynchronousProduct::successors(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    return m_synchronousProduct->out(state);
}

int FiniteSynchronousProduct::countSuccessors(const unsigned state) const
{
    const EdgeIterator& edgeIterator { m_synchronousProduct->out(state) };
    return std::distance(edgeIterator.begin(), edgeIterator.end());
}

PowersetConstSharedPtr FiniteSynchronousProduct::points(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    return m_denotationByState.at(state);
}

std::pair<int, int> FiniteSynchronousProduct::productStatePair(const unsigned state) const
{
    assert(static_cast<int>(state) < totalStates() && "State is out of range!");

    return m_productStatePair->at(state);
}

void FiniteSynchronousProduct::printDotFormat(std::ostream& os) const
{
    spot::print_dot(os, m_synchronousProduct);
}

spot::const_twa_graph_ptr FiniteSynchronousProduct::twa()
{
    return m_synchronousProduct;
}
