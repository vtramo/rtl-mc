#pragma once

#include <argparse/argparse.hpp>
#include "RtlfParsingResult.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "parsertlf.h"
#include "Verbosity.h"

using namespace SpotUtils;

class RtlMcProgram {
public:
    RtlMcProgram(const int argc, char *argv[])
    {
        buildRtlMcProgram();
        parseArgs(argc, argv);

        if (m_gap)
        {
            auto [polyhedralSystem, rtlf] = gap(m_k, m_t);
            m_polyhedralSystem = polyhedralSystem;
            m_rtlFormula = rtlf;
            return;
        }

        if (m_nogap)
        {
            auto [polyhedralSystem, rtlf] = nogap(m_k, m_t);
            m_polyhedralSystem = polyhedralSystem;
            m_rtlFormula = rtlf;
            return;
        }

        readAndParsePolyhedralSystemFile();
        readAndParseRtlFile();
    }

    struct AutomatonOptimizationFlags {
        bool low {};
        bool medium {};
        bool high {};
        bool any {};
    };

    [[nodiscard]] PolyhedralSystemSharedPtr polyhedralSystem() const { return m_polyhedralSystem; }
    [[nodiscard]] const spot::formula& rtlFormula() const { return m_rtlFormula; }
    [[nodiscard]] const AutomatonOptimizationFlags& automatonOptimizationFlags() const { return m_automatonOptimizationFlags; }
    [[nodiscard]] bool universal() const { return m_universal; }
    [[nodiscard]] bool existential() const { return m_existential; }
    [[nodiscard]] bool directLtl() const { return m_directLtl; }
    [[nodiscard]] Verbosity verbosityLevel() const { return m_verbosityLevel; }

private:
    argparse::ArgumentParser m_rtlMcProgram {};

    std::string m_polyhedralSystemFilename {};
    std::string m_rtlFilename {};
    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    spot::formula m_rtlFormula {};

    AutomatonOptimizationFlags m_automatonOptimizationFlags {};
    bool m_directLtl {};
    bool m_verbose {};
    bool m_universal {};
    bool m_existential {};

    bool m_gap {};
    bool m_nogap {};
    int m_t {};
    int m_k {};

    Verbosity m_verbosityLevel {};

    void buildRtlMcProgram()
    {
        m_rtlMcProgram.add_description("Model Checking Linear Temporal Properties on Polyhedral Systems");

        auto& group { m_rtlMcProgram.add_mutually_exclusive_group(true) };
        group.add_argument("-f", "--from-files")
            .help("Polyhedral System file and RTLf file (formula φ)")
            .nargs(2);

        group.add_argument("--gap")
            .nargs(2)
            .help("GAP experiment with k alternating steps and max time t. Example: --gap 3 15.")
            .scan<'i', int>();

        group.add_argument("--nogap")
            .nargs(2)
            .help("NO GAP experiment with k alternating steps and max time t. Example: --nogap 2 20.")
            .scan<'i', int>();

        auto& semantic { m_rtlMcProgram.add_mutually_exclusive_group() };
        semantic.add_argument("--existential")
            .help("compute the set of points from which there exists a trajectory that satisfies φ (default)")
            .flag()
            .store_into(m_existential);
        semantic.add_argument("--universal")
            .help("compute the set of points from which every trajectory satisfies φ")
            .flag()
            .store_into(m_universal);

        m_rtlMcProgram.add_argument("--direct-ltl")
            .help("discretize the RTLf formula directly into LTL in a single step.")
            .flag()
            .store_into(m_directLtl);

        auto& automatonOptimizationGroup { m_rtlMcProgram.add_mutually_exclusive_group() };
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

        m_rtlMcProgram.add_argument("-V", "--verbose")
          .action([&](const auto &)
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
          .help("enable verbose output. Each occurrence of -V increases verbosity level. "
              "Verbose mode provides additional details during program execution to aid debugging "
              "and understanding of the internal processes.")
          .nargs(0);
    }

    void parseArgs(const int argc, char *argv[])
    {
        try
        {
            m_rtlMcProgram.parse_args(argc, argv);
            std::vector<std::string> filenames { m_rtlMcProgram.get<std::vector<std::string>>("--from-files") };
            if (!filenames.empty())
            {
                m_polyhedralSystemFilename = filenames.at(0);
                m_rtlFilename = filenames.at(1);
            }

            std::vector<int> gap { m_rtlMcProgram.get<std::vector<int>>("--gap") };
            if (!gap.empty())
            {
                assert(filenames.empty());
                m_gap = true;
                setGapNoGapParameters(gap[0], gap[1]);
            }

            std::vector<int> nogap { m_rtlMcProgram.get<std::vector<int>>("--nogap") };
            if (!nogap.empty())
            {
                assert(filenames.empty() && gap.empty());
                m_nogap = true;
                setGapNoGapParameters(nogap[0], nogap[1]);
            }

            if (!m_existential && !m_universal) m_existential = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_rtlMcProgram;
            exit(1);
        }
    }

    void setGapNoGapParameters(int k, int t)
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
        std::ifstream polyhedralSystemFile { m_polyhedralSystemFilename };
        PolyhedralSystemParsingResult polyhedralSystemParsingResult { parsePolyhedralSystem(polyhedralSystemFile) };
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
        std::ifstream rtlfFile { m_rtlFilename };
        std::stringstream buffer {};
        buffer << rtlfFile.rdbuf();
        std::string rtlf { buffer.str() };
        rtlfFile.close();

        RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf, m_polyhedralSystem->getAtoms() ) };
        if (!rtlfParsingResult.ok())
        {
            std::cerr << "Error while parsing RTLf formula (file " << m_rtlFilename << ").\n";
            std::cerr << rtlfParsingResult;
            exit(1);
        }
        m_rtlFormula = std::move(*rtlfParsingResult);
    }

    static std::tuple<PolyhedralSystemSharedPtr, spot::formula> gap(int k, int t)
    {
        assert(k >= 0);
        assert(t >= 0);
        PolyhedralSystemSharedPtr polyhedralSystem {
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

        spot::formula rtlf { And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q")) }) };

        return { std::move(polyhedralSystem), std::move(rtlf) };
    }

    static std::tuple<PolyhedralSystemSharedPtr, spot::formula> nogap(int k, int t)
    {
        assert(k >= 0);
        assert(t >= 0);
        PolyhedralSystemSharedPtr polyhedralSystem {
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

        spot::formula rtlf { And({ ap("t0"), G(ap("t1")), generateAlternatingFormula(k, ap("p"), ap("q")) }) };

        return { std::move(polyhedralSystem), std::move(rtlf) };
    }
};