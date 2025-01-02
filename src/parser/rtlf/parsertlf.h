#ifndef PARSERTLF_H
#define PARSERTLF_H

#include <set>
#include <string>
#include "RtlfParsingResult.h"

RtlfParsingResult parseRtlf(std::string_view rtlf);
RtlfParsingResult parseRtlf(std::string_view rtlf, std::set<std::string>&& allowedAtomicPropositions);

#endif //PARSERTLF_H
