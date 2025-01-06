
#include <spot/tl/parse.hh>
#include <spot/tl/print.hh>
#include <spot/twaalgos/hoa.hh>
#include "BackwardNFA.h"
#include "LabelDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "PolyhedralSystem.h"
#include "discretization.h"

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

    LabelDenotationMap labelDenotationMap { polyhedralSystem };
    DiscreteLtlFormula discreteLtlFormula { discretize(spot::parse_infix_psl("G i & t0 & G t1 & (F p & F(q & F(p & Fq)))").f) };
    BackwardNFA backwardNfa { std::move(discreteLtlFormula), labelDenotationMap };
    std::cout << backwardNfa << '\n';
    std::cout << labelDenotationMap << '\n';
}
