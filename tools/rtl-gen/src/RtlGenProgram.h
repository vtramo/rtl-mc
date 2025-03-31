#pragma once

#include <formula.h>
#include <argparse/argparse.hpp>
#include <string>

#include "AlternationFormulaFamilyParams.h"

/*!
 *  \brief CLI interface for the rtl-gen formula generator program.
 *
 *  The `RtlGenProgram` class manages the command-line interface (CLI) for the `rtl-gen` program,
 *  a tool for generating RTL formulae. It organizes formulas into subcommands,
 *  where each subcommand corresponds to a formula family with specific options.
 *
 *  The program supports the `alternation` subcommand, which generates formulae that require a trajectory
 *  alternating between two propositions `p` and `q` a specified number of times (`k`).
 */
class RtlGenProgram
{
public:
    /*!
     *  \brief Type alias for parameters specific to the selected formula family
     */
    using FormulaFamilyParams = std::variant<AlternationFormulaFamilyParams>;

    /*!
    *  \brief Constructs the `rtl-gen` program, sets up arguments, and parses command-line input.
     *
     *  \param argc The argument count from `main` function.
     *  \param argv The argument vector from `main` function.
     *  \param version The version of the program (default is `0.0.0`).
     */
    RtlGenProgram(const int argc, char* argv[], const std::string_view version = "0.0.0")
        : m_rtlGenProgram { "rtl-gen", std::string { version } }
    {
        buildRtlGenProgram();
        parseArgs(argc, argv);
    }

    /*!
     *  \brief Retrieves the parameters for the selected formula family.
     *  \return The formula family parameters.
     */
    [[maybe_unused]] FormulaFamilyParams familyParameters() const { return m_formulaFamilyParams; }

private:
    static const inline std::string s_alternationSubcommandName { "alternation" }; //!< The name of the alternation subcommand.

    argparse::ArgumentParser m_rtlGenProgram{}; //!< Main argument parser for the program.
    argparse::ArgumentParser m_alternationFormulaFamilySubcommand{ s_alternationSubcommandName }; //!< Argument parser for the alternation formula family.
    FormulaFamilyParams m_formulaFamilyParams{}; //!< Stores the formula family parameters.

    /*!
     *  \brief Initializes the program by adding descriptions and configuring subcommands.
     */
    void buildRtlGenProgram()
    {
        addDescription();
        configureSubcommands();
    }

    /*!
     *  \brief Adds a description to the main program.
     */
    void addDescription()
    {
        m_rtlGenProgram
            .add_description("A tool for generating RTL formulae")
            .set_usage_max_line_width(80);
    }

    /*!
     *  \brief Configures all subcommands for the program.
     */
    void configureSubcommands()
    {
        configureAlternationFormulaFamilySubcommand();
    }

    /*!
     *  \brief Configures the alternation formula family subcommand.
     */
    void configureAlternationFormulaFamilySubcommand()
    {
        m_alternationFormulaFamilySubcommand.add_description(
            "Family of formulae that requires a trajectory that alternates k times between the propositions p and q"
        );
        m_alternationFormulaFamilySubcommand.add_argument("-k", "--alternations")
            .help("The number of alternations the trajectory must make")
            .default_value(1u)
            .scan<'u', unsigned>();

        m_alternationFormulaFamilySubcommand.add_argument("-p")
            .help("Specifies the name of the first proposition")
            .default_value("p");

        m_alternationFormulaFamilySubcommand.add_argument("-q")
            .help("Specifies the name of the second proposition")
            .default_value("q");

        m_rtlGenProgram.add_subparser(m_alternationFormulaFamilySubcommand);
    }

    /*!
     *  \brief Parses the command-line arguments and populates the formula family parameters.
     *  \param argc The argument count.
     *  \param argv The argument vector.
     *  \throws std::exception if argument parsing fails.
     */
    void parseArgs(const int argc, char* argv[])
    {
        try
        {
            m_rtlGenProgram.parse_args(argc, argv);

            if (m_rtlGenProgram.is_subcommand_used(s_alternationSubcommandName))
            {
                m_formulaFamilyParams = AlternationFormulaFamilyParams {
                    m_alternationFormulaFamilySubcommand.get<std::string>("-p"),
                    m_alternationFormulaFamilySubcommand.get<std::string>("-q"),
                    m_alternationFormulaFamilySubcommand.get<unsigned>("--alternations"),
                };
            }
            else
            {
                std::cerr << m_rtlGenProgram;
                exit(1);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_rtlGenProgram;
            exit(1);
        }
    }
};