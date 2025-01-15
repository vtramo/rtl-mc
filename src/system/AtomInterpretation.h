//
// Created by vincenzo on 16/12/24.
//

#ifndef ATOMINTERPRETATION_H
#define ATOMINTERPRETATION_H

#include "ppl_aliases.h"

class AtomInterpretation {
public:
    explicit AtomInterpretation(const Powerset& interpretation);
    explicit AtomInterpretation(Powerset&& interpretation);
    explicit AtomInterpretation(const Powerset& interpretation, const Powerset& invariant);
    explicit AtomInterpretation(Powerset&& interpretation, Powerset&& invariant);
    explicit AtomInterpretation(Powerset&& interpretation, const Powerset& invariant);
    explicit AtomInterpretation(const Powerset& interpretation, Powerset&& invariant);

    AtomInterpretation(const AtomInterpretation& interpretation) = default;
    AtomInterpretation& operator= (const AtomInterpretation& interpretation) = delete;

    AtomInterpretation(AtomInterpretation&& interpretation) noexcept;
    AtomInterpretation& operator= (AtomInterpretation&& interpretation) = delete;

    [[nodiscard]] const Powerset& interpretation() const;
    [[nodiscard]] const Powerset& notInterpretation() const;
private:
    Powerset m_interpretation {};
    Powerset m_notInterpretation {};
};

bool operator== (const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2);
bool operator!= (const AtomInterpretation& interpretation1, const AtomInterpretation& interpretation2);
std::ostream& operator<< (std::ostream& out, const AtomInterpretation& interpretation);

#endif //ATOMINTERPRETATION_H
