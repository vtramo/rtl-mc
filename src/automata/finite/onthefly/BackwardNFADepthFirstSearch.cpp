#include "BackwardNFADepthFirstSearch.h"

BackwardNFADepthFirstSearch::BackwardNFADepthFirstSearch(BackwardNFAConstSharedPtr backwardNfa)
    : twa_reachable_iterator_depth_first(backwardNfa->twa())
{
    m_backwardNfa = backwardNfa;
}

void BackwardNFADepthFirstSearch::operator() ()
{
	run();
}

void BackwardNFADepthFirstSearch::run()
{
    spot::twa_reachable_iterator_depth_first::run();
}

bool BackwardNFADepthFirstSearch::wantState([[maybe_unused]] unsigned state) const {
    return true;
}

void BackwardNFADepthFirstSearch::processState([[maybe_unused]] const unsigned state) {
}

void BackwardNFADepthFirstSearch::processEdge([[maybe_unused]] const unsigned src, [[maybe_unused]] const unsigned dst)
{
}

bool BackwardNFADepthFirstSearch::want_state(const spot::state* state) const
{
    unsigned stateId { m_backwardNfa->m_automaton->state_number(state) };
    if (m_backwardNfa->m_dummyInitialState == stateId)
        return true;
    return wantState(stateId);
}

void BackwardNFADepthFirstSearch::process_state(const spot::state* s, [[maybe_unused]] int n, [[maybe_unused]] spot::twa_succ_iterator* si)
{
    unsigned stateId { m_backwardNfa->m_automaton->state_number(s) };
    if (m_backwardNfa->m_dummyInitialState == stateId)
        return;
    processState(stateId);
}

void BackwardNFADepthFirstSearch::process_link(
    const spot::state* in_s,
    [[maybe_unused]] int in,
    const spot::state* out_s,
    [[maybe_unused]] int out,
    [[maybe_unused]] const spot::twa_succ_iterator* si
)
{
    unsigned srcStateId { m_backwardNfa->m_automaton->state_number(in_s) };
    unsigned dstStateId { m_backwardNfa->m_automaton->state_number(out_s) };
    if (srcStateId == m_backwardNfa->m_dummyInitialState) return;
    processEdge(srcStateId, dstStateId);
}

void BackwardNFADepthFirstSearch::push(const spot::state* s, const int sn)
{
    spot::twa_reachable_iterator_depth_first::push(s, sn);
}

void BackwardNFADepthFirstSearch::pop()
{
    spot::twa_reachable_iterator_depth_first::pop();
}