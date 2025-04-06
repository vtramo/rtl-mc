#pragma once

#include <spot/twaalgos/postproc.hh>

#include "Automaton.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralLtlAutomatonStats.h"
#include "StateDenotation.h"
#include "PolyhedralSystemFormulaDenotationMap.h"

class PolyhedralLtlAutomaton;
using PolyhedralLtlAutomatonConstSharedPtr = std::shared_ptr<const PolyhedralLtlAutomaton>;
using PolyhedralLtlAutomatonSharedPtr = std::shared_ptr<PolyhedralLtlAutomaton>;

class PolyhedralLtlAutomaton: public Automaton
{
public:
    ~PolyhedralLtlAutomaton() override;

    [[nodiscard]] unsigned totalInitialStates() const override;
    [[nodiscard]] unsigned totalEdges() const override;
    [[nodiscard]] unsigned totalAcceptingStates() const override;
    [[nodiscard]] unsigned totalStates() const override;
    [[nodiscard]] bool isInitialState(unsigned state) const override;
    [[nodiscard]] bool isAcceptingState(unsigned state) const override;
    [[nodiscard]] virtual const std::unordered_set<unsigned>& initialStates() const;
    [[nodiscard]] virtual const std::unordered_set<unsigned>& acceptingStates() const;
    [[nodiscard]] int countSuccessors(unsigned state) const override;
    [[nodiscard]] const StateDenotation& stateDenotation(unsigned state) const;
    [[nodiscard]] const PolyhedralLtlAutomatonStats& stats() const override;
    [[nodiscard]] virtual const DiscreteLtlFormula& formula() const;
    [[nodiscard]] virtual spot::postprocessor::optimization_level optimizationLevel() const;
    [[nodiscard]] PPL::dimension_type spaceDimension() const;

protected:
    std::unordered_set<unsigned> m_initialStates {};
    std::unordered_set<unsigned> m_acceptingStates {};
    unsigned m_dummyInitialState {};
    unsigned m_dummyEdges {};
    std::unordered_map<unsigned, StateDenotation> m_stateDenotationById {};
    PolyhedralSystemFormulaDenotationMap m_polyhedralSystemFormulaDenotationMap {};
    std::shared_ptr<PolyhedralLtlAutomatonStats> m_polyhedralLtlAutomatonStats {};
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

    void initialiseAutomaton() override;
    void initialiseStats() override;
    virtual void buildAutomaton(const spot::const_twa_graph_ptr& twaGraph);
    virtual bool isAcceptingEdge(const spot::const_twa_graph_ptr& graph, const Edge& edge);
    virtual bool filterEdge(const spot::const_twa_graph_ptr& graph, const Edge& edge);
    virtual StateDenotation extractStateDenotationFromEdge(const spot::const_twa_graph_ptr& twaGraph, const Edge& edge);
    virtual bdd stateLabelsAsBdd(unsigned state) const;
    virtual void eraseInitialEdgesWithEmptyDenotation(spot::twa_graph_ptr twaGraph);
    virtual void createNewEdge(unsigned srcState, unsigned dstState);
    virtual void purgeUnreachableStates(spot::twa_graph_ptr twaGraph);
    virtual void purgeUnreachableStates();
    virtual void postprocessAutomaton();
    virtual void onConstructionCompleted(double executionTimeSeconds);
    virtual void logConstructionCompleted(double executionTimeSeconds);
    void createDummyInitialStateWithEdgesToInitialStates();
    void updatePatchStats(int totPatches);
    virtual spot::twa_graph_ptr translateDiscreteLtlFormulaIntoTgba(bool anyOption);
    static void renumberOrRemoveStatesAfterPurge(const std::vector<unsigned>& newst, RenumberingContextVoidPtr renumberingContextVoidPtr);
};
