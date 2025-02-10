#include <BackwardNFAPermutator.h>
#include <DenotConcurrentV1.h>
#include <DenotRecursive.h>
#include <future>
#include <random>
#include <bits/random.h>
#include <spot/tl/parse.hh>
#include <spot/twaalgos/postproc.hh>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/ranges.h>

#include "discretization/DiscreteFiniteLtlFormula.h"
#include "utils/ppl/ppl_output.h"
#include "automaton/BackwardNFA.h"
#include "automaton/PolyhedralSystemFormulaDenotationMap.h"
#include "discretization/DiscreteLtlFormula.h"
#include "system/PolyhedralSystem.h"
#include "utils/ppl/ppl_aliases.h"
#include "utils/ppl/ppl_utils.h"
#include "core/Denot.h"
#include "cli/RtlMcProgram.h"
#include "utils/Timer.h"
#include "logger/logger.h"
#include "logger/Verbosity.h"
#include "stats/collectors.h"
#include "stats/StatsFormatter.h"
#include "utils/spot/spot_utils.h"

class DiscreteLtlFormula;
using PPL::IO_Operators::operator<<;
using namespace SpotUtils;

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
);

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    const BackwardNFA& backwardNfa
);

std::unordered_map<int, int> createPermutationMap(
    const std::vector<int>& originalStates,
    const std::vector<int>& permutedStates
);

int main(const int argc, char *argv[])
{
    Log::configureLogger();
    RtlMcProgram rtlMcProgram { argc, argv };
    Log::setVerbosityLevel(rtlMcProgram.verbosityLevel());

    PolyhedralSystemSharedPtr polyhedralSystem { rtlMcProgram.polyhedralSystem() };
    spot::formula rtlFormula {
        rtlMcProgram.universal()
            ? Not(rtlMcProgram.rtlFormula())
            : rtlMcProgram.rtlFormula()
    };

    Log::log(Verbosity::verbose, "[Polyhedral System]\n{}", *polyhedralSystem);
    PolyhedralSystemStats polyhedralSystemStats { collectPolyhedralSystemStats(*polyhedralSystem) };

    Log::log(Verbosity::verbose, "[RTLf Formula] Formula: {}.", rtlFormula);
    RtlFormulaStats rtlfFormulaStats { collectRtlfStats(rtlFormula) };
    Log::log(Verbosity::verbose, "[RTLf Formula] Total atomic propositions: {}.", rtlfFormulaStats.totalAtomicPropositions);
    Log::log(Verbosity::verbose, "[RTLf Formula] Length: {}.\n", rtlfFormulaStats.length);

    Log::log(Verbosity::verbose, ">>> RTLf formula discretization started.");
    Timer timer {};

    DiscreteLtlFormula discreteLtlFormula {
        rtlMcProgram.directLtl()
            ? DiscreteLtlFormula::discretizeToLtl(std::move(rtlFormula))
            : DiscreteFiniteLtlFormula::discretize(std::move(rtlFormula)).toLtl()
    };

    const double discretizationExecutionTimeSeconds { timer.elapsedInSeconds() };
    Log::log(Verbosity::verbose, "<<< Discretization completed. Elapsed time: {} s.", discretizationExecutionTimeSeconds);
    DiscretizationStats discretizationStats { collectDiscretizationStats(discreteLtlFormula, discretizationExecutionTimeSeconds) };
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Formula: {}.", discreteLtlFormula);
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Total atomic propositions: {}.", discretizationStats.discreteLtlFormulaTotalAtomicPropositions);
    Log::log(Verbosity::verbose, "[Discrete LTL formula] Length: {}.\n", discretizationStats.discreteLtlFormulaLength);

    try
    {
        Log::log(Verbosity::verbose, ">>> BackwardNFA automaton construction started.");
        timer.reset();

        PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
        BackwardNFA backwardNfa {
            buildBackwardNfa(
                std::move(discreteLtlFormula),
                std::move(polyhedralSystemFormulaDenotationMap),
                rtlMcProgram.automatonOptimizationFlags()
            )
        };

        std::ofstream file("output.csv");
        file << "permutation,permutationMap,result,denot total iterations\n";
        BackwardNFAPermutator permutator { backwardNfa };
        std::vector originalStates { 3, 242, 2, 11, 10, 0, 233, 236, 237, 232 };
        std::vector permutedStates { 3, 242, 2, 11, 10, 0, 233, 236, 237, 232 };
        constexpr int totalRandomPermutations { 1000 };
        for (int permutation { 1 }; permutation <= totalRandomPermutations; ++permutation)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(permutedStates.begin(), permutedStates.end(), std::default_random_engine(seed));
            std::unordered_map randomPermutation { createPermutationMap(originalStates, permutedStates) };
            file << permutation << '|';
            file << fmt::format("{}|", randomPermutation);

            std::unique_ptr<BackwardNFA> permutedBackwardNfa { permutator.swapStates(randomPermutation) };
            std::unique_ptr denotUniquePtr { createDenot(rtlMcProgram, polyhedralSystem, *permutedBackwardNfa) };
            Denot& denot { *denotUniquePtr };

            PowersetUniquePtr denotResult {
                rtlMcProgram.universal()
                    ? PPLUtils::minus(polyhedralSystem->getInvariant(), *denot())
                    : denot()
            };
            file << PPLOutput::toString(*denotResult, polyhedralSystem->getSymbolTable()) << '|';
            file << denot.totalIterations() << "\n";
        }
        file.close();
    }
    catch (const std::exception& e)
    {
        spdlog::error(e.what());
        exit(1);
    }
}

BackwardNFA buildBackwardNfa(
    DiscreteLtlFormula&& discreteLtlFormula,
    PolyhedralSystemFormulaDenotationMap&& polyhedralSystemFormulaDenotationMap,
    AutomatonOptimizationFlags optimizationFlags
)
{
    spot::postprocessor::optimization_level optimizationLevel {};
    if (optimizationFlags.low) optimizationLevel = spot::postprocessor::optimization_level::Low;
    if (optimizationFlags.medium) optimizationLevel = spot::postprocessor::optimization_level::Medium;
    if (optimizationFlags.high) optimizationLevel = spot::postprocessor::optimization_level::High;
    return {
        std::move(discreteLtlFormula),
        std::move(polyhedralSystemFormulaDenotationMap),
        optimizationLevel,
        optimizationFlags.any
    };
}

std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    const BackwardNFA& backwardNfa
) {
    if (rtlMcProgram.concurrent()) {
        return std::make_unique<DenotConcurrentV1>(polyhedralSystem, backwardNfa);
    }

    return std::make_unique<DenotRecursive>(polyhedralSystem, backwardNfa);
}

std::unordered_map<int, int> createPermutationMap(const std::vector<int>& originalStates, const std::vector<int>& permutedStates)
{
    std::unordered_map<int, int> permutation {};
    int totalStates { static_cast<int>(originalStates.size()) };
    permutation.reserve(totalStates);
    for (int i { 0 }; i < totalStates; ++i) {
        const int state { originalStates[i] };
        const int replacingState { permutedStates[i] };
        permutation[state] = replacingState;
    }
    return permutation;
}