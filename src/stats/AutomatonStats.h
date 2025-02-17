#pragma once

#include <string>
#include <spot/twaalgos/sccinfo.hh>
#include "spot_utils.h"
#include <optional>


struct AutomatonStats
{
    struct TranslationFormulaIntoTgbaStats
    {
        std::string optimizationLevel { SpotUtils::toOptimizationLevelString(spot::postprocessor::optimization_level::Low) };
        double executionTimeSeconds {};
        int totalStates {};
        int totalEdges {};
        int totalAcceptingSets {};
    };

    struct TranslationTgbaIntoNfaStats
    {
        double executionTimeSeconds {};
        int totalStates {};
        int totalEdges {};
        int totalFinalStates {};
    };

    struct NfaStats
    {
        double executionTimeSeconds {};
        int totalStates {};
        int totalEdges {};
        int totalFinalStates {};
    };

    struct BackwardNFAConstructionStats
    {
        double executionTimeSeconds {};
        int totalInitialStates {};
        int totalStates {};
        int totalEdges {};
        int totalFinalStates {};
        int maxNumberPatches {};
        int totalNumberPatches {};
        int maxRecursiveDepth {};
        std::optional<spot::scc_info> sccInfo {};
    };

    TranslationFormulaIntoTgbaStats translationFormulaIntoTgba {};
    NfaStats translationTgbaIntoNfaStats {};
    NfaStats nfaOptimizations {};
    BackwardNFAConstructionStats backwardNfaConstructionStats {};
};
