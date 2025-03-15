#pragma once

#include <vector>
#include <spot_utils.h>

#include "ParserError.h"

class RtlParsingResult
{
public:
    [[nodiscard]] bool ok() const { return !hasParserErrors() && isLtl() && !isUsingIllegalAtoms(); }
    [[nodiscard]] bool isLtl() const { return m_isLtl; }
    [[nodiscard]] bool isUsingIllegalAtoms() const { return !m_illegalAtoms.empty(); }
    [[nodiscard]] bool hasParserErrors() const { return !m_parserErrors.empty(); }
    [[nodiscard]] std::vector<ParserError> parserErrors() const { return m_parserErrors; }
    [[nodiscard]] spot::atomic_prop_vector illegalAtoms() const { return m_illegalAtoms; }

    [[nodiscard]] spot::formula& operator* ()
    {
        if (!m_rtlf)
            throw std::runtime_error("Parsing failed: RtlfFormula is null due to errors...");

        return *m_rtlf;
    }

    [[nodiscard]] bool operator!() const { return !ok(); }

    [[nodiscard]] static RtlParsingResult ok(spot::formula&& rtlf)
    {
        return RtlParsingResult{std::move(rtlf), {}, {}, true};
    }

    [[nodiscard]] static RtlParsingResult withParserErrors(std::vector<ParserError>&& parserErrors)
    {
        return RtlParsingResult{std::move(parserErrors)};
    }

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
