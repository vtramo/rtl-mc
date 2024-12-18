//
// Created by vincenzo on 16/12/24.
//

#ifndef ATOMINTERPRETATION_H
#define ATOMINTERPRETATION_H

#include "ppl.hh"

namespace PPL = Parma_Polyhedra_Library;

class AtomInterpretation {
public:
    explicit AtomInterpretation(const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& interpretation);
    explicit AtomInterpretation(
        PPL::Pointset_Powerset<PPL::NNC_Polyhedron> interpretation,
        const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& invariant
    );

    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& interpretation() const;
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron>& notInterpretation() const;

    bool operator==(const AtomInterpretation& other) const;
private:
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron> m_interpretation {};
    const PPL::Pointset_Powerset<PPL::NNC_Polyhedron> m_notInterpretation {};
};



#endif //ATOMINTERPRETATION_H
