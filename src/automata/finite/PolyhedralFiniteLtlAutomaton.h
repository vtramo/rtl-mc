#pragma once

#include "PolyhedralLtlAutomaton.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"

class PolyhedralFiniteLtlAutomaton;
using PolyhedralFiniteLtlAutomatonConstSharedPtr = std::shared_ptr<PolyhedralFiniteLtlAutomaton>;

class PolyhedralFiniteLtlAutomaton: public PolyhedralLtlAutomaton
{
public:
    PolyhedralFiniteLtlAutomaton(const PolyhedralFiniteLtlAutomaton& other);

    static PolyhedralFiniteLtlAutomatonConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "PolyhedralFiniteLtlAutomaton"
    );

protected:
    PolyhedralFiniteLtlAutomaton();

    explicit PolyhedralFiniteLtlAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralFiniteLtlAutomaton"
    );

    explicit PolyhedralFiniteLtlAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralFiniteLtlAutomaton"
    );

    void initializeAutomaton() override;
    void onConstructionCompleted(double executionTimeSeconds) override;
    spot::twa_graph_ptr convertToNfa(spot::twa_graph_ptr tgba);
    void setNfaStats(double executionTimeSeconds);
};