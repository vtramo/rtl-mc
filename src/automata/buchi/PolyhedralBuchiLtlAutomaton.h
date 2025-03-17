#pragma once

#include "PolyhedralLtlAutomaton.h"

class PolyhedralBuchiLtlAutomaton;
using PolyhedralBuchiLtlAutomatonConstSharedPtr = std::shared_ptr<PolyhedralBuchiLtlAutomaton>;

class PolyhedralBuchiLtlAutomaton: public PolyhedralLtlAutomaton
{
public:
    PolyhedralBuchiLtlAutomaton(const PolyhedralBuchiLtlAutomaton& other);

    static PolyhedralBuchiLtlAutomatonConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "PolyhedralBuchiLtlAutomaton"
    );

protected:
    PolyhedralBuchiLtlAutomaton();

    explicit PolyhedralBuchiLtlAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralBuchiLtlAutomaton"
    );

    explicit PolyhedralBuchiLtlAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "PolyhedralBuchiLtlAutomaton"
    );
};