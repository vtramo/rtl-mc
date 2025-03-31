#pragma once

#include "PolyhedralLtlAutomaton.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"
#include "PolyhedralLtlFiniteAutomatonStats.h"
#include "PolyhedralLtlFiniteAutomaton.h"

class PolyhedralLtlFiniteAutomaton;
using PolyhedralLtlFiniteAutomatonConstSharedPtr = std::shared_ptr<PolyhedralLtlFiniteAutomaton>;

class PolyhedralLtlFiniteAutomaton: public PolyhedralLtlAutomaton
{
public:
    PolyhedralLtlFiniteAutomaton(const PolyhedralLtlFiniteAutomaton& other);

    static PolyhedralLtlFiniteAutomatonConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "PolyhedralLtlFiniteAutomaton"
    );

    [[nodiscard]] const PolyhedralLtlFiniteAutomatonStats& stats() const override;

protected:
    std::shared_ptr<PolyhedralLtlFiniteAutomatonStats> m_polyhedralFiniteLtlAutomatonStats {};

    PolyhedralLtlFiniteAutomaton();
    void initialiseStats() override;

    explicit PolyhedralLtlFiniteAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralLtlFiniteAutomaton"
    );

    explicit PolyhedralLtlFiniteAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralLtlFiniteAutomaton"
    );

    void initialiseAutomaton() override;
    void onConstructionCompleted(double executionTimeSeconds) override;
    spot::twa_graph_ptr convertToNfa(spot::twa_graph_ptr tgba);
};