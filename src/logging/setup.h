#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "Verbosity.h"
#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "BackwardNFA.h"

template <> struct fmt::formatter<spot::formula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<PolyhedralSystem> : fmt::ostream_formatter {};
template <> struct fmt::formatter<DiscreteLtlFormula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<BackwardNFA> : fmt::ostream_formatter {};

inline void configureLogger(const Verbosity loggerVerbosity)
{
    spdlog::set_pattern("%v");
    switch (loggerVerbosity)
    {
    case Verbosity::silent:
        spdlog::set_level(spdlog::level::off);
        break;
    case Verbosity::verbose:
        spdlog::set_level(spdlog::level::info);
        break;
    case Verbosity::veryVerbose:
        spdlog::set_level(spdlog::level::debug);
        break;
    case Verbosity::debug:
        spdlog::set_level(spdlog::level::trace);
        break;
    default:
        spdlog::set_level(spdlog::level::off);
    }
}