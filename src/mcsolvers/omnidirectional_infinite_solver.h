#pragma once

#include "PolyhedralSystem.h"
#include "DiscreteLtlFormula.h"
#include "DiscreteFiniteLtlFormula.h"
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
    std::cout<< "emptiness: " <<  spot::generic_emptiness_check(buchiLtlAutomaton->twa()) << std::endl;
    buchiLtlAutomaton->printDotFormat(std::cout);
    spot::emptiness_check_ptr explicit_se05_search = spot::explicit_se05_search(buchiLtlAutomaton->twa());
    spot::emptiness_check_result_ptr emptiness_check_result_ptr = explicit_se05_search->check();
    std::cout<< "emptiness2: " <<  *emptiness_check_result_ptr->accepting_run() << std::endl;

    return std::make_shared<Powerset>(PPL::EMPTY);
}