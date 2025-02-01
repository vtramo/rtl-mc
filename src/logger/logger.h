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

namespace Log
{
    inline Verbosity g_loggerVerbosity { Verbosity::silent };

    inline void configureLogger(const Verbosity loggerVerbosity)
    {
        g_loggerVerbosity = loggerVerbosity;
        spdlog::set_pattern("%v");
        spdlog::set_level(spdlog::level::info);
    }

    template <typename ... Args>
    void log(const Verbosity verbosity, std::string_view message, Args&& ... args)
    {
        if (g_loggerVerbosity < verbosity) return;
        spdlog::info(message, std::forward<Args>(args)...);
    }
}