#ifndef PARSERTLF_H
#define PARSERTLF_H

#include <string>
#include "RtlfParsingResult.h"

RtlfParsingResult parseRtlf(std::string_view rtlf);
RtlfParsingResult parseRtlf(std::string_view rtlf, const spot::atomic_prop_set& allowedAtomicPropositions);

#endif //PARSERTLF_H
