#include "parsertlf.h"
#include "spot_utils.h"
#include <spot/tl/parse.hh>

static RtlfParsingResult toRtlfParsingResult(spot::parse_error_list&& spotErrors);

using namespace SpotUtils;

RtlfParsingResult parseRtlf(const std::string_view rtlf)
{
    spot::parsed_formula parsedFormula { spot::parse_infix_psl(std::string { rtlf }) };
    spot::parse_error_list spotErrors { parsedFormula.errors };

    if (!spotErrors.empty())
    {
        return toRtlfParsingResult(std::move(spotErrors));
    }

    spot::formula rtlfFormula { std::move(parsedFormula.f) };
    if (!rtlfFormula.is_ltl_formula())
    {
        return RtlfParsingResult::notLtlFormula(std::move(rtlfFormula));
    }

    return RtlfParsingResult::ok(std::move(rtlfFormula));
}

RtlfParsingResult parseRtlf(const std::string_view rtlf, const spot::atomic_prop_set& allowedAtomicPropositions)
{
    RtlfParsingResult rtlfParsingResult { parseRtlf(rtlf) };
    if (!rtlfParsingResult)
    {
        return rtlfParsingResult;
    }

    spot::formula& rtlfFormula { *rtlfParsingResult };
    spot::atomic_prop_vector illegalAtoms { collectAtomsNotIn(allowedAtomicPropositions, rtlfFormula) };

    if (!illegalAtoms.empty())
    {
        rtlfParsingResult.setIllegalAtoms(std::move(illegalAtoms));
    }

    return rtlfParsingResult;
}


static RtlfParsingResult toRtlfParsingResult(spot::parse_error_list&& spotErrors)
{
    std::vector<ParserError> parserErrors {};
    parserErrors.reserve(spotErrors.size());

    std::transform(spotErrors.begin(), spotErrors.end(),
        std::back_inserter(parserErrors),
        [](spot::one_parse_error& spotError)
        {
            return ParserError { std::move(spotError) };
        });

    return RtlfParsingResult::withParserErrors(std::move(parserErrors));
}