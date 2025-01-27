#pragma once

#include <string>
#include "spot_utils.h"

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
    };

    TranslationFormulaIntoTgbaStats translationFormulaIntoTgba {};
    NfaStats translationTgbaIntoNfaStats {};
    NfaStats nfaOptimizations {};
    BackwardNFAConstructionStats backwardNfaConstructionStats {};
};
