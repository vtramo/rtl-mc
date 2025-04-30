#pragma once

enum TranslationOptimizationLevel
{
    low,
    medium,
    high
};

struct TranslationOptimizationFlags {
    TranslationOptimizationLevel optimizationLevel {};
    bool any {};
};