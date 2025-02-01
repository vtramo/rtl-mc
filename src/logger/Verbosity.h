#pragma once

enum class Verbosity: int {
    silent = 0,
    verbose = 1,
    veryVerbose = 2,
    debug = 3,
    trace = 4,

    totVerbosityLevels
};
