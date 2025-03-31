#pragma once

#include "PolyhedralLtlAutomaton.h"

class PolyhedralLtlBüchiAutomaton;
using PolyhedralLtlBüchiAutomatonConstSharedPtr = std::shared_ptr<PolyhedralLtlBüchiAutomaton>;

class PolyhedralLtlBüchiAutomaton: public PolyhedralLtlAutomaton
{
public:
    PolyhedralLtlBüchiAutomaton(const PolyhedralLtlBüchiAutomaton& other);

    static PolyhedralLtlBüchiAutomatonConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "PolyhedralLtlBüchiAutomaton"
    );

protected:
    PolyhedralLtlBüchiAutomaton();

    explicit PolyhedralLtlBüchiAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralLtlBüchiAutomaton"
    );

    explicit PolyhedralLtlBüchiAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralLtlBüchiAutomaton"
    );
};