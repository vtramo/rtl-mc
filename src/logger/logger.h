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
    inline bool g_exportAutomatonDot {}; //!< Create a dot file for each graph/automaton created during the solving process.
    inline bool g_includeStateDenotationInAutomatonDot { true }; //!< Include state denotation in the final \ref PolyhedralLtlAutomaton
    inline bool g_exportDenotPathTreeDot{}; //!< Create dot files representing path trees generated during the on-the-fly algorithm execution.

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
     *  \param exportAutomatonDot Enable or disable this feature.
     */
    inline void setExportAutomatonDot(const bool exportAutomatonDot)
    {
        g_exportAutomatonDot = exportAutomatonDot;
    }

    /*!
     *  \brief Enables the creation of dot files representing path trees during the on-the-fly algorithm execution.
     *
     *  This function allows you to enable or disable the creation of `.dot` files that represent
     *  path trees generated during the on-the-fly algorithm execution.
     *
     *  \param exportDenotPathTree If true, enables the creation of path tree dot files.
     */
    inline void setExportDenotPathTreeDot(const bool exportDenotPathTree)
    {
        g_exportDenotPathTreeDot = exportDenotPathTree;
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
     *  \brief Saves the given graph to a `.dot` file if the export feature (\ref setExportAutomatonDot) is enabled.
     *
     *  This function exports the provided graph to a `.dot` file if the \ref setExportAutomatonDot is set to `true`.
     *  The `.dot` file is saved with a unique filename generated based on a sequence number, the provided filename,
     *  and the current timestamp. Each export will generate a new `.dot` file with an incremented sequence number.
     *
     *  \param graph A pointer to the graph to be exported.
     *  \param filename The base name of the file to which the graph will be exported. The final `.dot` file
     *         will have a name formed by appending the sequence number and timestamp to this base filename.
     *  \param id A prefix that will be added to the generated filename.
     *  \param options an optional string of letters to be passed to spot::print_dot. Each letter indicates
     *         a different option. Here are some examples: 'v' for vertical output, 'h' for horizontal
     *         output, 't' for transition-based acceptance, 'N' to hide automaton name, 'n' to show name,
     *         'c' for circle-shaped states, 'a' to show acceptance, 'k' for state-based labels.
     *         See spot::print_dot documentation for the complete list of supported options.
     *
     *  \example
     *  Log::log(graph, "my_graph");
     *
     *  This will save the graph to a file named something like:
     *  "1-my_graph-31-03-2025T14:35:00.dot"
     */
    inline void log(spot::const_twa_graph_ptr graph, const std::string_view filename, const unsigned id = 1, const char* options = nullptr)
    {
        std::ofstream dotOutputStream {
            std::to_string(id) + "-" + std::string { filename } + "-" + nowToString() + ".dot"
        };
        spot::print_dot(dotOutputStream, graph, options);
        dotOutputStream.close();
    }

    inline void logAutomaton(spot::const_twa_graph_ptr graph, const std::string_view filename, const char* options = nullptr)
    {
        if (!g_exportAutomatonDot)
        {
            return;
        }

        static std::atomic_int sequence { 1 };

        log(graph, filename, sequence++, options);
    }

    inline void logDenotPathTree(spot::const_twa_graph_ptr graph, const std::string_view filename, const char* options = nullptr)
    {
        if (!g_exportDenotPathTreeDot)
        {
            return;
        }

        static std::atomic_int sequence { 1 };

        log(graph, filename, sequence++, options);
    }
}