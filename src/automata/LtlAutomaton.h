#pragma once

#include <spot/twaalgos/postproc.hh>

#include "Automaton.h"
#include "DiscreteLtlFormula.h"
#include "AutomatonStats.h"

class LtlAutomaton: public Automaton
{
public:
    LtlAutomaton();
    explicit LtlAutomaton(
        std::string_view name,
        const DiscreteLtlFormula& discreteLtlFormula
    );
    explicit LtlAutomaton(
        std::string_view name,
        DiscreteLtlFormula&& discreteLtlFormula
    );
    explicit LtlAutomaton(
        std::string_view name,
        spot::twa_graph_ptr automaton,
        const DiscreteLtlFormula& discreteLtlFormula
    );
    ~LtlAutomaton() override;

    [[nodiscard]] virtual const DiscreteLtlFormula& formula() const;
    [[nodiscard]] virtual spot::postprocessor::optimization_level optimizationLevel() const;

protected:
    DiscreteLtlFormula m_discreteLtlFormula {};
    spot::postprocessor::optimization_level m_optimizationLevel {};
    AutomatonStats::TranslationFormulaIntoTgbaStats m_translationStats {};

    virtual spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption);
};
