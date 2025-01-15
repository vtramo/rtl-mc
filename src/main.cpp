#include <spot/tl/parse.hh>
#include <spot/tl/print.hh>
#include <spot/twaalgos/hoa.hh>
#include "RtlfParsingResult.h"
#include "BackwardNFA.h"
#include "PolyhedralSystemLabelDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "parsertlf.h"
#include "PolyhedralSystem.h"
#include "discretization.h"
#include "ppl_aliases.h"
#include <argparse/argparse.hpp>

void reachZero();

int main(const int argc, char *argv[])
{
    argparse::ArgumentParser rtlMcProgram { "rtl-mc" };

    std::string polyhedralSystemFilename {};
    rtlMcProgram.add_argument("system")
        .help("Polyhedral System file")
        .store_into(polyhedralSystemFilename);

    std::string rtlfFilename {};
    rtlMcProgram.add_argument("formula")
        .help("RTLf file")
        .store_into(rtlfFilename);

    try
    {
        rtlMcProgram.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << rtlMcProgram;
        exit(1);
    }

    std::ifstream polyhedralSystemFile { polyhedralSystemFilename };
    PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem(polyhedralSystemFile) };
    if (!polyhedralSystemParsingResult.ok())
    {
        std::cerr << "Failed to parse polyhedral system.\n";
        exit(1);
    }
    PolyhedralSystemSharedPtr polyhedralSystem { std::make_shared<PolyhedralSystem>(std::move(*polyhedralSystemParsingResult)) };
    polyhedralSystemFile.close();
    polyhedralSystem->setConstraintOutputMinimized(false);
    PolyhedralSystemLabelDenotationMap polyhedralSystemLabelDenotationMap { polyhedralSystem };
    std::cout << *polyhedralSystem << '\n';

    std::ifstream rtlfFile { rtlfFilename };
    std::stringstream buffer {};
    buffer << rtlfFile.rdbuf();
    std::string rtlf { buffer.str() };
    rtlfFile.close();

    RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf, polyhedralSystem->getAtoms() ) };
    if (!rtlfParsingResult.ok())
    {
        std::cerr << "Failed to parse rtlf file.\n";
        exit(1);
    }
        //
        // const PolyhedralSystemParsingResult polyhedralSystemParsingResult {
        //     parsePolyhedralSystem(
        //         "Inv ( { a >= 0 & b >= 0 } )\n"
        //         "i ( { a >= 0 & b >= 0 } )\n"
        //         "Flow { X + Z <= 4 }\n"
        //         "p { a >= b + 1 }\n"
        //         "q { b >= a + 1 }\n"
        //         "t0 { t = 0 }\n"
        //         "t1 { t <= 10 }\n"
        //     )
        // };
    DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretizeToLtl(std::move(*rtlfParsingResult)) };

    // DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretizeToLtl(spot::parse_infix_psl("p1 & q1 & X(p1) & X(q1) & (v1 U (r1 R z1)) & G(x1) & F(u1 & F(p2 & (F p3 | u2 W p4))) & (t | G(X(w)))").f) };
    // DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretizeToLtl(spot::parse_infix_psl("G(i) & t0 & G(t1) & F(p & F(q))").f) };
    try
    {
        BackwardNFA backwardNfa { std::move(discreteLtlFormula), std::move(polyhedralSystemLabelDenotationMap) };
        std::cout << backwardNfa << '\n';
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // backwardNfa.printDotFormat(std::cout);
    // reachZero();
}

void reachZero()
{
    Poly preFlow {};

    Powerset powersetA {};

    Poly polyB {};
    polyB.topological_closure_assign();
    Powerset powersetPolyB { polyB };

    Poly positivePreFlowB { polyB };
    positivePreFlowB.positive_time_elapse_assign(preFlow);
    Powerset powersetPositivePreFlowB { positivePreFlowB };

    powersetA.intersection_assign(powersetPolyB);
    powersetA.intersection_assign(powersetPositivePreFlowB);
}