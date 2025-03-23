#pragma once

/*!
 *  \brief Enumeration representing different levels of logging verbosity.
 *
 * This enum defines the verbosity levels for logging, ranging from `silent` (no logging) to `trace` (most detailed logging).
 * The levels are ordered, with higher values indicating more detailed logging.
 */
enum class Verbosity: int {
    silent = 0,       //!< No logging output.
    verbose = 1,      //!< Basic logging output.
    veryVerbose = 2,  //!< More detailed logging output.
    debug = 3,        //!< Debug-level logging output.
    trace = 4,        //!< Most detailed logging output (e.g., for tracing execution).

    totVerbosityLevels //!< Sentinel value representing the total number of verbosity levels.
};