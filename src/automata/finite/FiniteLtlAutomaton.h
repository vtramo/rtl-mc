#pragma once

#include "LtlAutomaton.h"
#include "AutomatonStats.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "DiscreteLtlFormula.h"
#include "StateDenotation.h"

class FiniteLtlAutomaton;
using FiniteLtlAutomatonConstSharedPtr = std::shared_ptr<FiniteLtlAutomaton>;

class FiniteLtlAutomaton: public LtlAutomaton
{
public:
    FiniteLtlAutomaton(const FiniteLtlAutomaton& other);

    [[nodiscard]] unsigned totalInitialStates() const override;
    [[nodiscard]] unsigned totalEdges() const override;
    [[nodiscard]] unsigned totalAcceptingStates() const override;
    [[nodiscard]] unsigned totalStates() const override;
    [[nodiscard]] unsigned isInitialState(unsigned state) const override;
    [[nodiscard]] bool isAcceptingState(unsigned state) const override;
    [[nodiscard]] const std::unordered_set<unsigned>& initialStates() const;
    [[nodiscard]] const std::unordered_set<unsigned>& acceptingStates() const;
    [[nodiscard]] int countSuccessors(unsigned state) const override;
    [[nodiscard]] const DiscreteLtlFormula& formula() const override;
    [[nodiscard]] const StateDenotation& stateDenotation(unsigned state) const;
    [[nodiscard]] const AutomatonStats& stats() const;

    static FiniteLtlAutomatonConstSharedPtr buildAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        spot::postprocessor::optimization_level optimizationLevel = spot::postprocessor::optimization_level::High,
        bool anyOption = false,
        std::string_view name = "FiniteLtlAutomaton"
    );

protected:
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};
    unsigned m_dummyInitialState {};
    unsigned m_dummyInitialEdges {};
    std::unordered_map<unsigned, StateDenotation> m_stateDenotationById {};
    PolyhedralSystemFormulaDenotationMap m_formulaDenotationMap {};
    AutomatonStats m_automatonStats {};
    int m_maxRecursiveDepth {};

    FiniteLtlAutomaton();

    explicit FiniteLtlAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "FiniteLtlAutomaton"
    );

    explicit FiniteLtlAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name = "FiniteLtlAutomaton"
    );

    friend std::ostream& operator<< (std::ostream& out, const FiniteLtlAutomaton& nfa);

    using RenumberingContextVoidPtr = void*;
    struct RenumberingContext
    {
        RenumberingContext(
            std::unordered_set<unsigned>* initialStates,
            std::unordered_set<unsigned>* acceptingStates,
            std::unordered_map<unsigned, StateDenotation>* stateDenotationById,
            unsigned* dummyInitialState
        )
            : m_initialStates { initialStates }
            , m_acceptingStates { acceptingStates }
            , m_stateDenotationById { stateDenotationById }
            , m_dummyInitialState { dummyInitialState }
        {}

        explicit RenumberingContext(std::unordered_set<unsigned>* acceptingStates)
            : m_acceptingStates { acceptingStates }
        {}

        std::unordered_set<unsigned>* m_initialStates {};
        std::unordered_set<unsigned>* m_acceptingStates {};
        std::unordered_map<unsigned, StateDenotation>* m_stateDenotationById {};
        unsigned* m_dummyInitialState {};
    };

    void initializeAutomaton() override;
    void buildAutomaton(const spot::const_twa_graph_ptr& nfa, const std::unordered_set<unsigned>& nfaAcceptingStates);
    StateDenotation extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& nfa, const bdd& guard);
    void eraseInitialEdgesWithEmptyDenotation(spot::twa_graph_ptr nfa);
    void createDummyInitialStateWithEdgesToInitialStates();
    virtual void createNewEdge(unsigned srcState, unsigned dstState);
    void purgeUnreachableStatesThenRenumberAcceptingStates(spot::twa_graph_ptr nfa, std::unordered_set<unsigned>& nfaAcceptingStates);
    void logNfaConstruction(double executionTimeSeconds);
    virtual void purgeUnreachableStates();
    std::unordered_set<unsigned> killAcceptingStates(const spot::twa_graph_ptr& graph);
    bdd stateLabelsAsBdd(unsigned outEdgeState) const;
    spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption) override;
    spot::twa_graph_ptr convertToNfa(spot::twa_graph_ptr tgba);
    void updateMaxNumberOfPatchesStats(int totPatches);
    void setNfaStats(double executionTimeSeconds, int totalPatches);

    static void renumberOrRemoveStatesAfterPurge(const std::vector<unsigned>& newst, RenumberingContextVoidPtr renumberingContextVoidPtr);
};