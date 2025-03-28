#include "logger.h"
#include "stats_collectors.h"
#include "brink_stay_atoms.h"
#include "Semantics.h"
#include "RtlMcProgram.h"
#include "Solver.h"
#include "OmnidirectionalInfiniteSolver.h"
#include "OmnidirectionalMaySolver.h"
#include "FiniteOnTheFlySolver.h"
#include "BrinkStayFiniteOnTheFlySolver.h"
#include "BrinkFiniteOnTheFlySolver.h"
#include "StayFiniteOnTheFlySolver.h"

#ifndef RTL_MC_VERSION
#define RTL_MC_VERSION "0.0.0"
#endif

void run(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemConstSharedPtr polyhedralSystem,
    SolverUniquePtr solver,
    bool isUniversalDenotation
);

int main(const int argc, char *argv[])
{
    Log::configureLogger();
    RtlMcProgram rtlMcProgram { argc, argv, RTL_MC_VERSION };
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
        if (polyhedralSystem->hasOmnidirectionalFlow())
        {
            Log::log(Verbosity::verbose, "[Semantics] Finite-time semantics, omnidirectional flow.");

            solver = std::make_unique<OmnidirectionalFiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else
        {
            Log::log(Verbosity::verbose, "[Semantics] Finite-time semantics, on-the-fly algorithm.");

            solver = std::make_unique<FiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent()
            );
        }
        break;
    case Semantics::inf:
        if (polyhedralSystem->hasOmnidirectionalFlow())
        {
            Log::log(Verbosity::verbose, "[Semantics] Infinite-time semantics, omnidirectional flow.");

            solver = std::make_unique<OmnidirectionalInfiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else if (polyhedralSystem->hasCompactFlow() && isNonRecurrent(rtlFormula))
        {
            Log::log(Verbosity::verbose, "[Semantics] Infinite-time semantics, non-recurrent RTL and closed flow.");

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
                "- Non-Recurrent RTL Formula and Compact Flow: the formula is non-recurrent and the flow of the polyhedral system is compact.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
        }
        break;
    case Semantics::may:
        if (polyhedralSystem->hasOmnidirectionalFlow())
        {
            Log::log(Verbosity::verbose, "[Semantics] May semantics, omnidirectional flow.");

            solver = std::make_unique<OmnidirectionalMaySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else if (polyhedralSystem->isMovementForced() && polyhedralSystem->hasBoundedInvariant())
        {
            Log::log(Verbosity::verbose, "[Semantics] May semantics, forced motion and bounded invariant.");

            solver = std::make_unique<BrinkFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent(),
                BrinkSemantics::may
            );
        }
        else if (polyhedralSystem->hasCompactFlow() && isNonRecurrent(rtlFormula))
        {
            Log::log(Verbosity::verbose, "[Semantics] May semantics, non-recurrent RTL and closed flow.");

            solver = std::make_unique<BrinkStayFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent(),
                BrinkSemantics::may
            );
        }
        else
        {
            spdlog::error(
                "Model-checking problem for may semantics: The problem can be solved under the following conditions:\n"
                "- Omnidirectional Flow: the interior of the polyhedral system's flow contains the origin, with no restrictions on the formula.\n"
                "- Non-Recurrent RTL Formula and Compact Flow: the formula is non-recurrent and the flow of the polyhedral system is compact.\n"
                "- Forced Motion and Bounded Invariant.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
        }
        break;
    case Semantics::must:
        if (polyhedralSystem->hasOmnidirectionalFlow())
        {
            Log::log(Verbosity::verbose, "[Semantics] Must semantics, omnidirectional flow.");

            solver = std::make_unique<OmnidirectionalInfiniteSolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation
            );
        }
        else if (polyhedralSystem->isMovementForced() && polyhedralSystem->hasBoundedInvariant())
        {
            Log::log(Verbosity::verbose, "[Semantics] Must semantics, forced motion and bounded invariant.");

            solver = std::make_unique<BrinkFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent(),
                BrinkSemantics::must
            );
        }
        else if (polyhedralSystem->hasCompactFlow() && isNonRecurrent(rtlFormula))
        {
            Log::log(Verbosity::verbose, "[Semantics] Must semantics, non-recurrent RTL and closed flow.");

            solver = std::make_unique<BrinkStayFiniteOnTheFlySolver>(
                polyhedralSystem,
                rtlFormula,
                automatonOptimizationFlags,
                isUniversalDenotation,
                rtlMcProgram.concurrent(),
                BrinkSemantics::must
            );
        }
        else
        {
            spdlog::error(
                "Model-checking problem for must semantics: The problem can be solved under the following conditions:\n"
                "- Omnidirectional Flow: the interior of the polyhedral system's flow contains the origin, with no restrictions on the formula.\n"
                "- Non-Recurrent RTL Formula and Compact Flow: the formula is non-recurrent and the flow of the polyhedral system is compact.\n"
                "- Forced Motion and Bounded Invariant.\n"
                "When these conditions are not satisfied, the problem remains open and unsolved."
            );

            exit(1);
        }
        break;
    }

    run(rtlMcProgram, polyhedralSystem, std::move(solver), isUniversalDenotation);
}

void run(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemConstSharedPtr polyhedralSystem,
    SolverUniquePtr solver,
    const bool isUniversalDenotation
)
{
    PowersetSharedPtr result {
        isUniversalDenotation
            ? PPLUtils::minus(polyhedralSystem->invariant(), *solver->run())
            : solver->run()
    };

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
        const SolverStats& solverStats { solver->stats() };
        std::cout << solverStats.format(rtlMcProgram.statsFormat()) << std::endl;
        break;
    }

    Log::log(Verbosity::verbose, "[Result] Existential Denotation: {}.", rtlMcProgram.existential());
    Log::log(Verbosity::verbose, "[Result] Universal Denotation: {}.", rtlMcProgram.universal());
    Log::log(Verbosity::verbose, "[Result] Model-checking problem: {}.", rtlMcProgram.modelChecking());
}
