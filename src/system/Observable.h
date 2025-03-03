#pragma once

#include <spot/tl/formula.hh>
#include <spot/tl/apcollect.hh>
#include <optional>

#include "ppl_utils.h"
#include "utils.h"
#include "ppl_aliases.h"

using PPL::IO_Operators::operator<<;

class Observable {
public:
    Observable(spot::atomic_prop_set atoms, PowersetSharedPtr interpretation)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::move(interpretation) }
    {
        computeHashCode();
    }

    Observable(spot::atomic_prop_set atoms, const Powerset& interpretation)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::make_shared<Powerset>(interpretation) }
    {
        computeHashCode();
    }

    Observable(spot::atomic_prop_set atoms, PowersetSharedPtr interpretation, std::string interpretationToString)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::move(interpretation) }
        , m_interpretationToString { std::move(interpretationToString) }
    {
        computeHashCode();
    }

    Observable(spot::atomic_prop_set atoms, const Powerset& interpretation, std::string interpretationToString)
        : m_atoms { std::make_shared<spot::atomic_prop_set>(std::move(atoms)) }
        , m_interpretation { std::make_shared<Powerset>(interpretation) }
        , m_interpretationToString { std::move(interpretationToString) }
    {
        computeHashCode();
    }

    Observable() = default;

    [[nodiscard]] const spot::atomic_prop_set& atoms() const { return *m_atoms; }
    [[nodiscard]] PowersetConstSharedPtr interpretation() const { return m_interpretation; }
    [[nodiscard]] bool isUniverse() const { return m_interpretation->is_universe(); }
    [[nodiscard]] bool isEmpty() const { return m_interpretation->is_empty(); }
    [[nodiscard]] int totalAtoms() const { return m_atoms->size(); }
    [[nodiscard]] int totalPatches() const { return m_interpretation->size(); }
    [[nodiscard]] std::size_t hashcode() const { return m_hashcode; }

    friend std::ostream& operator<< (std::ostream& os, const Observable& observable)
    {
        bool first = true;
        os << "Observable: {";
        for (const auto& atom: observable.atoms())
        {
            os << (first ? "" : ", ") << atom;
            first = false;
        }
        os << "}\n";
        os << "Interpretation: " <<
            (observable.m_interpretationToString.has_value()
                ? *observable.m_interpretationToString
                : PPLUtils::toString(*observable.interpretation()));
        return os;
    }

private:
    std::shared_ptr<spot::atomic_prop_set> m_atoms {};
    PowersetConstSharedPtr m_interpretation {};
    std::optional<std::string> m_interpretationToString {};
    std::size_t m_hashcode {};

    void computeHashCode()
    {
        hashCombine(m_hashcode, m_atoms, m_interpretation);
    }
};

inline bool operator== (const Observable& observable1, const Observable& observable2) {
    return observable1.atoms() == observable2.atoms() &&
           *observable1.interpretation() == *observable2.interpretation();
}

inline bool operator!= (const Observable& observable1, const Observable& observable2) {
    return !(observable1 == observable2);
}

template<> struct std::hash<Observable>
{
    std::size_t operator() (const Observable& observable) const noexcept
    {
        return observable.hashcode();
    }
};
