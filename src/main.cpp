#include <Timer.h>

#include "logger.h"
#include "stats_collectors.h"
#include "interface.h"
#include "omnidirectional_finite_solver.h"
#include "omnidirectional_infinite_solver.h"
#include "Semantics.h"
#include "DiscreteLtlFormula.h"

spot::formula preprocessRtlFormula(
    Semantics semantics,
    const spot::formula& rtlFormula,
    bool universal = false
);

void checkSemantics(PolyhedralSystemConstSharedPtr polyhedralSystem, Semantics semantics);

void showResult(OutputFormat outputFormat, bool modelCheckingProblem = false);

inline int maxPatches(const std::vector<Observable>& observables)
{
    int maxPatches {};

    for (const Observable& observable: observables)
    {
        maxPatches = std::max(maxPatches, observable.totalPatches());
    }

    return maxPatches;
}

int main(const int argc, char *argv[])
{
    Log::configureLogger();
    RtlMcProgram rtlMcProgram { argc, argv };
    // Log::setVerbosityLevel(rtlMcProgram.verbosityLevel());
    //
    // PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    // Semantics semantics { rtlMcProgram.semantics() };
    // PolyhedralAbstractionType polyhedralAbstractionType { rtlMcProgram.polyhedralAbstractionType() };
    // checkSemantics(polyhedralSystem, semantics, polyhedralAbstractionType);
    // Log::log(Verbosity::verbose, "[Polyhedral System]\n{}", *polyhedralSystem);
    // PolyhedralSystemStats polyhedralSystemStats { collectPolyhedralSystemStats(*polyhedralSystem) };
    //
    //
    // spot::formula rtlFormula {
    //     preprocessRtlFormula(
    //         semantics,
    //         polyhedralAbstractionType,
    //         rtlMcProgram.rtlFormula(),
    //         rtlMcProgram.universal()
    //     )
    // };
    // Log::log(Verbosity::verbose, "[RTL Formula] Formula: {}.", rtlFormula);
    // RtlFormulaStats rtlFormulaStats { collectRtlStats(rtlFormula) };
    // Log::log(Verbosity::verbose, "[RTL Formula] Total atomic propositions: {}.", rtlFormulaStats.totalAtomicPropositions);
    // Log::log(Verbosity::verbose, "[RTL Formula] Length: {}.\n", rtlFormulaStats.length);
    //
    //
    // Log::log(Verbosity::verbose, ">>> RTL formula discretisation started.");
    // Timer timer {};
    // DiscreteLtlFormula discreteLtlFormula {
    //     rtlMcProgram.directLtl()
    //         ? DiscreteLtlFormula::discretiseFromFiniteLtl(std::move(rtlFormula))
    //         : DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl()
    // };
    //
    // const double discretisationExecutionTimeSeconds { timer.elapsedInSeconds() };
    // Log::log(Verbosity::verbose, "<<< Discretisation completed. Elapsed time: {} s.", discretisationExecutionTimeSeconds);
    // DiscretisationStats discretisationStats { collectDiscretisationStats(discreteLtlFormula, discretisationExecutionTimeSeconds) };
    // Log::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", discreteLtlFormula);
    // Log::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", discretisationStats.discreteLtlFormulaTotalAtomicPropositions);
    // Log::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", discretisationStats.discreteLtlFormulaLength);
    //
    //
    // PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    // if (polyhedralAbstractionType == PolyhedralAbstractionType::onTheFly)
    // {
    //     if (semantics != Semantics::fin)
    //     {
    //         spdlog::error("An on-the-fly algorithm is only available for finite-time semantics!");
    //         exit(1);
    //     }
    //
    //     // onlyOnTheFlyDenotInterface()
    //     // showResult(rtlMcProgram.outputFormat(), rtlMcProgram.modelChecking(), )
    //     exit(0);
    // }
    //
    //
    //
    // PolyhedralLtlAutomatonConstSharedPtr ltlAutomaton {};
    // if (semantics == Semantics::fin)
    // {
    //     ltlAutomaton = std::make_shared<PolyhedralFiniteLtlAutomaton>(discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap));
    // } else if (semantics == Semantics::inf)
    // {
    //     ltlAutomaton = std::make_shared<PolyhedralBuchiLtlAutomaton>(discreteLtlFormula, std::move(polyhedralSystemFormulaDenotationMap));
    // } else
    // {
    //     throw std::runtime_error("Unsupported semantics for constructing an LTL automaton!");
    // }
    //
    //
    //
    // PolyhedralAbstractionConstSharedPtr abstraction {};
    // if (polyhedralAbstractionType == PolyhedralAbstractionType::general)
    // {
    //     std::vector observables { polyhedralSystem->generateObservables() };
    //     const unsigned sufficientHorizon { 2 * ltlAutomaton->totalStates() * maxPatches(observables) };
    //     abstraction =
    //         std::make_shared<GeneralPolyhedralAbstraction>(
    //             polyhedralSystem,
    //             std::move(observables),
    //             sufficientHorizon
    //         );
    // }
    // else if (polyhedralAbstractionType == PolyhedralAbstractionType::omnidirectional)
    // {
    //     abstraction = std::make_shared<OmnidirectionalPolyhedralAbstraction>(polyhedralSystem);
    // }
    // else
    // {
    //     throw std::runtime_error("Unsupported polyhedral abstraction type!");
    // }
    //
    //
    //
    // auto synchronousProduct { std::make_shared<PolyhedralSynchronousProductAutomaton>(ltlAutomaton, abstraction) };
    //
    //
    //
    // auto result { omnidirectionalInfiniteTimeSemanticsSolver(polyhedralSystem, rtlFormula, rtlMcProgram.automatonOptimizationFlags()) };
    // std::cout << PPLOutput::toString(*result, polyhedralSystem->symbolTable()) << '\n';
}
