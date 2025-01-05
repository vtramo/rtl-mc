#include "LabelDenotationMap.h"
#include "PolyhedralSystemParsingResult.h"
#include "BackwardNFA.h"
#include "systemparser.h"
#include <spot/tl/print.hh>

int main()
{
    const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
        parsePolyhedralSystem(
            "Inv ( { a >= 0 & b >= 0 } )\n"
            "i ( { a >= 0 & b >= 0 } )\n"
            "Flow { X + Z <= 4 }\n"
            "p { a >= b + 1 }\n"
            "q { b >= a + 1 }\n"
            "t0 { t = 0 }\n"
            "t1 { t <= 10 }\n"
        )
    };

    assert(polyhedralSystemParsingResult.ok());
    PolyhedralSystem polyhedralSystem { std::move(*polyhedralSystemParsingResult) };

    LabelDenotationMap atomSetDenotationMap { polyhedralSystem };
    BackwardNFA backwardNfa { spot::parse_infix_psl("G i & t0 & G t1 & (F p & F(q & F(p & Fq)))").f, atomSetDenotationMap };
    std::cout << "Total states: " << backwardNfa.totalStates() << '\n';

    for (const State& state: backwardNfa.states())
    {
        std::cout << std::boolalpha;
        std::cout << "State: " << state.index() << '\n';
        std::cout << "Initial: " << state.isInitial() << '\n';
        std::cout << "Sing: " << state.isSing() << '\n';
        std::cout << "Final: " << state.isFinal() << '\n';
        std::cout << "Labels: " << state.labels() << '\n';
        std::cout << "Predecessors: " << backwardNfa.predecessors(state.index()) << "\n\n";
    }

    std::cout << atomSetDenotationMap << '\n';
}
