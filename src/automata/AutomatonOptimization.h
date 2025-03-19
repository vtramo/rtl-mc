#pragma once

enum AutomatonOptimizationLevel
{
    low,
    medium,
    high
};

struct AutomatonOptimizationFlags {
    AutomatonOptimizationLevel optimizationLevel {};
    bool any {};
};