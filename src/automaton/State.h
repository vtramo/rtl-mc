#ifndef STATE_H
#define STATE_H

#include <spot/tl/apcollect.hh>

#include "ppl_aliases.h"

class State {
public:
    [[nodiscard]] int id() const { return m_id; }
    [[nodiscard]] const spot::atomic_prop_set& labels() const { return m_labels; }
    [[nodiscard]] const Powerset& denotation() const { return m_denotation; }
    [[nodiscard]] bool isInitial() const { return m_isInitial; }
    [[nodiscard]] bool isSing() const { return !m_isSing; }
    [[nodiscard]] bool isFinal() const { return m_isFinal; }

private:
    int m_id {};
    Powerset m_denotation { PPL::EMPTY };
    spot::atomic_prop_set m_labels {};
    bool m_isInitial {};
    bool m_isSing {};
    bool m_isFinal {};

    friend class BackwardNFA;
    void setId(const int id) { m_id = id; }
    void setDenotation(Powerset&& denotation) { m_denotation.m_swap(denotation); }
    void setLabels(spot::atomic_prop_set&& labels) { m_labels = std::move(labels); }
    void setIsInitial(const bool isInitial) { m_isInitial = isInitial; }
    void setIsSing(const bool isSing) { m_isSing = isSing; }
    void setIsFinal(const bool isFinal) { m_isFinal = isFinal; }
};

#endif //STATE_H
