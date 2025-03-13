#include "logger.h"
#include "stats_collectors.h"
#include "brink_stay_atoms.h"
#include "interface.h"
#include "Semantics.h"
#include "Solver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalMaySolver.h"
#include "FiniteOnTheFlySolver.h"
#include "BrinkStayFiniteOnTheFlySolver.h"
#include "BrinkFiniteOnTheFlySolver.h"
#include "StayFiniteOnTheFlySolver.h"

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
        else if (polyhedralSystem->isClosedFlow() && SpotUtils::isNonRecurrent(rtlFormula))
        {
            // add stay poi formula & F(stay & last) e risolvi usando on-the-fly finite
            solver = std::make_unique<StayFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        else
        {
            spdlog::error(
                "Model-checking problem for infinite-time semantics: The problem can be solved under the following conditions:\n"
                "- Omnidirectional Flow: the interior of the polyhedral system's flow contains the origin, with no restrictions on the formula.\n"
                "- Non-Recurrent RTL Formula and Closed Flow: the formula is non-recurrent and the flow of the polyhedral system is closed.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
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
        else if (polyhedralSystem->isMovementForced() && polyhedralSystem->isBoundedInvariant())
        {
            // add brink poi formula & F(brink & last) e risolvi usando on-the-fly finite
            solver = std::make_unique<BrinkFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        else if (polyhedralSystem->isClosedFlow() && SpotUtils::isNonRecurrent(rtlFormula))
        {
            // add brink poi formula & F(brink & last) e risolvi usando on-the-fly finite
            // unito
            // add stay poi formula & F(stay & last) e risolvi usando on-the-fly finite
            solver = std::make_unique<BrinkStayFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        else
        {
            spdlog::error(
                "Model-checking problem for may semantics: The problem can be solved under the following conditions:\n"
                "- Omnidirectional Flow: the interior of the polyhedral system's flow contains the origin, with no restrictions on the formula.\n"
                "- Non-Recurrent RTL Formula and Closed Flow: the formula is non-recurrent and the flow of the polyhedral system is closed.\n"
                "- Forced Motion and Bounded Invariant.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
        }
        break;
    case Semantics::must:
        if (polyhedralSystem->isOmnidirectionalFlow())
        {
            solver = std::make_unique<OmnidirectionalInfiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else if (polyhedralSystem->isMovementForced() && polyhedralSystem->isBoundedInvariant())
        {
            // add brink poi formula & F(brink & last) e risolvi usando on-the-fly finite
            solver = std::make_unique<BrinkFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        else if (polyhedralSystem->isClosedFlow() && SpotUtils::isNonRecurrent(rtlFormula))
        {
            // add brink poi formula & F(brink & last) e risolvi usando on-the-fly finite
            // unito
            // add stay poi formula & F(stay & last) e risolvi usando on-the-fly finite
            solver = std::make_unique<BrinkStayFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        else
        {
            spdlog::error(
                "Model-checking problem for must semantics: The problem can be solved under the following conditions:\n"
                "- Omnidirectional Flow: the interior of the polyhedral system's flow contains the origin, with no restrictions on the formula.\n"
                "- Non-Recurrent RTL Formula and Closed Flow: the formula is non-recurrent and the flow of the polyhedral system is closed.\n"
                "- Forced Motion and Bounded Invariant.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
        }
        break;
    }

    showResult(rtlMcProgram, polyhedralSystem, solver->run());
}

void showResult(const RtlMcProgram& rtlMcProgram, PolyhedralSystemConstSharedPtr polyhedralSystem, PowersetSharedPtr result)
{
    std::cerr << "showResult" << std::endl;
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
