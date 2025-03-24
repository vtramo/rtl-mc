/*!
* \file parsertl.h
 * \brief Functions for parsing RTL formulae.
 */

#pragma once

#include <string>
#include "RtlParsingResult.h"

/*!
 * \brief Parses an RTL formula from string input.
 * \param rtl The input string containing the RTL formula to parse
 * \return RtlParsingResult containing either the parsed formula or parsing errors
 */
RtlParsingResult parseRtl(std::string_view rtl);

/*!
 * \brief Parses an RTL formula with atomic proposition validation.
 * \param rtl The input string containing the RTL formula to parse
 * \param allowedAtomicPropositions Set of allowed atomic propositions
 * \return RtlParsingResult containing parsing results and validation info
 */
RtlParsingResult parseRtl(std::string_view rtl, const spot::atomic_prop_set& allowedAtomicPropositions);