#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spot/twaalgos/dot.hh>

#include "Verbosity.h"
#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "BackwardNFA.h"
#include "DenotPathNode.h"

class PolyhedralSynchronousProductAutomaton;

// Formatter specializations.
template <> struct fmt::formatter<spot::formula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<PolyhedralSystem> : fmt::ostream_formatter {};
template <> struct fmt::formatter<DiscreteLtlFormula> : fmt::ostream_formatter {};
template <> struct fmt::formatter<BackwardNFA> : fmt::ostream_formatter {};
template <> struct fmt::formatter<Observable> : fmt::ostream_formatter {};
template <> struct fmt::formatter<PolyhedralLtlAutomaton> : fmt::ostream_formatter {};
template <> struct fmt::formatter<spot::twa_run> : fmt::ostream_formatter {};
template <> struct fmt::formatter<PolyhedralSynchronousProductAutomaton> : fmt::ostream_formatter {};
template <> struct fmt::formatter<std::vector<DenotPathNode>> : fmt::ostream_formatter {};

/*!
 *  \brief Namespace for logging utilities, providing verbosity control and logging functions.
 */
namespace Log
{
    inline Verbosity g_loggerVerbosity { Verbosity::silent }; //!< Global verbosity level for logging.
    inline bool g_exportDot {}; //!< Create a dot file for each graph/automaton created during the solving process.

    /*!
     *  \brief Sets the global verbosity level for logging.
     *  \param loggerVerbosity The desired verbosity level.
     */
    inline void setVerbosityLevel(const Verbosity loggerVerbosity)
    {
        g_loggerVerbosity = loggerVerbosity;
    }


    /*!
     *  \brief Enables the creation of a dot file for each graph/automaton created during the solving process.
     *
     *  This function allows you to enable or disable the creation of `.dot` files that represent
     *  graphs and/or automata generated during the solving process.
     *
     *  \param exportDot Enable or disable this feature.
     */
    inline void setExportDot(const bool exportDot)
    {
        g_exportDot = exportDot;
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
        if (g_loggerVerbosity < verbosity)
        {
            return;
        }

        spdlog::info(message, std::forward<Args>(args)...);
    }

    /*!
     *  \brief Saves the given graph to a `.dot` file if the export feature (\ref setExportDot) is enabled.
     *
     *  This function exports the provided graph to a `.dot` file if the \ref setExportDot is set to `true`.
     *  The `.dot` file is saved with a unique filename generated based on a sequence number, the provided filename,
     *  and the current timestamp. Each export will generate a new `.dot` file with an incremented sequence number.
     *
     *  \param graph A pointer to the graph to be exported.
     *  \param filename The base name of the file to which the graph will be exported. The final `.dot` file
     *         will have a name formed by appending the sequence number and timestamp to this base filename.
     *
     *  \note This function requires the \ref setExportDot flag to be set to `true` to enable the export. If
     *        \ref setExportDot is `false`, no file is created.
     *
     *  \example
     *  Log::log(graph, "my_graph");
     *
     *  This will save the graph to a file named something like:
     *  "1-my_graph-31-03-2025T14:35:00.dot"
     */
    inline void log(spot::const_twa_graph_ptr graph, const std::string_view filename)
    {
        if (!g_exportDot)
        {
            return;
        }

        static std::atomic_int sequence { 1 };

        auto t { std::time(nullptr) };
        auto tm { *std::localtime(&t) };
        std::ostringstream tmStringStream {};
        tmStringStream << std::put_time(&tm, "%d-%m-%YT%H:%M:%S");
        std::ofstream dotOutputStream {
            std::to_string(sequence++) + "-" + std::string { filename } + "-" + tmStringStream.str() + ".dot"
        };
        spot::print_dot(dotOutputStream, graph);
        dotOutputStream.close();
    }
}