#pragma once

#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralLtlFiniteAutomaton.h"

class BackwardNFA;
using BackwardNFAConstSharedPtr = std::shared_ptr<const BackwardNFA>;

class BackwardNFA: public PolyhedralLtlFiniteAutomaton
{
public:
    BackwardNFA(const BackwardNFA& other);

    static BackwardNFAConstSharedPtr buildAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "BackwardNFA"
    );

    static BackwardNFAConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "BackwardNFA"
    );

protected:
    friend class BackwardNFADepthFirstSearch;
    friend class BackwardNFAPermutator;

    BackwardNFA();
    BackwardNFA(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "BackwardNFA"
    );

    BackwardNFA(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "BackwardNFA"
    );

    void createNewEdge(unsigned srcState, unsigned dstState) override;
    void postprocessAutomaton() override;
    void createDummyInitialStateWithEdgesToReachableAcceptingStates();
};
