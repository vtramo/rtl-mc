#include "parsertl.h"
#include "spot_utils.h"
#include "formula.h"
#include <spot/tl/parse.hh>

static RtlParsingResult toRtlfParsingResult(spot::parse_error_list&& spotErrors);

using namespace SpotUtils;

/*!
 * The RTL syntax follows Spot's standard LTL syntax for spot::formula.
 *
 * Example of valid RTL formulas:
 * - \f$ \texttt{G}(p \rightarrow \texttt{F}\,q) \f$
 * - \f$ \texttt{F}(a \, \& \, \texttt{X} b) \f$
 * - \f$ p \, \texttt{U} \, q \f$
 */
RtlParsingResult parseRtl(const std::string_view rtl)
{
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(std::string { rtl }) };
    spot::parse_error_list spotErrors { parsedFormula.errors };

    if (!spotErrors.empty())
    {
        return toRtlfParsingResult(std::move(spotErrors));
    }

    spot::formula rtlfFormula { std::move(parsedFormula.f) };
    if (!rtlfFormula.is_ltl_formula())
    {
        return RtlParsingResult::notLtlFormula(std::move(rtlfFormula));
    }

    return RtlParsingResult::ok(std::move(rtlfFormula));
}

/*!
 * This function parses the RTL formula and checks that all atomic propositions
 * used in the formula are present in the allowed set. If the formula contains
 * any atomic proposition not in \p allowedAtomicPropositions, the result will
 * be marked as invalid (\ref RtlParsingResult::ok() == \c false) and the illegal atoms can be retrieved
 * via \ref RtlParsingResult::illegalAtoms().
 *
 * \note Even when illegal atoms are found, the formula is still parsed and
 *       can be accessed via operator*, but ok() will return false.
 *
 * \see parseRtl(const std::string_view)
 */
RtlParsingResult parseRtl(const std::string_view rtl, const spot::atomic_prop_set& allowedAtomicPropositions)
{
    RtlParsingResult rtlfParsingResult { parseRtl(rtl) };
    if (!rtlfParsingResult)
    {
        return rtlfParsingResult;
    }

    spot::formula& rtlfFormula { *rtlfParsingResult };
    std::vector illegalAtoms { collectAtomsNotIn(allowedAtomicPropositions, rtlfFormula) };

    if (!illegalAtoms.empty())
    {
        rtlfParsingResult.setIllegalAtoms(std::move(illegalAtoms));
    }

    return rtlfParsingResult;
}

/*!
 * \brief Converts Spot's parse errors to our \ref RtlParsingResult.
 * \param spotErrors List of parse errors from Spot
 * \return RtlParsingResult containing the converted errors
 *
 * This helper function transforms Spot's native parse error format
 * into \ref RtlParsingResult.
 */
static RtlParsingResult toRtlfParsingResult(spot::parse_error_list&& spotErrors)
{
    std::vector<ParserError> parserErrors {};
    parserErrors.reserve(spotErrors.size());

    std::transform(spotErrors.begin(), spotErrors.end(),
        std::back_inserter(parserErrors),
        [](spot::one_parse_error& spotError)
        {
            return ParserError { std::move(spotError) };
        });

    return RtlParsingResult::withParserErrors(std::move(parserErrors));
}