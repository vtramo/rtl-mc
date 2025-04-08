#pragma once

#include <string>
#include <argparse/argparse.hpp>

class SysGenProgram
{
public:
    SysGenProgram(const int argc, char* argv[], const std::string_view version = "0.0.0")
        : m_sysGenProgram { "sys-gen", std::string { version } }
        {
            buildRtlGenProgram();
            parseArgs(argc, argv);
        }

    [[nodiscard]] unsigned totalTanks() const { return m_totalTanks; }
    [[nodiscard]] bool includeClock() const { return m_includeClock; }
    [[nodiscard]] unsigned gapThickness() const { return m_gapThickness; }
private:
    static const inline std::string s_gapSubcommandName { "gap" };

    argparse::ArgumentParser m_sysGenProgram{};
    argparse::ArgumentParser m_gapSubcommand{ s_gapSubcommandName };
    unsigned m_totalTanks { 2 };
    unsigned m_gapThickness { 1 };
    bool m_includeClock {};

    void buildRtlGenProgram()
    {
        addDescription();
        configureSubcommands();
    }

    void addDescription()
    {
        m_sysGenProgram
            .add_description("A tool for generating polyhedral systems")
            .set_usage_max_line_width(80);
    }

    void configureSubcommands()
    {
        m_gapSubcommand.add_description("GAP Experiment with n tanks");

        m_gapSubcommand.add_argument("-t", "--tanks")
            .help("The total number of tanks")
            .nargs(1)
            .default_value(2u)
            .scan<'u', unsigned>();

        m_gapSubcommand.add_argument("-c", "--clock")
            .help("Include a clock t")
            .default_value(false)
            .flag()
            .store_into(m_includeClock);

        m_gapSubcommand.add_argument("-g", "--thickness")
            .help("Gap thickness")
            .nargs(1)
            .default_value(1u)
            .scan<'u', unsigned>();

        m_sysGenProgram.add_subparser(m_gapSubcommand);
    }

    void parseArgs(const int argc, char* argv[])
    {
        try
        {
            m_sysGenProgram.parse_args(argc, argv);

            if (m_sysGenProgram.is_subcommand_used(s_gapSubcommandName))
            {
                m_totalTanks = m_gapSubcommand.get<unsigned>("-t");
                if (m_totalTanks < 2)
                {
                    throw std::invalid_argument("Total tanks must be at least 2!");
                }

                m_gapThickness = m_gapSubcommand.get<unsigned>("-g");
                if (m_gapThickness < 1)
                {
                    throw std::invalid_argument("Gap thickness must be at least 1!");
                }
            }
            else
            {
                std::cerr << m_sysGenProgram;
                exit(1);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_sysGenProgram;
            exit(1);
        }
    }
};
