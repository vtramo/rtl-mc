#pragma once

#include <vector>
#include <set>
#include <spot_utils.h>

#include "ParserError.h"

class RtlfParsingResult {
public:
    [[nodiscard]] bool ok() const { return !hasParserErrors() && isLtl() && !isUsingIllegalAtoms(); }
    [[nodiscard]] bool isLtl() const { return m_isLtl; }
    [[nodiscard]] bool isUsingIllegalAtoms() const { return !m_illegalAtoms.empty(); }
    [[nodiscard]] bool hasParserErrors() const { return !m_parserErrors.empty(); }
    [[nodiscard]] std::vector<ParserError> parserErrors() const { return m_parserErrors; }
    [[nodiscard]] spot::atomic_prop_vector illegalAtoms() const { return m_illegalAtoms; }

    [[nodiscard]] spot::formula& operator* () {
      if (!m_rtlf) {
        throw std::runtime_error("Parsing failed: RtlfFormula is null due to errors...");
      }
      return *m_rtlf;
    }

    [[nodiscard]] bool operator! () const { return !ok(); }

    [[nodiscard]] static RtlfParsingResult ok(spot::formula&& rtlf)
    {
      return RtlfParsingResult{ std::move(rtlf), {}, {}, true };
    }

    [[nodiscard]] static RtlfParsingResult withParserErrors(std::vector<ParserError>&& parserErrors)
    {
      return RtlfParsingResult { std::move(parserErrors) };
    }

    [[nodiscard]] static RtlfParsingResult notLtlFormula(spot::formula&& rtlf)
    {
      return RtlfParsingResult { std::move(rtlf), false };
    }

private:
    std::optional<spot::formula> m_rtlf {};
    const std::vector<ParserError> m_parserErrors {};
    std::vector<spot::formula> m_illegalAtoms {};
    const bool m_isLtl { false };

    friend RtlfParsingResult parseRtlf(std::string_view rtlf, const spot::atomic_prop_set& allowedAtomicPropositions);

    RtlfParsingResult(
      std::optional<spot::formula>&& rtlf,
      std::vector<ParserError>&& parserErrors,
      std::vector<spot::formula>&& illegalAtoms,
      const bool isLtl = false
    )
      : m_rtlf { std::move(rtlf) }
      , m_parserErrors { std::move(parserErrors) }
      , m_illegalAtoms { std::move(illegalAtoms) }
      , m_isLtl { isLtl }
    {
    }

    explicit RtlfParsingResult(std::vector<ParserError>&& parserErrors)
      : RtlfParsingResult({}, std::move(parserErrors), {}, false)
    {
    }

    explicit RtlfParsingResult(
      spot::formula&& rtlf,
      const bool isLtl = false
    ) : RtlfParsingResult(std::move(rtlf), {}, {}, isLtl)
    {
    }

    explicit RtlfParsingResult(
      std::vector<spot::formula>&& forbiddenAtoms,
      const bool isLtl = true
    ) : RtlfParsingResult({}, {}, std::move(forbiddenAtoms), isLtl)
    {
    }

    explicit RtlfParsingResult(spot::formula&& rtlf) : RtlfParsingResult(std::move(rtlf), {}, {}, false)
    {
    }

    void setIllegalAtoms(std::vector<spot::formula>&& illegalAtoms)
    {
      m_illegalAtoms = std::move(illegalAtoms);
    }
};