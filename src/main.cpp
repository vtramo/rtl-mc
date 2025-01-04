// #include "parsertlf.h"
// #include <spot_utils.h>
#include <BackwardNFA.h>
#include <spot/tl/print.hh>
#include "spot-examples/automata.h"

int main()
{
    BackwardNFA backward_nfa { spot::parse_infix_psl("F(p0)").f };
    std::cout << backward_nfa.totalStates() << '\n';
    // automata();
}
