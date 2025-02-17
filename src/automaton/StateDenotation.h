#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/ranges.h>

#include "spot_utils.h"
#include "ppl_output.h"
#include "ppl_aliases.h"

class StateDenotation
{
public:
    [[nodiscard]] PowersetConstSharedPtr denotation() const { return m_denotation; }
    [[nodiscard]] const spot::formula& formula() const { return m_formula; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }
    [[nodiscard]] bool isUniverse() const { return m_denotation->is_top(); }
    [[nodiscard]] bool isEmpty() const { return m_denotation->is_empty(); }
    [[nodiscard]] int totalPatches() const { return m_denotation->size(); }
    [[nodiscard]] const spot::atomic_prop_set& observables() const { return *m_observables; }
    [[nodiscard]] std::string toString(const PolyhedralSystemSymbolTable& symbolTable) const
    {
        std::vector<std::string> toStringObservables {};
        toStringObservables.reserve(m_observables->size());
        for (const spot::formula& observable: *m_observables)
            toStringObservables.emplace_back(observable.ap_name());

        return fmt::format(
            "Formula: {}\n"
            "IsSing: {}\n"
            "Observables: [{}]\n"
            "Denotation: {}",
            SpotUtils::toFormulaString(m_formula),
            m_isSing,
            fmt::join(toStringObservables, ", "),
            PPLOutput::toString(*m_denotation, symbolTable)
        );
    }

    void print(std::ostream& ostream, const PolyhedralSystemSymbolTable& symbolTable) const
    {
        ostream << std::boolalpha;
        ostream << toString(symbolTable);
        ostream << std::noboolalpha;
    }

    StateDenotation(spot::formula&& formula, PowersetConstSharedPtr denotation, const bool isSing = false)
        : m_formula { std::move(formula) }
        , m_denotation { std::move(denotation) }
        , m_isSing { isSing }
    {
        extractObservables();
    }

    StateDenotation(const StateDenotation& other)
        : m_formula { other.m_formula }
        , m_observables { std::make_unique<spot::atomic_prop_set>(*other.m_observables) }
        , m_denotation { std::make_shared<Powerset>(*other.m_denotation) }
        , m_isSing { other.m_isSing }
    {
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_formula { std::move(other.m_formula) }
        , m_observables { std::move(other.m_observables) }
        , m_denotation { std::move(other.m_denotation) }
        , m_isSing { other.m_isSing }
    {
    }

private:
    spot::formula m_formula {};
    std::unique_ptr<spot::atomic_prop_set> m_observables {};
    PowersetConstSharedPtr m_denotation {};
    bool m_isSing {};

    void extractObservables()
    {
        m_observables = std::unique_ptr<spot::atomic_prop_set> { spot::atomic_prop_collect(m_formula) };
        if (m_isSing && !SpotUtils::containsSing(*m_observables))
            m_observables->insert(SpotUtils::sing());
    }
};
