#pragma once

#include <spot/tl/formula.hh>

#include "ppl_aliases.h"
#include "ppl_output.h"
#include "PolyhedralSystemSymbolTable.h"

class DenotPathNode
{
public:
    DenotPathNode(
        const int state,
        const Poly& P,
        const Poly& X,
        const spot::formula& observable,
        const PolyhedralSystemSymbolTable& symbolTable
    )
        : m_P{P}
          , m_X{X}
          , m_observable{observable}
          , m_state{state}
    {
        m_Pstr = PPLOutput::toString(P, symbolTable);
        m_Xstr = PPLOutput::toString(X, symbolTable);
    }

    [[nodiscard]] int state() const { return m_state; }
    [[nodiscard]] const Poly& patch() const { return m_P; }
    [[nodiscard]] const Poly& reachPoints() const { return m_X; }
    [[nodiscard]] const spot::formula& observable() const { return m_observable; }
    [[nodiscard]] const std::string_view patchString() const { return m_Pstr; }
    [[nodiscard]] const std::string_view reachPointsString() const { return m_Xstr; }

    friend std::ostream& operator<<(std::ostream& os, const DenotPathNode& node)
    {
        return os << "State: " << node.m_state << std::endl
            << "Observable: " << node.m_observable << std::endl
            << "P: " << node.m_Pstr << std::endl
            << "X: " << node.m_Xstr << std::endl;
    }

    bool operator==(const DenotPathNode& other) const
    {
        return m_state == other.m_state &&
            m_P == other.m_P &&
            m_X == other.m_X &&
            m_observable == other.m_observable;
    }

    bool operator!=(const DenotPathNode& other) const
    {
        return !(*this == other);
    }

private:
    Poly m_P{};
    Poly m_X{};
    spot::formula m_observable{};
    int m_state{};
    std::string m_Pstr{};
    std::string m_Xstr{};
};

template <>
struct std::hash<DenotPathNode>
{
    size_t operator() (const DenotPathNode& node) const noexcept
    {
        size_t stateHash = hash<int>{}(node.state());
        size_t pHash = hash<string>{}(std::string { node.patchString() });
        size_t xHash = hash<string>{}(std::string { node.reachPointsString() });
        size_t obsHash = node.observable().id();

        return ((stateHash ^ (pHash << 1)) >> 1) ^
            ((xHash ^ (obsHash << 1)) >> 1);
    }
};

inline std::ostream& operator<<(std::ostream& os, const std::vector<DenotPathNode>& path)
{
    bool first{true};

    if (!path.empty()) os << ">>>>>>>>>" << std::endl;
    for (const auto& node : path)
    {
        os << (first ? "" : "------------------------\n");
        os << node;
        first = false;
    }
    if (!path.empty()) os << "<<<<<<<<<" << std::endl;

    return os;
}
