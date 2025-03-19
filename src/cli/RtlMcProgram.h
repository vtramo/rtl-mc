#pragma once

#include <argparse/argparse.hpp>
#include "RtlParsingResult.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "mcparser.h"
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
        : m_rtlMcProgram { "rtl_mc", std::string { version } }
    {
        buildRtlMcProgram();
        parseArgs(argc, argv);

        if (m_gap || m_nogap)
        {
            auto [polyhedralSystem, rtlf] = m_gap ? gap(m_k, m_t) : nogap(m_k, m_t);
            m_polyhedralSystem = polyhedralSystem;
            m_rtlFormula = rtlf;
        }
        else
        {
            readAndParsePolyhedralSystemFile();
            readAndParseRtlFile();
        }

        if (modelChecking())
            parseModelCheckingPoint();
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

private:
    argparse::ArgumentParser m_rtlMcProgram{};

    Semantics m_semantics{};
    std::string m_polyhedralSystemFilename{};
    std::string m_rtlFilename{};
    PolyhedralSystemSharedPtr m_polyhedralSystem{};
    spot::formula m_rtlFormula{};

    AutomatonOptimizationFlags m_automatonOptimizationFlags{};
    bool m_directLtl{};
    bool m_verbose{};
    bool m_universal{};
    bool m_existential{};
    bool m_concurrent{};

    std::optional<std::string> m_modelCheckingPointString{};
    Poly m_modelCheckingPoint{};

    bool m_gap{};
    bool m_nogap{};
    int m_t{};
    int m_k{};

    Verbosity m_verbosityLevel{Verbosity::silent};
    OutputFormat m_outputFormat{OutputFormat::normal};
    std::string m_statsFormat{};

    void buildRtlMcProgram()
    {
        addDescription();
        addInputTypeGroup();
        addUniversalOrExistentialGroup();
        addSemanticsArgument();
        addModelCheckingArgument();
        addDirectLtlArgument();
        addAutomatonOptimizationFlagsGroup();
        addOutputFormatGroup();
        addConcurrentArgument();
    }

    void addDescription()
    {
        m_rtlMcProgram
            .add_description("Model Checking Linear Temporal Properties on Polyhedral Systems")
            .set_usage_max_line_width(80);
    }

    void addInputTypeGroup()
    {
        auto& inputTypeGroup{m_rtlMcProgram.add_mutually_exclusive_group(true)};
        inputTypeGroup.add_argument("-f", "--from-files")
                      .help("Polyhedral System file and RTL formula φ file.")
                      .nargs(2);

        inputTypeGroup.add_argument("--gap")
                      .nargs(2)
                      .help("GAP experiment with k alternating steps and max time t. Example: --gap 3 15.")
                      .scan<'i', int>();

        inputTypeGroup.add_argument("--nogap")
                      .nargs(2)
                      .help("NO GAP experiment with k alternating steps and max time t. Example: --nogap 2 20.")
                      .scan<'i', int>();
    }

    void addUniversalOrExistentialGroup()
    {
        auto& existentialOrUniversalGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        existentialOrUniversalGroup.add_argument("--existential")
                                   .help("Compute the set of points from which there exists a trajectory that satisfies φ (default).")
                                   .flag()
                                   .store_into(m_existential);
        existentialOrUniversalGroup.add_argument("--universal")
                                   .help("Compute the set of points from which every trajectory satisfies φ.")
                                   .flag()
                                   .store_into(m_universal);
    }

    void addSemanticsArgument()
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
                  "> fin:   Considers only finite-time trajectories (default).\n"
                  "         Suitable for properties that are positively verified as soon as a prefix of the trajectory satisfies them,\n"
                  "         such as reachability properties.\n"
                  "> inf:   Considers only infinite-time trajectories.\n"
                  "         Suitable for non-terminating properties, such as liveness or fairness properties.\n"
                  "> may:   Considers all trajectories that are either infinite-time, or end in a may-exit point,\n"
                  "         i.e., a point on the boundary of the invariant from which at least one admissible direction exits.\n"
                  "> must:  Considers all trajectories that are either infinite-time, or end in a must-exit point,\n"
                  "         i.e., a point on the boundary of the invariant from which all admissible directions exit.");

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

    void addDirectLtlArgument()
    {
        m_rtlMcProgram.add_argument("--direct-ltl")
                      .help("Discretise the RTLf formula directly into LTL in a single step, improving performance (experimental).\n"
                            "This option is only effective for finite-time semantics.")
                      .flag()
                      .store_into(m_directLtl);
    }

    void addAutomatonOptimizationFlagsGroup()
    {
        auto& automatonOptimizationGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        automatonOptimizationGroup.add_argument("--low")
                                  .help("Minimal optimizations during automaton construction (fast, default).")
                                  .action([&](const auto&)
                                  {
                                        m_automatonOptimizationFlags.optimizationLevel = AutomatonOptimizationLevel::low;
                                  });
        automatonOptimizationGroup.add_argument("--medium")
                                  .help("Moderate optimizations during automaton construction.")
                                  .action([&](const auto&)
                                  {
                                        m_automatonOptimizationFlags.optimizationLevel = AutomatonOptimizationLevel::medium;
                                  });
        automatonOptimizationGroup.add_argument("--high")
                                  .help("All available optimizations during automaton construction (slow).")
                                  .action([&](const auto&)
                                  {
                                        m_automatonOptimizationFlags.optimizationLevel = AutomatonOptimizationLevel::high;
                                  });
        m_rtlMcProgram.add_argument("--any")
                      .help("Tells the translator that it should attempt to \n"
                          "reduce or produce a deterministic result: any automaton denoting the \n"
                          "given formula is OK. This effectively disables post-processings and \n"
                          "speeds up the translation.")
                      .flag()
                      .store_into(m_automatonOptimizationFlags.any);
    }

    void addOutputFormatGroup()
    {
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
                         }).help("Formats the execution statistics. Example: --stats \"Tot states: %Ats\". "
                             "Placeholders (%Ats, %Ate, etc.) are described in the documentation.");
    }

    void addConcurrentArgument()
    {
        m_rtlMcProgram
            .add_argument("-c", "--concurrent")
            .help("Enable concurrent execution (highly experimental). This option is only effective with the on-the-fly algorithm for finite semantics.")
            .flag()
            .store_into(m_concurrent);
    }

    void parseArgs(const int argc, char* argv[])
    {
        try
        {
            m_rtlMcProgram.parse_args(argc, argv);
            std::vector filenames{m_rtlMcProgram.get<std::vector<std::string>>("--from-files")};
            if (!filenames.empty())
            {
                m_polyhedralSystemFilename = filenames.at(0);
                m_rtlFilename = filenames.at(1);
            }

            std::vector gap{m_rtlMcProgram.get<std::vector<int>>("--gap")};
            if (!gap.empty())
            {
                assert(filenames.empty());
                m_gap = true;
                setGapNoGapParameters(gap[0], gap[1]);
            }

            std::vector nogap{m_rtlMcProgram.get<std::vector<int>>("--nogap")};
            if (!nogap.empty())
            {
                assert(filenames.empty() && gap.empty());
                m_nogap = true;
                setGapNoGapParameters(nogap[0], nogap[1]);
            }

            if (!m_existential && !m_universal) m_existential = true;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_rtlMcProgram;
            exit(1);
        }
    }

    void setGapNoGapParameters(const int k, const int t)
    {
        if (k <= 0)
        {
            std::cerr << "k parameter must be greater than 0!\n";
            std::cerr << m_rtlMcProgram;
            exit(1);
        }

        if (t <= 0)
        {
            std::cerr << "t parameter must be greater than 0!\n";
            std::cerr << m_rtlMcProgram;
            exit(1);
        }

        m_k = k;
        m_t = t;
    }

    void readAndParsePolyhedralSystemFile()
    {
        std::ifstream polyhedralSystemFile{m_polyhedralSystemFilename};
        PolyhedralSystemParsingResult polyhedralSystemParsingResult{parsePolyhedralSystem(polyhedralSystemFile)};
        if (!polyhedralSystemParsingResult.ok())
        {
            std::cerr << "Error while parsing Polyhedral System (file " << m_polyhedralSystemFilename << ").\n";
            std::cerr << polyhedralSystemParsingResult;
            exit(1);
        }
        m_polyhedralSystem = std::make_shared<PolyhedralSystem>(std::move(*polyhedralSystemParsingResult));
        polyhedralSystemFile.close();
    }

    void readAndParseRtlFile()
    {
        std::ifstream rtlfFile{m_rtlFilename};
        std::stringstream buffer{};
        buffer << rtlfFile.rdbuf();
        std::string rtlf{buffer.str()};
        rtlfFile.close();

        RtlParsingResult rtlfParsingResult{parseRtl(rtlf, m_polyhedralSystem->atoms())};
        if (!rtlfParsingResult.ok())
        {
            std::cerr << "Error while parsing RTLf formula (file " << m_rtlFilename << ").\n";
            std::cerr << rtlfParsingResult;
            exit(1);
        }
        m_rtlFormula = std::move(*rtlfParsingResult);
    }

    void parseModelCheckingPoint()
    {
        McPointParsingResult mcPointParsingResult{
            parseMcPoint(
                std::string_view{*m_modelCheckingPointString},
                m_polyhedralSystem->symbolTable()
            )
        };

        if (std::holds_alternative<std::vector<ParserError>>(mcPointParsingResult))
        {
            spdlog::error("Errors while parsing model-checking point:");
            std::vector errors{std::get<std::vector<ParserError>>(mcPointParsingResult)};
            for (auto& error : errors)
                spdlog::error("- {}", error.errorMessage());
            exit(1);
        }

        m_modelCheckingPoint = Poly{
            Parma_Polyhedra_Library::Generator_System{std::get<PPL::Generator>(mcPointParsingResult)}
        };
    }

    static std::tuple<PolyhedralSystemSharedPtr, spot::formula> gap(const int k, const int t)
    {
        assert(k >= 0);
        assert(t >= 0);
        PolyhedralSystemSharedPtr polyhedralSystem{
            std::make_shared<PolyhedralSystem>(
                std::move(
                    *parsePolyhedralSystem(
                        "Inv ( { a >= 0 & b >= 0 } )"
                        "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                        "p { a >= b + 1 }"
                        "q { b >= a + 1 }"
                        "t0 { t = 0 }"
                        "t1 { t <= " + std::to_string(t) + " }"
                    )
                )
            )
        };

        spot::formula rtlf{And({ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q"))})};

        return {std::move(polyhedralSystem), std::move(rtlf)};
    }

    static std::tuple<PolyhedralSystemSharedPtr, spot::formula> nogap(const int k, const int t)
    {
        assert(k >= 0);
        assert(t >= 0);
        PolyhedralSystemSharedPtr polyhedralSystem{
            std::make_shared<PolyhedralSystem>(
                std::move(
                    *parsePolyhedralSystem(
                        "Inv ( { a >= 0 & b >= 0 } )"
                        "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
                        "p { a > b }"
                        "q { b > a }"
                        "t0 { t = 0 }"
                        "t1 { t <= " + std::to_string(t) + " }"
                    )
                )
            )
        };

        spot::formula rtlf{And({ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q"))})};

        return {std::move(polyhedralSystem), std::move(rtlf)};
    }
};
