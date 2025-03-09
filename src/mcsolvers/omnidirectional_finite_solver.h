#pragma once

#include <spot/twaalgos/reachiter.hh>

#include "PolyhedralSystem.h"
#include "OmnidirectionalPolyhedralAbstraction.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralFiniteLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "PolyhedralSynchronousProductAutomaton.h"
#include "GeneralPolyhedralAbstraction.h"
#include "automata_builder.h"

inline PowersetSharedPtr omnidirectionalFiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    auto abstraction { std::make_shared<OmnidirectionalPolyhedralAbstraction>(polyhedralSystem) };
    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralFiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildPolyhedralFiniteLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };

    std::cout << "nfa total states: " << finiteLtlAutomaton->totalStates() << std::endl;
    std::cout << "nfa total edges: " << finiteLtlAutomaton->totalEdges() << std::endl;
    std::cout << "nfa total initial states: " << finiteLtlAutomaton->totalInitialStates() << std::endl;
    std::cout << "nfa total final states: " << finiteLtlAutomaton->totalAcceptingStates() << std::endl;
    class ExistentialDenotationDfs: public spot::twa_reachable_iterator_depth_first
    {
    public:
        explicit ExistentialDenotationDfs(
            PolyhedralSynchronousProductAutomaton* PolyhedralSynchronousProductAutomaton,
            std::vector<bool>* visited,
            spot::twa_graph_ptr transposedSynchronousProduct,
            PowersetSharedPtr result
        )
            : spot::twa_reachable_iterator_depth_first(transposedSynchronousProduct)
        {
            m_result = result;
            m_PolyhedralSynchronousProductAutomaton = PolyhedralSynchronousProductAutomaton;
            m_visited = visited;
            m_transposePolyhedralSynchronousProductAutomaton = transposedSynchronousProduct;
        }

        void process_state(const spot::state* state, const int _, spot::twa_succ_iterator* __) override
        {
            unsigned productStateNumber { m_transposePolyhedralSynchronousProductAutomaton->state_number(state) };
            // std::cout << "process state " << productStateNumber << '\n';
            m_visited->operator[](productStateNumber) = true;
            if (m_PolyhedralSynchronousProductAutomaton->isInitialState(productStateNumber))
                addInitialStateDenotationToResult(productStateNumber);
        }

        bool want_state(const spot::state* state) const override
        {
            // return true;
            unsigned productStateNumber { m_transposePolyhedralSynchronousProductAutomaton->state_number(state) };
            return !m_visited->operator[](productStateNumber);
        }

        PowersetSharedPtr result() { return m_result; }
    private:
        PolyhedralSynchronousProductAutomaton* m_PolyhedralSynchronousProductAutomaton {};
        spot::twa_graph_ptr m_transposePolyhedralSynchronousProductAutomaton {};
        std::vector<bool>* m_visited {};
        PowersetSharedPtr m_result {};

        void addInitialStateDenotationToResult(const int state)
        {
            // std::cout << "adding state: " << state << std::endl;
            auto [nfaState, abstractionState] = m_PolyhedralSynchronousProductAutomaton->productStatePair(state);
            // std::cout << "state product (" << nfaState << ", " << abstractionState << ")" << std::endl;
            // PowersetConstSharedPtr points { m_PolyhedralSynchronousProductAutomaton->points(state) };
            PowersetConstSharedPtr points { m_PolyhedralSynchronousProductAutomaton->points(state) };
            PPLUtils::fusion(*m_result, *points);
        }
    };

    PolyhedralSynchronousProductAutomaton PolyhedralSynchronousProductAutomaton { finiteLtlAutomaton, abstraction };
    std::cout << "Total states synch product: " << PolyhedralSynchronousProductAutomaton.totalStates() << std::endl;
    std::cout << "Total edges synch product: " << PolyhedralSynchronousProductAutomaton.totalEdges() << std::endl;
    std::cout << "Total initial states synch product: " << PolyhedralSynchronousProductAutomaton.totalInitialStates() << std::endl;
    std::cout << "Total final states synch product: " << PolyhedralSynchronousProductAutomaton.totalAcceptingStates() << std::endl;
    for (int initialState: PolyhedralSynchronousProductAutomaton.initialStates())
    {
        // std::cout << "Initial state " << initialState << std::endl;
        const auto& [nfaState, abstractionState] { PolyhedralSynchronousProductAutomaton.productStatePair(initialState) };
        const auto& points { abstraction->points(abstractionState) };
        std::cout << "Initial state synch product: " << PPLOutput::toString(*points, polyhedralSystem->symbolTable()) << std::endl;
    }
    // for (int finalState: PolyhedralSynchronousProductAutomaton.acceptingStates())
        // std::cout << "Final state " << finalState << std::endl;

    spot::twa_graph_ptr transposedSynchronousProduct { PolyhedralSynchronousProductAutomaton.transpose() };
    std::vector visited(PolyhedralSynchronousProductAutomaton.totalStates(), false);
    PowersetSharedPtr result { std::make_shared<Powerset>(polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (int finalState: PolyhedralSynchronousProductAutomaton.acceptingStates())
    {
        ExistentialDenotationDfs dfs { &PolyhedralSynchronousProductAutomaton, &visited, transposedSynchronousProduct, result };
        // std::cout << "final state: " << finalState << std::endl;
        transposedSynchronousProduct->set_init_state(finalState);
        dfs.run();
    }
    const Powerset universe { polyhedralSystem->spaceDimension(), PPL::UNIVERSE };
    PowersetSharedPtr result2 { reachPlus(universe, *result, polyhedralSystem->preFlow()) };
    return result;
}

inline int maxPatches(const std::vector<Observable>& observables)
{
    int maxPatches {};

    for (const Observable& observable: observables)
    {
        maxPatches = std::max(maxPatches, observable.totalPatches());
    }

    return maxPatches;
}

inline PowersetSharedPtr generalFiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    DiscreteLtlFormula discreteLtlFormula { DiscreteFiniteLtlFormula::discretise(std::move(rtlFormula)).toLtl() };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralFiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildPolyhedralFiniteLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };
    std::vector observables { polyhedralSystem->generateObservables() };
    const unsigned sufficientHorizon { 2 * finiteLtlAutomaton->totalStates() * maxPatches(observables) };
    GeneralPolyhedralAbstraction generalPolyhedralAbstraction {
        polyhedralSystem,
        std::move(observables),
        static_cast<int>(sufficientHorizon)
    };

    generalPolyhedralAbstraction.printDotFormat(std::cout);
    return {};
}