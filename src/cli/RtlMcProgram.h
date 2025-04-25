#pragma once

#include <argparse/argparse.hpp>
#include "RtlParsingResult.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "pointparser.h"
#include "parsertl.h"
#include "Verbosity.h"
#include "AutomatonOptimization.h"
#include "OutputFormat.h"
#include "Semantics.h"

using namespace SpotUtils;

class RtlMcProgram
{
public:
    RtlMcProgram(const int argc, char* argv[], const std::string_view version = "0.0.0")
        : m_rtlMcProgram{"rtl-mc", std::string{version}}
    {
        buildRtlMcProgram();
        parseArgs(argc, argv);
        readAndParsePolyhedralSystem();
        readAndParseRtlFormula();

        if (modelChecking())
        {
            parseModelCheckingPoint();
        }
    }

    [[nodiscard]] PolyhedralSystemSharedPtr polyhedralSystem() const { return m_polyhedralSystem; }
    [[nodiscard]] const spot::formula& rtlFormula() const { return m_rtlFormula; }

    [[nodiscard]] const AutomatonOptimizationFlags& automatonOptimizationFlags() const
    {
        return m_automatonOptimizationFlags;
    }

    [[nodiscard]] bool universal() const { return m_universal; }
    [[nodiscard]] bool existential() const { return m_existential; }
    [[nodiscard]] bool modelChecking() const { return m_modelCheckingPointString.has_value(); }
    [[nodiscard]] Poly modelCheckingPoint() const { return m_modelCheckingPoint; }
    [[nodiscard]] bool directLtl() const { return m_directLtl; }
    [[nodiscard]] bool concurrent() const { return m_concurrent; }
    [[nodiscard]] Verbosity verbosityLevel() const { return m_verbosityLevel; }
    [[nodiscard]] OutputFormat outputFormat() const { return m_outputFormat; }
    [[nodiscard]] std::string statsFormat() const { return m_statsFormat; }
    [[nodiscard]] Semantics semantics() const { return m_semantics; }
    [[nodiscard]] bool isExportAutomatonDotEnabled() const { return m_exportAutomatonDot; }
    [[nodiscard]] bool isExportDenotPathTreeDotEnabled() const { return m_exportDenotPathTreeDot; }

private:
    argparse::ArgumentParser m_rtlMcProgram{};

    std::istream* m_polyhedralSystemInputStream{&std::cin};
    std::istream* m_rtlFormulaInputStream{&std::cin};

    PolyhedralSystemSharedPtr m_polyhedralSystem{};
    spot::formula m_rtlFormula{};

    Semantics m_semantics{};
    bool m_universal{};
    bool m_existential{};
    std::optional<std::string> m_modelCheckingPointString{};
    Poly m_modelCheckingPoint{};

    AutomatonOptimizationFlags m_automatonOptimizationFlags{};
    bool m_directLtl{};
    bool m_concurrent{};

    Verbosity m_verbosityLevel{Verbosity::silent};
    OutputFormat m_outputFormat{OutputFormat::normal};
    std::string m_statsFormat{};
    bool m_exportAutomatonDot{};
    bool m_exportDenotPathTreeDot{};

    void buildRtlMcProgram()
    {
        addDescription();
        addInputArguments();
        addSemanticsArguments();
        addAutomatonOptimizationFlagsArguments();
        addOutputFormatArguments();
        addAdvancedArguments();
    }

    void addDescription()
    {
        m_rtlMcProgram
            .add_description("Model Checking Linear Temporal Properties on Polyhedral Systems")
            .set_usage_max_line_width(80);
    }

    void addInputArguments()
    {
        m_rtlMcProgram.add_group("Input options");

        auto& inputPolyhedralSystemGroup{m_rtlMcProgram.add_mutually_exclusive_group()};

        inputPolyhedralSystemGroup.add_argument("--system-file", "-sf")
                                  .help("Provide the polyhedral system from a file.")
                                  .metavar("<FILE>")
                                  .action([&](const std::string& filename)
                                      {
                                          m_polyhedralSystemInputStream = {new std::ifstream{filename}};
                                      }
                                  ).nargs(1);

        inputPolyhedralSystemGroup.add_argument("--system-string", "-ss")
                                  .help("Provide the polyhedral system directly as a string.")
                                  .metavar("<STRING>")
                                  .action([&](const std::string& systemString)
                                      {
                                          m_polyhedralSystemInputStream = {new std::istringstream{systemString}};
                                      }
                                  ).nargs(1);

        inputPolyhedralSystemGroup.add_argument("--system-stdin", "-si")
                                  .help("Provide the polyhedral system via stdin (default).")
                                  .flag()
                                  .default_value(true);


        auto& inputRtlFormula{m_rtlMcProgram.add_mutually_exclusive_group()};
        inputRtlFormula.add_argument("--formula-file", "-ff")
                       .help("Provide the RTL formula from a file.")
                       .metavar("<FILE>")
                       .action([&](const std::string& filename)
                           {
                               m_rtlFormulaInputStream = {new std::ifstream{filename}};
                           }
                       ).nargs(1);

        inputRtlFormula.add_argument("--formula-string", "-fs")
                       .help("Provide the RTL formula directly as a string.")
                       .metavar("<STRING>")
                       .action([&](const std::string& rtlString)
                           {
                               m_rtlFormulaInputStream = {new std::istringstream{rtlString}};
                           }
                       ).nargs(1);

        inputRtlFormula.add_argument("--formula-stdin", "-fi")
                       .help("Provide the RTL formula via stdin (default).")
                       .flag()
                       .default_value(true);
    }

    void addSemanticsArguments()
    {
        m_rtlMcProgram.add_group("Semantics options");
        addSemanticsTypeArgument();
        addUniversalOrExistentialArguments();
        addModelCheckingArgument();
    }

    void addSemanticsTypeArgument()
    {
        m_rtlMcProgram.add_argument("--semantics")
                      .action([&](const std::string& semanticsString)
                      {
                          std::optional<Semantics> semantics = toSemantics(semanticsString);
                          if (!semantics)
                          {
                              spdlog::error("Invalid semantics. Supported semantics are: {fin, inf, may, must}.");
                              exit(1);
                          }
                          m_semantics = *semantics;
                      })
                      .nargs(1)
                      .help("E.g. --semantics=fin. Possible semantics:\n"
                          "     > fin:   Considers only finite-time trajectories (default).\n"
                          "              Suitable for properties that are positively verified\n"
                          "              as soon as a prefix of the trajectory satisfies them,\n"
                          "              such as reachability properties.\n"
                          "     > inf:   Considers only infinite-time trajectories.\n"
                          "              Suitable for non-terminating properties, such as\n"
                          "              safety or fairness properties.\n"
                          "     > may:   Considers all trajectories that are either infinite-time,\n"
                          "              or end in a may-exit point, i.e., a point on the boundary\n"
                          "              of the invariant from which at least one admissible\n"
                          "              direction exits.\n"
                          "     > must:  Considers all trajectories that are either infinite-time,\n"
                          "              or end in a must-exit point, i.e., a point on the boundary\n"
                          "              of the invariant from which all admissible directions exit.");
    }

    void addUniversalOrExistentialArguments()
    {
        auto& existentialOrUniversalGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        existentialOrUniversalGroup.add_argument("--existential")
                                   .help(
                                       "Compute the set of points from which there exists a trajectory that satisfies φ (default).")
                                   .flag()
                                   .store_into(m_existential);
        existentialOrUniversalGroup.add_argument("--universal")
                                   .help("Compute the set of points from which every trajectory satisfies φ.")
                                   .flag()
                                   .store_into(m_universal);
    }

    void addModelCheckingArgument()
    {
        m_rtlMcProgram.add_argument("--mc")
                      .help(
                          "Check if a given point x ∈ ℚⁿ is the source of a trajectory in the polyhedral system \n"
                          "that satisfies the temporal formula φ. \n"
                          "For --existential, checks if some trajectory from the point satisfies φ. \n"
                          "For --universal, checks if all trajectories from the point satisfy φ. \n"
                          "Specify all system variables with rational values (e.g., [x=1/3, y=-2/3, z=1]).")
                      .action([&](const std::string& mcPoint)
                      {
                          m_modelCheckingPointString.emplace(mcPoint);
                          assert(m_modelCheckingPointString.has_value());
                      });
    }

    void addAutomatonOptimizationFlagsArguments()
    {
        m_rtlMcProgram.add_group("Automaton formula translation optimizations (Spot library flags)");
        auto& automatonOptimizationGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        automatonOptimizationGroup.add_argument("--low")
                                  .help("Minimal optimizations during automaton construction (fast, default).")
                                  .flag()
                                  .action([&](const auto&)
                                  {
                                      m_automatonOptimizationFlags.optimizationLevel = AutomatonOptimizationLevel::low;
                                  });
        automatonOptimizationGroup.add_argument("--medium")
                                  .help("Moderate optimizations during automaton construction.")
                                  .flag()
                                  .action([&](const auto&)
                                  {
                                      m_automatonOptimizationFlags.optimizationLevel =
                                          AutomatonOptimizationLevel::medium;
                                  });
        automatonOptimizationGroup.add_argument("--high")
                                  .help("All available optimizations during automaton construction (slow).")
                                  .flag()
                                  .action([&](const auto&)
                                  {
                                      m_automatonOptimizationFlags.optimizationLevel = AutomatonOptimizationLevel::high;
                                  });
        m_rtlMcProgram.add_argument("--any")
                      .help("No preference.")
                      .flag()
                      .store_into(m_automatonOptimizationFlags.any);
    }

    void addOutputFormatArguments()
    {
        m_rtlMcProgram.add_group("Output format");
        auto& outputFormatGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        outputFormatGroup.add_argument("-V", "--verbose")
                         .action([&](const auto&)
                         {
                             m_verbosityLevel =
                                 static_cast<Verbosity>(
                                     std::min(
                                         static_cast<int>(m_verbosityLevel) + 1,
                                         static_cast<int>(Verbosity::totVerbosityLevels) - 1
                                     ));
                         })
                         .append()
                         .default_value(false)
                         .implicit_value(true)
                         .help("Enable verbose output. Each occurrence of -V increases verbosity level (e.g. -VVV).")
                         .nargs(0);
        outputFormatGroup.add_argument("-q", "--quiet")
                         .flag()
                         .help("Suppress all normal output.")
                         .action([&](const auto&)
                         {
                             m_verbosityLevel = Verbosity::silent;
                             m_outputFormat = OutputFormat::quiet;
                         });
        outputFormatGroup.add_argument("-s", "--stats")
                         .action([&](const auto& formatStats)
                         {
                             m_statsFormat = formatStats;
                             m_outputFormat = OutputFormat::stats;
                         }).help("Formats the execution statistics.");
    }

    void addAdvancedArguments()
    {
        m_rtlMcProgram.add_group("Advanced options");
        addExportDotArguments();
        addDirectLtlArgument();
        addConcurrentArgument();
    }

    void addDirectLtlArgument()
    {
        m_rtlMcProgram.add_argument("--direct-ltl")
                      .help(
                          "Discretise the RTLf formula directly into LTL in a single step, improving performance (experimental).\n"
                          "This option is only effective for finite-time semantics.")
                      .flag()
                      .store_into(m_directLtl);
    }

    void addConcurrentArgument()
    {
        m_rtlMcProgram
            .add_argument("-c", "--concurrent")
            .help("Enable concurrent execution (highly experimental). This option is only effective with\n"
                "the on-the-fly algorithm for finite semantics.")
            .flag()
            .store_into(m_concurrent);
    }

    void addExportDotArguments()
    {
        m_rtlMcProgram
            .add_argument("--export-automaton-dot")
            .help("Create a .dot file for each graph/automaton created (including any intermediate changes)\n"
                "during the solving process.")
            .flag()
            .store_into(m_exportAutomatonDot);

        m_rtlMcProgram
            .add_argument("--export-denot-path-tree-dot")
            .help("Create dot files representing path trees generated during the on-the-fly algorithm execution.")
            .flag()
            .store_into(m_exportDenotPathTreeDot);
    }

    void parseArgs(const int argc, char* argv[])
    {
        try
        {
            m_rtlMcProgram.parse_args(argc, argv);

            if (m_polyhedralSystemInputStream == &std::cin && m_rtlFormulaInputStream == &std::cin)
            {
                throw std::runtime_error(
                    "It is not possible to read both the polyhedral system and the RTL formula from stdin!");
            }

            if (!m_existential && !m_universal)
            {
                m_existential = true;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_rtlMcProgram;
            exit(1);
        }
    }

    void readAndParsePolyhedralSystem()
    {
        PolyhedralSystemParsingResult polyhedralSystemParsingResult{
            parsePolyhedralSystem(*m_polyhedralSystemInputStream)
        };
        if (!polyhedralSystemParsingResult.ok())
        {
            std::cerr << "Error while parsing Polyhedral System.\n";
            std::cerr << polyhedralSystemParsingResult;
            exit(1);
        }
        m_polyhedralSystem = std::make_shared<PolyhedralSystem>(std::move(*polyhedralSystemParsingResult));

        if (m_polyhedralSystemInputStream != &std::cin)
        {
            delete m_polyhedralSystemInputStream;
        }
    }

    void readAndParseRtlFormula()
    {
        RtlParsingResult rtlParsingResult{parseRtl(*m_rtlFormulaInputStream, m_polyhedralSystem->atoms())};
        if (!rtlParsingResult.ok())
        {
            std::cerr << "Error while parsing RTL formula.\n";
            std::cerr << rtlParsingResult;
            exit(1);
        }
        m_rtlFormula = std::move(*rtlParsingResult);

        if (m_rtlFormulaInputStream != &std::cin)
        {
            delete m_rtlFormulaInputStream;
        }
    }

    void parseModelCheckingPoint()
    {
        RationalPointParsingResult rationalPointParsingResult{
            parseRationalPoint(
                std::string_view{*m_modelCheckingPointString},
                m_polyhedralSystem->symbolTable()
            )
        };

        if (std::holds_alternative<std::vector<ParserError>>(rationalPointParsingResult))
        {
            spdlog::error("Errors while parsing model-checking point:");
            std::vector errors{std::get<std::vector<ParserError>>(rationalPointParsingResult)};
            for (auto& error : errors)
                spdlog::error("- {}", error.errorMessage());
            exit(1);
        }

        RationalPoint rationalPoint { std::get<RationalPoint>(rationalPointParsingResult) };
        m_modelCheckingPoint = Poly { PPL::Generator_System { rationalPoint.generator() } };
    }
};
