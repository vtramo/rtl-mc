#pragma once

#include "AutomatonStats.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"
#include "BackwardNFADepthFirstSearch.h"

class BackwardNFA {
public:
    using EdgeIterator = spot::internal::state_out<spot::digraph<spot::twa_graph_state, spot::twa_graph_edge_data>>;

    BackwardNFA() = default;
    BackwardNFA(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    BackwardNFA(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false
    );

    BackwardNFA(const BackwardNFA& other);

    [[nodiscard]] int totalStates() const;
    [[nodiscard]] int totalInitialStates() const;
    [[nodiscard]] int totalFinalStates() const;
    [[nodiscard]] int totalEdges() const;
    [[nodiscard]] bool isInitialState(int state) const;
    [[nodiscard]] bool isFinalState(int state) const;
    [[nodiscard]] const std::unordered_set<int>& initialStates() const;
    [[nodiscard]] const std::unordered_set<int>& finalStates() const;
    [[nodiscard]] bool hasPredecessors(int state) const;
    [[nodiscard]] EdgeIterator predecessors(int state) const;
    [[nodiscard]] int countPredecessors(int state) const;
    [[nodiscard]] const DiscreteLtlFormula& formula() const;
    [[nodiscard]] const StateDenotation& stateDenotation(int state) const;
    [[nodiscard]] spot::postprocessor::optimization_level optimizationLevel() const;
    [[nodiscard]] const AutomatonStats& stats() const;
    [[nodiscard]] int maxRecursiveDepth() const;

    void printHoaFormat(std::ostream& os) const;
    void printDotFormat(std::ostream& os) const;

private:
    spot::twa_graph_ptr m_backwardNfa {};
    std::unordered_set<int> m_initialStates {};
    std::unordered_set<int> m_finalStates {};
    int m_dummyInitialState {};
    int m_dummyInitialEdges {};
    std::unordered_map<int, StateDenotation> m_stateDenotationById {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    PolyhedralSystemFormulaDenotationMap m_formulaDenotationMap {};
    spot::postprocessor::optimization_level m_optimizationLevel {};
    AutomatonStats m_automatonStats {};
    int m_maxRecursiveDepth {};

    friend std::ostream& operator<< (std::ostream& out, const BackwardNFA& backwardNfa);
    friend class BackwardNFADepthFirstSearch;
    friend void permutation(const BackwardNFA& backwardNfa, const std::unordered_map<int, int>& permutation);

    using RenumberingContextVoidPtr = void*;
    struct RenumberingContext
    {
        RenumberingContext(
            std::unordered_set<int>* initialStates,
            std::unordered_set<int>* finalStates,
            std::unordered_map<int, StateDenotation>* stateDenotationById,
            int* dummyInitialState
        )
            : m_initialStates { initialStates }
            , m_finalStates { finalStates }
            , m_stateDenotationById { stateDenotationById }
            , m_dummyInitialState { dummyInitialState }
        {}

        explicit RenumberingContext(std::unordered_set<int>* finalStates)
            : m_finalStates { finalStates }
        {}

        std::unordered_set<int>* m_initialStates {};
        std::unordered_set<int>* m_finalStates {};
        std::unordered_map<int, StateDenotation>* m_stateDenotationById {};
        int* m_dummyInitialState {};
    };
    static void renumberOrRemoveStatesAfterPurge(const std::vector<unsigned>& newst, RenumberingContextVoidPtr renumberingContextVoidPtr);
    static std::unordered_set<int> killFinalStates(const spot::twa_graph_ptr& graph);

    void buildAutomaton(const spot::const_twa_graph_ptr& nfa, const std::unordered_set<int>& nfaAcceptingStates);
    StateDenotation extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard);
    void eraseInitialEdgesWithEmptyDenotation(spot::twa_graph_ptr nfa);
    void createDummyInitialStateWithEdgesToReachableFinalStates();
    void purgeUnreachableStatesThenRenumberFinalStates(spot::twa_graph_ptr nfa, std::unordered_set<int>& nfaFinalStates);
    void setMaxRecursiveDepth(int totalPatches);
    void logBackwardNfaConstruction(double executionTimeSeconds);
    void purgeUnreachableStates();
    spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption);
    spot::twa_graph_ptr convertToNfa(spot::twa_graph_ptr tgba);
    spot::const_twa_ptr twa() const;

    void updateMaxNumberOfPatchesStats(int totPatches);
    void setBackwardNfaStats(double executionTimeSeconds);
};