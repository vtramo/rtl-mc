//
// Created by vincenzo on 16/12/24.
//

#ifndef ATOMINTERPRETATION_H
#define ATOMINTERPRETATION_H

#include "ppl.hh"
#include "ppl_aliases.h"

class AtomInterpretation {
public:
    explicit AtomInterpretation(const Powerset& interpretation);
    explicit AtomInterpretation(
        Powerset interpretation,
        const Powerset& invariant
    );

    const Powerset& interpretation() const;
    const Powerset& notInterpretation() const;

    bool operator==(const AtomInterpretation& other) const;
private:
    const Powerset m_interpretation {};
    const Powerset m_notInterpretation {};
};



#endif //ATOMINTERPRETATION_H
