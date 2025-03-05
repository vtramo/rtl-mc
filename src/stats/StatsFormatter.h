#pragma once

#include "PolyhedralSystemStats.h"
#include "AutomatonStats.h"
#include "DenotStats.h"
#include "RtlFormulaStats.h"
#include "DiscretisationStats.h"

#include <string>
#include <spdlog/fmt/fmt.h>

using namespace std::literals;

class StatsFormatter
{
public:
    StatsFormatter(
        PolyhedralSystemStats&& polyhedralSystemStats,
        RtlFormulaStats&& rtlFormulaStats,
        DiscretisationStats&& discretisationStats,
        AutomatonStats&& automatonStats,
        DenotStats&& denotStats
    ) : m_polyhedralSystemStats { std::move(polyhedralSystemStats) }
      , m_rtlFormulaStats { std::move(rtlFormulaStats) }
      , m_discretisationStats { std::move(discretisationStats) }
      , m_automatonStats { std::move(automatonStats) }
      , m_denotStats { std::move(denotStats) }
    {
    }

    std::string operator() (std::string&& input)
    {
        return format(std::move(input));
    }

    std::string format(std::string&& input)
    {
        replacePlaceholders(input);

        return fmt::format(input,
                           m_polyhedralSystemStats.totalAtomicPropositions,
                           m_polyhedralSystemStats.spaceDimension,
                           m_rtlFormulaStats.totalAtomicPropositions,
                           m_rtlFormulaStats.length,
                           m_discretisationStats.discreteLtlFormulaTotalAtomicPropositions,
                           m_discretisationStats.discreteLtlFormulaLength,
                           m_discretisationStats.discretisationExecutionTimeSeconds,
                           m_automatonStats.translationFormulaIntoTgba.totalAcceptingSets,
                           m_automatonStats.translationFormulaIntoTgba.totalStates,
                           m_automatonStats.translationFormulaIntoTgba.totalEdges,
                           m_automatonStats.translationFormulaIntoTgba.optimizationLevel,
                           m_automatonStats.translationFormulaIntoTgba.executionTimeSeconds,
                           m_automatonStats.translationTgbaIntoNfaStats.executionTimeSeconds,
                           m_automatonStats.translationTgbaIntoNfaStats.totalStates,
                           m_automatonStats.translationTgbaIntoNfaStats.totalEdges,
                           m_automatonStats.translationTgbaIntoNfaStats.totalFinalStates,
                           m_automatonStats.nfaOptimizations.executionTimeSeconds,
                           m_automatonStats.nfaOptimizations.totalStates,
                           m_automatonStats.nfaOptimizations.totalEdges,
                           m_automatonStats.nfaOptimizations.totalFinalStates,
                           m_automatonStats.nfaConstructionStats.executionTimeSeconds,
                           m_automatonStats.nfaConstructionStats.totalStates,
                           m_automatonStats.nfaConstructionStats.totalEdges,
                           m_automatonStats.nfaConstructionStats.totalAcceptingStates,
                           m_automatonStats.nfaConstructionStats.totalInitialStates,
                           m_automatonStats.nfaConstructionStats.sccInfo->scc_count(),
                           m_automatonStats.nfaConstructionStats.maxNumberPatches,
                           m_automatonStats.nfaConstructionStats.totalNumberPatches,
                           m_automatonStats.nfaConstructionStats.maxRecursiveDepthOnTheFly,
                           m_denotStats.totalIterations,
                           m_denotStats.executionTimeSeconds);
    }

private:
    static constexpr std::string_view s_Pta{ "%Pta"sv };  // Polyhedral System total atomic propositions
    static constexpr std::string_view s_Psp{ "%Psp"sv };  // Polyhedral System space dimension

    static constexpr std::string_view s_rta{ "%rta"sv };  // Rtlf formula total atomic propositions
    static constexpr std::string_view s_rl{ "%rl"sv };    // Rtlf formula length

    static constexpr std::string_view s_dta{ "%dta"sv };  // Discrete LTL formula total atomic propositions
    static constexpr std::string_view s_dl{ "%dl"sv };    // Discrete LTL formula length
    static constexpr std::string_view s_dx{ "%dx"sv };    // Discretisation execution time in s

    static constexpr std::string_view s_Ata{ "%Ata"sv };  // Translation Discrete LTL formula into TGBA total accepting sets
    static constexpr std::string_view s_Ats{ "%Ats"sv };  // Translation Discrete LTL formula into TGBA total states
    static constexpr std::string_view s_Ate{ "%Ate"sv };  // Translation Discrete LTL formula into TGBA total edges
    static constexpr std::string_view s_Ato{ "%Ato"sv };  // Translation Discrete LTL formula into TGBA optimization level
    static constexpr std::string_view s_Atx{ "%Atx"sv };  // Translation Discrete LTL formula into TGBA execution time in s

    static constexpr std::string_view s_Afx{ "%Afx"sv };  // TGBA to NFA execution time in s
    static constexpr std::string_view s_Afs{ "%Afs"sv };  // TGBA to NFA total states
    static constexpr std::string_view s_Afe{ "%Afe"sv };  // TGBA to NFA total edges
    static constexpr std::string_view s_Aff{ "%Aff"sv };  // TGBA to NFA total final states

    static constexpr std::string_view s_Afox{ "%Afox"sv };  // NFA optimizations execution time in s
    static constexpr std::string_view s_Afos{ "%Afos"sv };  // NFA after optimizations total states
    static constexpr std::string_view s_Afoe{ "%Afoe"sv };  // NFA after optimizations total edges
    static constexpr std::string_view s_Afof{ "%Afof"sv };  // NFA after optimizations total final states

    static constexpr std::string_view s_Abx{ "%Abx"sv };  // BackwardNFA construction execution time in s
    static constexpr std::string_view s_Abs{ "%Abs"sv };  // BackwardNFA total states
    static constexpr std::string_view s_Abe{ "%Abe"sv };  // BackwardNFA total edges
    static constexpr std::string_view s_Abf{ "%Abf"sv };  // BackwardNFA total final states
    static constexpr std::string_view s_Abi{ "%Abi"sv };  // BackwardNFA total initial states
    static constexpr std::string_view s_Abc{ "%Abc"sv };  // BackwardNFA total strongly connected components
    static constexpr std::string_view s_Abm{ "%Abm"sv };  // BackwardNFA maximum number of patches in the denotation of any state
    static constexpr std::string_view s_Abp{ "%Abp"sv };  // BackwardNFA total patches
    static constexpr std::string_view s_Abr{ "%Abr"sv };  // BackwardNFA max recursive depth

    static constexpr std::string_view s_Dt{ "%Dt"sv };    // Denot total iterations
    static constexpr std::string_view s_Dx{ "%Dx"sv }; // Denot execution time in s


    PolyhedralSystemStats m_polyhedralSystemStats {};
    RtlFormulaStats m_rtlFormulaStats {};
    DiscretisationStats m_discretisationStats {};
    AutomatonStats m_automatonStats {};
    DenotStats m_denotStats {};

    static void replacePlaceholders(std::string& input)
    {
        replace(input, s_Pta, "{0}");
        replace(input, s_Psp, "{1}");
        replace(input, s_rta, "{2}");
        replace(input, s_rl, "{3}");
        replace(input, s_dta, "{4}");
        replace(input, s_dl, "{5}");
        replace(input, s_dx, "{6}");
        replace(input, s_Ata, "{7}");
        replace(input, s_Ats, "{8}");
        replace(input, s_Ate, "{9}");
        replace(input, s_Ato, "{10}");
        replace(input, s_Atx, "{11}");
        replace(input, s_Afx, "{12}");
        replace(input, s_Afs, "{13}");
        replace(input, s_Afe, "{14}");
        replace(input, s_Aff, "{15}");
        replace(input, s_Afox, "{16}");
        replace(input, s_Afos, "{17}");
        replace(input, s_Afoe, "{18}");
        replace(input, s_Afof, "{19}");
        replace(input, s_Abx, "{20}");
        replace(input, s_Abs, "{21}");
        replace(input, s_Abe, "{22}");
        replace(input, s_Abf, "{23}");
        replace(input, s_Abi, "{24}");
        replace(input, s_Abc, "{25}");
        replace(input, s_Abm, "{26}");
        replace(input, s_Abp, "{27}");
        replace(input, s_Abr, "{28}");
        replace(input, s_Dt, "{29}");
        replace(input, s_Dx, "{30}");
    }

    static void replace(std::string& str, const std::string_view from, const std::string_view to)
    {
        size_t pos { str.find(from) };

        while (pos != std::string::npos) {
            str.replace(pos, from.size(), to);
            pos = str.find(from, pos + to.size());
        }
    }
};