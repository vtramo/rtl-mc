#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Verbosity.h"
#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "BackwardNFA.h"

// Specializations of fmt::formatter to enable {fmt} library formatting for custom types using operator<<.
template <> struct fmt::formatter<spot::formula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<PolyhedralSystem> : fmt::ostream_formatter {};
template <> struct fmt::formatter<DiscreteLtlFormula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<BackwardNFA> : fmt::ostream_formatter {};

/*!
 *  \brief Namespace for logging utilities, providing verbosity control and logging functions.
 */
namespace Log
{
    inline Verbosity g_loggerVerbosity { Verbosity::silent }; //!< Global verbosity level for logging.

    /*!
     *  \brief Sets the global verbosity level for logging.
     *  \param loggerVerbosity The desired verbosity level.
     */
    inline void setVerbosityLevel(const Verbosity loggerVerbosity)
    {
        g_loggerVerbosity = loggerVerbosity;
    }

    /*!
     *  \brief Configures the logger with a simple pattern and default log level.
     */
    inline void configureLogger()
    {
        spdlog::set_pattern("%v");
        spdlog::set_level(spdlog::level::info);
    }

    /*!
     *  \brief Logs a message if the global verbosity level is sufficient.
     *  \tparam Args Types of the format arguments.
     *  \param verbosity The verbosity level of the message.
     *  \param message The message to log, optionally with format placeholders.
     *  \param args Arguments to format into the message.
     */
    template <typename ... Args>
    void log(const Verbosity verbosity, std::string_view message, Args&& ... args)
    {
        if (g_loggerVerbosity < verbosity) return;
        spdlog::info(message, std::forward<Args>(args)...);
    }
}