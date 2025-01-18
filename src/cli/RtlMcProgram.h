#pragma once

#include <argparse/argparse.hpp>
#include "RtlfParsingResult.h"
#include "PolyhedralSystemParsingResult.h"
#include "systemparser.h"
#include "parsertlf.h"

class RtlMcProgram {
public:
    RtlMcProgram(const std::string_view programName, const int argc, char *argv[])
        : m_programName { programName }
        , m_rtlMcProgram { argparse::ArgumentParser { m_programName } }
    {
        buildRtlMcProgram();
        parseArgs(argc, argv);
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

private:
    std::string m_programName {};
    AutomatonOptimizationFlags m_automatonOptimizationFlags {};
    std::string m_polyhedralSystemFilename {};
    std::string m_rtlFilename {};
    argparse::ArgumentParser m_rtlMcProgram {};

    PolyhedralSystemSharedPtr m_polyhedralSystem {};
    spot::formula m_rtlFormula {};

    void buildRtlMcProgram()
    {
        m_rtlMcProgram.add_argument("system")
            .help("Polyhedral System file")
            .store_into(m_polyhedralSystemFilename);

        m_rtlMcProgram.add_argument("formula")
            .help("RTLf file")
            .store_into(m_rtlFilename);

        auto& automatonOptimizationGroup { m_rtlMcProgram.add_mutually_exclusive_group() };
        automatonOptimizationGroup.add_argument("--low")
            .help("minimal optimizations (fast)")
            .store_into(m_automatonOptimizationFlags.low);
        automatonOptimizationGroup.add_argument("--medium")
            .help("moderate optimizations")
            .store_into(m_automatonOptimizationFlags.medium);
        automatonOptimizationGroup.add_argument("--high")
            .help("all available optimizations (slow, default)")
            .store_into(m_automatonOptimizationFlags.high);
        m_rtlMcProgram.add_argument("--any")
            .help("tells the translator that it should attempt to "
                  "reduce or produce a deterministic result: any automaton denoting the "
                  "given formula is OK.  This effectively disables post-processings and "
                  "speeds up the translation")
            .flag()
            .store_into(m_automatonOptimizationFlags.any);
    }

    void parseArgs(const int argc, char *argv[])
    {
        try
        {
            m_rtlMcProgram.parse_args(argc, argv);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_rtlMcProgram;
            exit(1);
        }
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
};