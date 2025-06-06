#pragma once

#include <spot/twaalgos/reachiter.hh>
#include "BackwardNFA.h"

class BackwardNFA;

class BackwardNFADepthFirstSearch: protected spot::twa_reachable_iterator_depth_first
{
public:
    explicit BackwardNFADepthFirstSearch(BackwardNFAConstSharedPtr backwardNfa);

    void operator() ();
    void run() final;
    virtual bool wantState(unsigned state) const;
    virtual void processState(unsigned state);
    virtual void processEdge(unsigned src, unsigned dst);

protected:
    BackwardNFAConstSharedPtr m_backwardNfa {};

private:
    bool want_state(const spot::state* state) const final;
    void process_state(const spot::state* s, int n, spot::twa_succ_iterator* si) final;
    void process_link(
        const spot::state* in_s,
        int in,
        const spot::state* out_s,
        int out,
        const spot::twa_succ_iterator* si
    ) final;
    void push(const spot::state* s, int sn) final;
    void pop() final;
};