#pragma once

#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "PolyhedralSystemFormulaDenotationMap.h"
#include "PolyhedralBuchiLtlAutomaton.h"
#include "AutomatonOptimizationFlags.h"
#include "automata_builder.h"
#include <spot/twaalgos/couvreurnew.hh>
#include <spot/twaalgos/genem.hh>
#include <spot/twaalgos/se05.hh>

inline PowersetSharedPtr omnidirectionalInfiniteTimeSemanticsSolver(
    PolyhedralSystemSharedPtr polyhedralSystem,
    spot::formula rtlFormula,
    const AutomatonOptimizationFlags optimizationFlags
)
{
    DiscreteLtlFormula discreteLtlFormula { DiscreteLtlFormula::discretiseInfinite(std::move(rtlFormula)) };
    PolyhedralSystemFormulaDenotationMap polyhedralSystemFormulaDenotationMap { polyhedralSystem };
    PolyhedralBuchiLtlAutomatonConstSharedPtr buchiLtlAutomaton {
        buildPolyhedralBuchiLtlAutomaton(
            std::move(discreteLtlFormula),
            polyhedralSystemFormulaDenotationMap,
            optimizationFlags
        )
    };
    auto abstraction { std::make_shared<OmnidirectionalPolyhedralAbstraction>(polyhedralSystem) };
    auto synchronousProduct { std::make_shared<PolyhedralSynchronousProductAutomaton>(buchiLtlAutomaton, abstraction) };
    std::cout<< "emptiness: " <<  spot::generic_emptiness_check(synchronousProduct->twa()) << std::endl;
    synchronousProduct->printDotFormat(std::cout);
    spot::emptiness_check_ptr explicit_se05_search = spot::explicit_se05_search(synchronousProduct->twa());
    spot::emptiness_check_result_ptr emptiness_check_result_ptr = explicit_se05_search->check();
    while (emptiness_check_result_ptr != nullptr)
    {
        spot::twa_run_ptr twa_run_ptr = emptiness_check_result_ptr->accepting_run();
        for (spot::twa_run::step prefix : twa_run_ptr->prefix)
        {
            const spot::state* state = prefix.s;
            unsigned stateNumber { buchiLtlAutomaton->twa()->state_number(state) };
            std::cout << "stateNumber: " << stateNumber << std::endl;
        }
        std::cout << std::endl;
        emptiness_check_result_ptr = explicit_se05_search->check();
    }

    return std::make_shared<Powerset>(PPL::EMPTY);
}