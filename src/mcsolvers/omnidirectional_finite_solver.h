#pragma once

#include <spot/twaalgos/reachiter.hh>

#include "PolyhedralSystem.h"
#include "OmnidirectionalPolyhedralAbstraction.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "FiniteLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "FiniteSynchronousProduct.h"
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
    FiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildFiniteLtlAutomaton(
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
            FiniteSynchronousProduct* finiteSynchronousProduct,
            std::vector<bool>* visited,
            spot::twa_graph_ptr transposedSynchronousProduct,
            PowersetSharedPtr result
        )
            : spot::twa_reachable_iterator_depth_first(transposedSynchronousProduct)
        {
            m_result = result;
            m_finiteSynchronousProduct = finiteSynchronousProduct;
            m_visited = visited;
            m_transposeFiniteSynchronousProduct = transposedSynchronousProduct;
        }

        void process_state(const spot::state* state, const int _, spot::twa_succ_iterator* __) override
        {
            unsigned productStateNumber { m_transposeFiniteSynchronousProduct->state_number(state) };
            // std::cout << "process state " << productStateNumber << '\n';
            m_visited->operator[](productStateNumber) = true;
            if (m_finiteSynchronousProduct->isInitialState(productStateNumber))
                addInitialStateDenotationToResult(productStateNumber);
        }

        bool want_state(const spot::state* state) const override
        {
            // return true;
            unsigned productStateNumber { m_transposeFiniteSynchronousProduct->state_number(state) };
            return !m_visited->operator[](productStateNumber);
        }

        PowersetSharedPtr result() { return m_result; }
    private:
        FiniteSynchronousProduct* m_finiteSynchronousProduct {};
        spot::twa_graph_ptr m_transposeFiniteSynchronousProduct {};
        std::vector<bool>* m_visited {};
        PowersetSharedPtr m_result {};

        void addInitialStateDenotationToResult(const int state)
        {
            // std::cout << "adding state: " << state << std::endl;
            auto [nfaState, abstractionState] = m_finiteSynchronousProduct->productStatePair(state);
            // std::cout << "state product (" << nfaState << ", " << abstractionState << ")" << std::endl;
            // PowersetConstSharedPtr points { m_finiteSynchronousProduct->points(state) };
            PowersetConstSharedPtr points { m_finiteSynchronousProduct->points(state) };
            PPLUtils::fusion(*m_result, *points);
        }
    };

    FiniteSynchronousProduct finiteSynchronousProduct { finiteLtlAutomaton, abstraction };
    std::cout << "Total states synch product: " << finiteSynchronousProduct.totalStates() << std::endl;
    std::cout << "Total edges synch product: " << finiteSynchronousProduct.totalEdges() << std::endl;
    std::cout << "Total initial states synch product: " << finiteSynchronousProduct.totalInitialStates() << std::endl;
    std::cout << "Total final states synch product: " << finiteSynchronousProduct.totalAcceptingStates() << std::endl;
    for (int initialState: finiteSynchronousProduct.initialStates())
    {
        // std::cout << "Initial state " << initialState << std::endl;
        const auto& [nfaState, abstractionState] { finiteSynchronousProduct.productStatePair(initialState) };
        const auto& points { abstraction->points(abstractionState) };
        std::cout << "Initial state synch product: " << PPLOutput::toString(*points, polyhedralSystem->symbolTable()) << std::endl;
    }
    // for (int finalState: finiteSynchronousProduct.acceptingStates())
        // std::cout << "Final state " << finalState << std::endl;

    spot::twa_graph_ptr transposedSynchronousProduct { finiteSynchronousProduct.transpose() };
    std::vector visited(finiteSynchronousProduct.totalStates(), false);
    PowersetSharedPtr result { std::make_shared<Powerset>(polyhedralSystem->spaceDimension(), PPL::EMPTY) };
    for (int finalState: finiteSynchronousProduct.acceptingStates())
    {
        ExistentialDenotationDfs dfs { &finiteSynchronousProduct, &visited, transposedSynchronousProduct, result };
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
    FiniteLtlAutomatonConstSharedPtr finiteLtlAutomaton {
        buildFiniteLtlAutomaton(
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