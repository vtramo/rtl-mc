#ifndef STATE_H
#define STATE_H

#include "ppl_aliases.h"
#include "AtomSet.h"

class State {
public:
    [[nodiscard]] int index() const { return m_index; }
    [[nodiscard]] const AtomSet& labels() const { return m_labels; }
    [[nodiscard]] const Powerset& denotation() const { return m_denotation; }
    [[nodiscard]] bool isInitial() const { return m_isInitial; }
    [[nodiscard]] bool isSing() const { return m_isSing; }
    [[nodiscard]] bool isFinal() const { return m_isFinal; }

private:
    int m_index {};
    Powerset m_denotation { PPL::EMPTY };
    AtomSet m_labels {};
    bool m_isInitial {};
    bool m_isSing {};
    bool m_isFinal {};

    friend class BackwardNFA;
    void setIndex(const int index) { m_index = index; }
    void setDenotation(Powerset&& denotation) { m_denotation.m_swap(denotation); }
    void setLabels(AtomSet&& labels) { m_labels = std::move(labels); }
    void setIsInitial(const bool isInitial) { m_isInitial = isInitial; }
    void setIsSing(const bool isSing) { m_isSing = isSing; }
    void setIsFinal(const bool isFinal) { m_isFinal = isFinal; }
};

inline std::ostream& operator<< (std::ostream& out, const State& state)
{
    out << std::boolalpha;
    out << "State: " << state.index() << '\n';
    out << "Initial: " << state.isInitial() << '\n';
    out << "Sing: " << state.isSing() << '\n';
    out << "Final: " << state.isFinal() << '\n';
    out << "Labels: " << state.labels() << '\n';
    return out << std::noboolalpha;
}

#endif //STATE_H
