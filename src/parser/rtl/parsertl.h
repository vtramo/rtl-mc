#pragma once

#include <string>
#include "RtlParsingResult.h"

RtlParsingResult parseRtl(std::string_view rtl);
RtlParsingResult parseRtl(std::string_view rtl, const spot::atomic_prop_set& allowedAtomicPropositions);