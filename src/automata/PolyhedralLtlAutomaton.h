#pragma once

#include <spot/twaalgos/postproc.hh>

#include "Automaton.h"
#include "DiscreteLtlFormula.h"
#include "AutomatonStats.h"
#include "StateDenotation.h"
#include "PolyhedralSystemFormulaDenotationMap.h"

class PolyhedralLtlAutomaton;
using PolyhedralLtlAutomatonConstSharedPtr = std::shared_ptr<PolyhedralLtlAutomaton>;

class PolyhedralLtlAutomaton: public Automaton
{
public:
    ~PolyhedralLtlAutomaton() override;

    [[nodiscard]] unsigned totalInitialStates() const override;
    [[nodiscard]] unsigned totalEdges() const override;
    [[nodiscard]] unsigned totalAcceptingStates() const override;
    [[nodiscard]] unsigned totalStates() const override;
    [[nodiscard]] unsigned isInitialState(unsigned state) const override;
    [[nodiscard]] bool isAcceptingState(unsigned state) const override;
    [[nodiscard]] virtual const std::unordered_set<unsigned>& initialStates() const;
    [[nodiscard]] virtual const std::unordered_set<unsigned>& acceptingStates() const;
    [[nodiscard]] int countSuccessors(unsigned state) const override;
    [[nodiscard]] const StateDenotation& stateDenotation(unsigned state) const;
    [[nodiscard]] const AutomatonStats& stats() const;
    [[nodiscard]] virtual const DiscreteLtlFormula& formula() const;
    [[nodiscard]] virtual spot::postprocessor::optimization_level optimizationLevel() const;
    [[nodiscard]] PPL::dimension_type spaceDimension() const;

protected:
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};
    unsigned m_dummyInitialState {};
    unsigned m_dummyInitialEdges {};
    std::unordered_map<unsigned, StateDenotation> m_stateDenotationById {};
    PolyhedralSystemFormulaDenotationMap m_formulaDenotationMap {};
    AutomatonStats m_automatonStats {};
    DiscreteLtlFormula m_discreteLtlFormula {};
    spot::postprocessor::optimization_level m_optimizationLevel {};

    PolyhedralLtlAutomaton();
    explicit PolyhedralLtlAutomaton(const PolyhedralLtlAutomaton& other);
    explicit PolyhedralLtlAutomaton(
        const DiscreteLtlFormula& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name
    );
    explicit PolyhedralLtlAutomaton(
        DiscreteLtlFormula&& discreteLtlFormula,
        PolyhedralSystemFormulaDenotationMap&& polyhedralSystemLabelDenotationMap,
        std::string_view name
    );

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

    friend std::ostream& operator<< (std::ostream& out, const PolyhedralLtlAutomaton& automaton);

    void initializeAutomaton() override;
    virtual void buildAutomaton(const spot::const_twa_graph_ptr& twaGraph, const std::unordered_set<unsigned>& acceptingStates);
    virtual StateDenotation extractStateDenotationFromEdgeGuard(const spot::const_twa_graph_ptr& twaGraph, const bdd& guard);
    virtual bdd stateLabelsAsBdd(unsigned state) const;
    virtual void eraseInitialEdgesWithEmptyDenotation(spot::twa_graph_ptr twaGraph);
    virtual void createNewEdge(unsigned srcState, unsigned dstState);
    virtual void purgeUnreachableStatesThenRenumberAcceptingStates(spot::twa_graph_ptr twaGraph, std::unordered_set<unsigned>& acceptingStates);
    virtual void onConstructionCompleted(double executionTimeSeconds);
    virtual void logConstructionCompleted(double executionTimeSeconds);
    virtual void purgeUnreachableStates();
    void createDummyInitialStateWithEdgesToInitialStates();
    void updatePatchStats(int totPatches);
    virtual std::unordered_set<unsigned> killAcceptingStates(const spot::twa_graph_ptr& graph);
    virtual spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption);
    static void renumberOrRemoveStatesAfterPurge(const std::vector<unsigned>& newst, RenumberingContextVoidPtr renumberingContextVoidPtr);
};
