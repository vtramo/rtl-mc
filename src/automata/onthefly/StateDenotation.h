#pragma once

#include <spdlog/spdlog.h>
#include <spot/twa/formula2bdd.hh>

#include "spot_utils.h"
#include "ppl_output.h"
#include "ppl_aliases.h"

class StateDenotation
{
public:
    [[nodiscard]] PowersetConstSharedPtr denotation() const { return m_denotation; }
    [[nodiscard]] const spot::formula& formula() const { return m_formula; }
    [[nodiscard]] const spot::atomic_prop_set& labels() const { return *m_labels; }
    [[nodiscard]] bool isSingular() const { return m_isSing; }
    [[nodiscard]] bool isUniverse() const { return m_denotation->is_top(); }
    [[nodiscard]] bool isEmpty() const { return m_denotation->is_empty(); }
    [[nodiscard]] int totalPatches() const { return m_denotation->size(); }
    [[nodiscard]] std::string toString(const PolyhedralSystemSymbolTable& symbolTable) const
    {
        return fmt::format(
            "Formula: {}\n"
            "IsSing: {}\n"
            "Denotation: {}",
            SpotUtils::toFormulaString(m_formula),
            m_isSing,
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
        m_labels = std::unique_ptr<spot::atomic_prop_set>(spot::atomic_prop_collect(m_formula));

        if (!isSing)
        {
            removeSingFromLabels();
        }

        assert(!isSing || SpotUtils::containsSing(*m_labels));
    }

    StateDenotation(const StateDenotation& other)
        : m_formula { other.m_formula }
        , m_labels { std::make_unique<spot::atomic_prop_set>(*other.m_labels) }
        , m_denotation { std::make_shared<Powerset>(*other.m_denotation) }
        , m_isSing { other.m_isSing }
    {
    }

    StateDenotation(StateDenotation&& other) noexcept
        : m_formula { std::move(other.m_formula) }
        , m_labels { std::move(other.m_labels) }
        , m_denotation { std::move(other.m_denotation) }
        , m_isSing { other.isSingular() }
    {
    }

private:
    spot::formula m_formula {};
    std::unique_ptr<spot::atomic_prop_set> m_labels {};
    bdd m_labelsAsBdd {};
    PowersetConstSharedPtr m_denotation {};
    bool m_isSing {};

    void removeSingFromLabels() const
    {
        for (auto label { m_labels->begin() }; label != m_labels->end(); ++label)
        {
            if (*label == SpotUtils::sing())
            {
                m_labels->erase(label);
                break;
            }
        }
    }
};
