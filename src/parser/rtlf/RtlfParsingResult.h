#ifndef RTLFPARSINGRESULT_H
#define RTLFPARSINGRESULT_H

#include <vector>
#include <set>
#include <spot_utils.h>

#include "ParserError.h"

class RtlfParsingResult {
public:
    [[nodiscard]] bool ok() const { return !hasParserErrors() && isLtl() && isXFree() && !isUsingIllegalAtoms(); }
    [[nodiscard]] bool isLtl() const { return m_isLtl; }
    [[nodiscard]] bool isXFree() const { return m_isXFree; }
    [[nodiscard]] bool isUsingIllegalAtoms() const { return !m_illegalAtoms.empty(); }
    [[nodiscard]] bool hasParserErrors() const { return !m_parserErrors.empty(); }
    [[nodiscard]] std::vector<ParserError> parserErrors() const { return m_parserErrors; }
    [[nodiscard]] spot::atomic_prop_vector illegalAtoms() const { return m_illegalAtoms; }

    [[nodiscard]] spot::formula& operator* () { return *m_rtlf; }
    [[nodiscard]] bool operator! () const { return ok(); }

    [[nodiscard]] static RtlfParsingResult ok(spot::formula&& rtlf)
    {
      return RtlfParsingResult{ std::move(rtlf), {}, {}, true, true };
    }

    [[nodiscard]] static RtlfParsingResult withParserErrors(std::vector<ParserError>&& parserErrors)
    {
      return RtlfParsingResult { std::move(parserErrors) };
    }

    [[nodiscard]] static RtlfParsingResult notLtlFormula(spot::formula&& rtlf)
    {
      return RtlfParsingResult { std::move(rtlf), false, false };
    }

    [[nodiscard]] static RtlfParsingResult notXFree(spot::formula&& rtlf)
    {
      return RtlfParsingResult{ std::move(rtlf), true, false };
    }

private:
    std::optional<spot::formula> m_rtlf {};
    const std::vector<ParserError> m_parserErrors {};
    std::vector<spot::formula> m_illegalAtoms {};
    const bool m_isLtl { false };
    const bool m_isXFree { false };

    friend RtlfParsingResult parseRtlf(std::string_view rtlf, std::set<std::string>&& allowedAtomicPropositions);

    RtlfParsingResult(
      std::optional<spot::formula>&& rtlf,
      std::vector<ParserError>&& parserErrors,
      std::vector<spot::formula>&& forbiddenAtoms,
      const bool isLtl = false,
      const bool isXFree = false
    )
      : m_rtlf { std::move(rtlf) }
      , m_parserErrors { std::move(parserErrors) }
      , m_illegalAtoms { std::move(forbiddenAtoms) }
      , m_isLtl { isLtl }
      , m_isXFree { isXFree }
    {
    }

    explicit RtlfParsingResult(std::vector<ParserError>&& parserErrors)
      : RtlfParsingResult({}, std::move(parserErrors), {}, false, false)
    {
    }

    explicit RtlfParsingResult(
      spot::formula&& rtlf,
      const bool isLtl = false,
      const bool isXFree = false
    ) : RtlfParsingResult(std::move(rtlf), {}, {}, isLtl, isXFree)
    {
    }

    explicit RtlfParsingResult(
      std::vector<spot::formula>&& forbiddenAtoms,
      const bool isLtl = true,
      const bool isXFree = true
    ) : RtlfParsingResult({}, {}, std::move(forbiddenAtoms), isLtl, isXFree)
    {
    }

    explicit RtlfParsingResult(spot::formula&& rtlf) : RtlfParsingResult(std::move(rtlf), {}, {}, false, false)
    {
    }

    void setIllegalAtoms(std::vector<spot::formula>&& illegalAtoms)
    {
      m_illegalAtoms = std::move(illegalAtoms);
    }
};

#endif //RTLFPARSINGRESULT_H
