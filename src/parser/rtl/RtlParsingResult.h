/*!
*  \file RtlParsingResult.h
 * \brief Represents the result of parsing an RTL formula.
 *
 * This class encapsulates the result of parsing an RTL formula, including the parsed formula (if successful),
 * any parsing errors encountered, and validation information about the formula's atomic propositions.
 */

#pragma once

#include <vector>
#include <spot/tl/apcollect.hh>

#include "ParserError.h"

/*!
 * \class RtlParsingResult
 * \brief Represents the result of parsing an RTL formula with validation information.
 *
 * The RtlParsingResult class stores the outcome of parsing an RTL formula, including:
 * - The parsed formula (if successful)
 * - Any parser errors encountered
 * - Whether the formula is a valid LTL formula
 * - Any illegal atomic propositions used
 *
 * The result can be checked for success using ok(), and the formula can be accessed via operator*.
 * Detailed error information is available through the various accessor methods.
 */
class RtlParsingResult
{
public:
    /*!
     * \brief Checks if parsing was completely successful.
     * \return \c true if:
     *         - No parser errors occurred
     *         - The formula is valid LTL
     *         - No illegal atoms were used
     */
    [[nodiscard]] bool ok() const { return !hasParserErrors() && isLtl() && !isUsingIllegalAtoms(); }

    /*!
     * \brief Checks if the parsed formula is valid LTL.
     * \return \c true if the formula is LTL, false otherwise
     */
    [[nodiscard]] bool isLtl() const { return m_isLtl; }

    /*!
     * \brief Checks if the formula uses atomic propositions not allowed in the system.
     * \return \c true if illegal atoms were used, false otherwise
     */
    [[nodiscard]] bool isUsingIllegalAtoms() const { return !m_illegalAtoms.empty(); }

    /*!
     * \brief Checks if any parser errors occurred.
     * \return \c true if parser errors exist, false otherwise
     */
    [[nodiscard]] bool hasParserErrors() const { return !m_parserErrors.empty(); }

    /*!
     * \brief Gets the list of parser errors.
     * \return Vector of ParserError objects
     */
    [[nodiscard]] std::vector<ParserError> parserErrors() const { return m_parserErrors; }

    /*!
     * \brief Gets the list of illegal atomic propositions used.
     * \return Vector of spot::formula objects representing illegal atoms
     */
    [[nodiscard]] std::vector<spot::formula> illegalAtoms() const { return m_illegalAtoms; }

    /*!
     * \brief Dereference operator to access the parsed formula.
     * \return Reference to the parsed spot::formula
     * \throws std::runtime_error if parsing failed and no formula is available
     */
    [[nodiscard]] spot::formula& operator* ()
    {
        if (!m_rtlf)
            throw std::runtime_error("Parsing failed: RtlfFormula is null due to errors...");

        return *m_rtlf;
    }

    /*!
     * \brief Boolean negation operator to check for failure.
     * \return \c true if parsing failed, \c false otherwise
     */
    [[nodiscard]] bool operator!() const { return !ok(); }

    /*!
     * \brief Creates a successful parsing result.
     * \param rtlf The successfully parsed formula
     * \return RtlParsingResult indicating success
     */
    [[nodiscard]] static RtlParsingResult ok(spot::formula&& rtlf)
    {
        return RtlParsingResult{std::move(rtlf), {}, {}, true};
    }

    /*!
     * \brief Creates a failed parsing result with parser errors.
     * \param parserErrors Vector of parser errors encountered
     * \return RtlParsingResult containing the errors
     */
    [[nodiscard]] static RtlParsingResult withParserErrors(std::vector<ParserError>&& parserErrors)
    {
        return RtlParsingResult{std::move(parserErrors)};
    }

    /*!
     * \brief Creates a failed parsing result indicating the formula is not LTL.
     * \param rtlf The parsed formula (not LTL)
     * \return RtlParsingResult indicating non-LTL formula
     */
    [[nodiscard]] static RtlParsingResult notLtlFormula(spot::formula&& rtlf)
    {
        return RtlParsingResult{std::move(rtlf), false};
    }

private:
    std::optional<spot::formula> m_rtlf {};
    const std::vector<ParserError> m_parserErrors {};
    std::vector<spot::formula> m_illegalAtoms {};
    const bool m_isLtl { false };

    friend RtlParsingResult parseRtl(std::string_view rtl, const spot::atomic_prop_set& allowedAtomicPropositions);

    RtlParsingResult(
        std::optional<spot::formula>&& rtlf,
        std::vector<ParserError>&& parserErrors,
        std::vector<spot::formula>&& illegalAtoms,
        const bool isLtl = false
    )
        : m_rtlf{std::move(rtlf)}
          , m_parserErrors{std::move(parserErrors)}
          , m_illegalAtoms{std::move(illegalAtoms)}
          , m_isLtl{isLtl}
    {
    }

    explicit RtlParsingResult(std::vector<ParserError>&& parserErrors)
        : RtlParsingResult({}, std::move(parserErrors), {}, false)
    {
    }

    explicit RtlParsingResult(
        spot::formula&& rtlf,
        const bool isLtl = false
    ) : RtlParsingResult(std::move(rtlf), {}, {}, isLtl)
    {
    }

    explicit RtlParsingResult(
        std::vector<spot::formula>&& forbiddenAtoms,
        const bool isLtl = true
    ) : RtlParsingResult({}, {}, std::move(forbiddenAtoms), isLtl)
    {
    }

    explicit RtlParsingResult(spot::formula&& rtlf) : RtlParsingResult(std::move(rtlf), {}, {}, false)
    {
    }

    void setIllegalAtoms(std::vector<spot::formula>&& illegalAtoms)
    {
        m_illegalAtoms = std::move(illegalAtoms);
    }
};

/*!
 * \brief Output stream operator for RtlParsingResult.
 * \param os The output stream
 * \param result The result to output
 * \return Reference to the output stream
 *
 * Formats the result for output, showing:
 * - The formula if successful
 * - Non-LTL warning if applicable
 * - Illegal atoms if present
 * - Parser errors if any occurred
 */
inline std::ostream& operator<< (std::ostream& os, RtlParsingResult& result)
{
    if (result.ok())
        return os << *result;

    if (!result.isLtl())
        os << "Not a RTLf formula!\n";

    if (result.isUsingIllegalAtoms())
    {
        os << "The RTLf formula is using atomic propositions not present in the Polyhedral System: \n";
        os << "  Illegal atomic propositions: [";
        bool first { true };
        for (const auto& atom: result.illegalAtoms())
        {
            os << (first ? "" : ", ") << atom.ap_name();
            first = false;
        }
        os << "]\n";
    }

    if (result.hasParserErrors())
        os << result.parserErrors();

    return os;
}
