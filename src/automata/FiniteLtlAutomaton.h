#pragma once

#include "AutomatonStats.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"
#include "BackwardNFADepthFirstSearch.h"

class FiniteLtlAutomaton {
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    FiniteLtlAutomaton() = default;
    FiniteLtlAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    FiniteLtlAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    FiniteLtlAutomaton(const FiniteLtlAutomaton& other);

    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] int initialState() const;
    [[nodiscard]] bool isInitialState(int state) const;
    [[nodiscard]] bool isFinalState(int state) const;
    [[nodiscard]] const std::unordered_set<int>& finalStates() const;
    [[nodiscard]] bool hasSuccessors(int state) const;
    [[nodiscard]] EdgeIterator successors(int state) const;
    [[nodiscard]] int countSuccessors(int state) const;
    [[nodiscard]] const DiscreteLtlFormula& formula() const;
    [[nodiscard]] spot::const_twa_graph_ptr twa() const;
    [[nodiscard]] spot::postprocessor::optimization_level optimizationLevel() const;
    [[nodiscard]] const AutomatonStats& stats() const;
    [[nodiscard]] int maxRecursiveDepth() const;

    void printHoaFormat(std::ostream& os) const;
    void printDotFormat(std::ostream& os) const;

private:
    spot::twa_graph_ptr m_nfa {};
    std::unordered_set<int> m_finalStates {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    PolyhedralSystemFormulaDenotationMap m_formulaDenotationMap {};
    spot::postprocessor::optimization_level m_optimizationLevel {};
    AutomatonStats m_automatonStats {};
    int m_maxRecursiveDepth {};

    friend std::ostream& operator<< (std::ostream& out, const FiniteLtlAutomaton& finiteLtlAutomaton);

    using RenumberingContextVoidPtr = void*;
    struct RenumberingContext
    {
        explicit RenumberingContext(std::unordered_set<int>* finalStates)
            : m_finalStates { finalStates }
        {}

        std::unordered_set<int>* m_finalStates {};
    };

    static void renumberOrRemoveStatesAfterPurge(
        const std::vector<unsigned>& newst,
        RenumberingContextVoidPtr renumberingContextVoidPtr
    );

    StateDenotation extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard);
    std::unordered_set<int> killFinalStates();
    void eraseEdgesWithEmptyDenotation();
    void purgeUnreachableStatesThenRenumberFinalStates();
    void logConstructionCompleted();
    void purgeUnreachableStates();
    spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption, spot::bdd_dict_ptr bddDict);
    spot::twa_graph_ptr convertToNfa(spot::twa_graph_ptr tgba);

    std::array<int, 2> collectPatchStats();
    void updateMaxNumberOfPatchesStats(int totPatches);
    void setStats(int totalPatches, int maxNumberPatches);
};

using FiniteLtlAutomatonConstSharedPtr = std::shared_ptr<FiniteLtlAutomaton>;