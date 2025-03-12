#include "logger.h"
#include "stats_collectors.h"
#include "interface.h"
#include "Semantics.h"
#include "Solver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalMaySolver.h"
#include "GeneralInfiniteSolver.h"
#include "FiniteOnTheFlySolver.h"

void showResult(const RtlMcProgram& rtlMcProgram, PolyhedralSystemConstSharedPtr polyhedralSystem, PowersetSharedPtr result);

int main(const int argc, char *argv[])
{
    Log::configureLogger();
    RtlMcProgram rtlMcProgram { argc, argv };
    Log::setVerbosityLevel(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula { rtlMcProgram.rtlFormula() };
    AutomatonOptimizationFlags automatonOptimizationFlags { rtlMcProgram.automatonOptimizationFlags() };
    Semantics semantics { rtlMcProgram.semantics() };
    bool isUniversalDenotation { rtlMcProgram.universal() };

    SolverUniquePtr solver {};
    switch (semantics)
    {
    case Semantics::fin:
        solver = std::make_unique<FiniteOnTheFlySolver>(
            polyhedralSystem,
            rtlFormula,
            automatonOptimizationFlags,
            isUniversalDenotation,
            rtlMcProgram.concurrent()
        );
        break;
    case Semantics::inf:
        if (polyhedralSystem->isOmnidirectionalFlow())
        {
            solver = std::make_unique<OmnidirectionalInfiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else
        {
            solver = std::make_unique<GeneralInfiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        break;
    case Semantics::may:
        if (polyhedralSystem->isOmnidirectionalFlow())
        {
            solver = std::make_unique<OmnidirectionalMaySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else
        {
            throw std::runtime_error("Not implemented yet");
        }
        break;
        break;
    case Semantics::must:
        throw std::runtime_error("Not implemented yet");
        break;
    }

    showResult(rtlMcProgram, polyhedralSystem, solver->run());
}

void showResult(const RtlMcProgram& rtlMcProgram, PolyhedralSystemConstSharedPtr polyhedralSystem, PowersetSharedPtr result)
{
    switch (rtlMcProgram.outputFormat())
    {
    case OutputFormat::normal:
        if (rtlMcProgram.modelChecking())
        {
            Poly point { rtlMcProgram.modelCheckingPoint() };
            std::cout << std::boolalpha << result->contains(Powerset { point }) << std::noboolalpha << '\n';
        }
        else
        {
            std::cout << PPLOutput::toString(*result, polyhedralSystem->symbolTable()) << '\n';
        }
        break;
    case OutputFormat::quiet:
        break;
    case OutputFormat::stats:
        throw std::runtime_error("Not implemented yet");
        break;
    }

    Log::log(Verbosity::verbose, "[Result] Existential Denotation: {}.", rtlMcProgram.existential());
    Log::log(Verbosity::verbose, "[Result] Universal Denotation: {}.", rtlMcProgram.universal());
    Log::log(Verbosity::verbose, "[Result] Model-checking problem: {}.", rtlMcProgram.modelChecking());
}
