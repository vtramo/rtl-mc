#pragma once

#include <argparse/argparse.hpp>
#include "RtlfParsingResult.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "mcparser.h"
#include "parsertlf.h"
#include "Verbosity.h"
#include "AutomatonOptimizationFlags.h"
#include "OutputFormat.h"
#include "Semantics.h"
#include "PolyhedralAbstractionType.h"

using namespace SpotUtils;

class RtlMcProgram
{
public:
    RtlMcProgram(const int argc, char* argv[])
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
    [[nodiscard]] PolyhedralAbstractionType polyhedralAbstractionType() const { return m_polyhedralAbstractionType; }

private:
    argparse::ArgumentParser m_rtlMcProgram{};

    Semantics m_semantics{};
    PolyhedralAbstractionType m_polyhedralAbstractionType{};
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
        addSemanticsGroup();
        addPolyhedralTypeGroup();
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
                      .help("Polyhedral System file and RTLf file (formula φ)")
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
                                   .help("compute the set of points from which there exists a trajectory that satisfies φ (default)")
                                   .flag()
                                   .store_into(m_existential);
        existentialOrUniversalGroup.add_argument("--universal")
                                   .help("compute the set of points from which every trajectory satisfies φ")
                                   .flag()
                                   .store_into(m_universal);
    }

    void addSemanticsGroup()
    {
        auto& semantics{m_rtlMcProgram.add_mutually_exclusive_group()};
        semantics.add_argument("--fin")
                 .help("this semantics considers only finite-time trajectories (default). It is "
                     "suitable to properties that are positively verified as soon as a prefix of a "
                     "trajectory satisfies them, such as reachability properties.")
                 .action([&](const auto&)
                 {
                     m_semantics = Semantics::fin;
                 });
        semantics.add_argument("--inf")
                 .help("this semantics considers only infinite-time trajectories. "
                     "It is suitable to non-terminating properties, such as liveness or fairness "
                     "properties.")
                 .action([&](const auto&)
                 {
                     m_semantics = Semantics::inf;
                 });
        semantics.add_argument("--may")
                 .help("this semantics considers all trajectories that "
                     "are either infinite-time, or end in a may-exit point, i.e., a point that is "
                     "on the boundary of the invariant and from which at least one admissible "
                     "direction exits from the invariant.")
                 .action([&](const auto&)
                 {
                     m_semantics = Semantics::may;
                 });
    }

    void addPolyhedralTypeGroup()
    {
        auto& polyhedralTypeGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        polyhedralTypeGroup.add_argument("--on-the-fly")
            .help("use an on-the-fly symbolic algorithm that avoids the explicit product construction between "
                "the automaton of the formula and the polyhedral abstraction, improving space "
                "efficiency and practical performance (default). Only available for finite-time semantics.")
             .action([&](const auto&)
             {
                 m_polyhedralAbstractionType = PolyhedralAbstractionType::onTheFly;
             });
        polyhedralTypeGroup.add_argument("--general")
            .help("construct a general polyhedral abstraction. Note that the resulting graph can be extremely "
                  "large and may become impractical to construct, even for relatively small inputs. This approach "
                  "is computationally expensive and may not be feasible.")
                 .action([&](const auto&)
                 {
                     m_polyhedralAbstractionType = PolyhedralAbstractionType::general;
                 });
        polyhedralTypeGroup.add_argument("--omnidirectional")
             .help("construct a much more succinct polyhedral abstraction based on geometric adjacency. "
                 "The interior of the flow of the polyhedral system must contains the origin.")
             .action([&](const auto&)
             {
                 m_polyhedralAbstractionType = PolyhedralAbstractionType::omnidirectional;
             });

    }

    void addModelCheckingArgument()
    {
        m_rtlMcProgram.add_argument("--mc")
                      .help(
                          "check if a given point x ∈ ℝⁿ is the source of a trajectory in the polyhedral system that satisfies the temporal formula φ. "
                          "For --existential, checks if some trajectory from the point satisfies φ. "
                          "For --universal, checks if all trajectories from the point satisfy φ. "
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
                      .help("discretise the RTLf formula directly into LTL in a single step.")
                      .flag()
                      .store_into(m_directLtl);
    }

    void addAutomatonOptimizationFlagsGroup()
    {
        auto& automatonOptimizationGroup{m_rtlMcProgram.add_mutually_exclusive_group()};
        automatonOptimizationGroup.add_argument("--low")
                                  .help("minimal optimizations during automaton construction (fast, default)")
                                  .store_into(m_automatonOptimizationFlags.low);
        automatonOptimizationGroup.add_argument("--medium")
                                  .help("moderate optimizations during automaton construction")
                                  .store_into(m_automatonOptimizationFlags.medium);
        automatonOptimizationGroup.add_argument("--high")
                                  .help("all available optimizations during automaton construction (slow)")
                                  .store_into(m_automatonOptimizationFlags.high);
        m_rtlMcProgram.add_argument("--any")
                      .help("tells the translator that it should attempt to "
                          "reduce or produce a deterministic result: any automaton denoting the "
                          "given formula is OK.  This effectively disables post-processings and "
                          "speeds up the translation")
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
                         .help("enable verbose output. Each occurrence of -V increases verbosity level")
                         .nargs(0);
        outputFormatGroup.add_argument("-q", "--quiet")
                         .flag()
                         .help("suppress all normal output")
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
                         }).help("formats the execution statistics. Example: --stats \"Tot states: %Ats\". "
                             "Placeholders (%Ats, %Ate, etc.) are described in the documentation.");
    }

    void addConcurrentArgument()
    {
        m_rtlMcProgram
            .add_argument("-c", "--concurrent")
            .help("concurrent execution")
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

        RtlfParsingResult rtlfParsingResult{parseRtlf(rtlf, m_polyhedralSystem->atoms())};
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
