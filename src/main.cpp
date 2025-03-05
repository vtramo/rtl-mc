#include "logger.h"
#include "interface.h"
#include "omnidirectional_finite_solver.h"

int main(const int argc, char *argv[])
{
    // return onlyOnTheFlyDenotInterface(argc, argv);
    RtlMcProgram rtlMcProgram { argc, argv };
    Log::setVerbosityLevel(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    auto result { omnidirectionalFiniteTimeSemanticsSolver(polyhedralSystem, rtlFormula, rtlMcProgram.automatonOptimizationFlags()) };
    std::cout << PPLOutput::toString(*result, polyhedralSystem->symbolTable()) << '\n';
}
