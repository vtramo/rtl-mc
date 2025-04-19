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
        : m_P { P }
        , m_X { X }
        , m_observable { observable }
        , m_state { state }
    {
        m_Pstr = PPLOutput::toString(P, symbolTable);
        m_Xstr = PPLOutput::toString(X, symbolTable);
    }

    int state() const { return m_state; }
    const Poly& patch() const { return m_P; }
    const Poly& reachPoints() const { return m_X; }
    const spot::formula& observable() const { return m_observable; }

    friend std::ostream& operator<< (std::ostream& os, const DenotPathNode& node)
    {
        return os << "State: " << node.m_state << std::endl
                  << "Observable: " << node.m_observable << std::endl
                  << "P: " << node.m_Pstr << std::endl
                  << "X: " << node.m_Xstr << std::endl;
    }

private:
    Poly m_P {};
    Poly m_X {};
    spot::formula m_observable {};
    int m_state {};
    std::string m_Pstr {};
    std::string m_Xstr {};
};

inline std::ostream& operator<< (std::ostream& os, const std::vector<DenotPathNode>& path)
{
    bool first { true };

    if (!path.empty()) os << ">>>>>>>>>" << std::endl;
    for (const auto& node: path)
    {
        os << (first ? "" : "------------------------\n");
        os << node;
        first = false;
    }
    if (!path.empty()) os << "<<<<<<<<<" << std::endl;

    return os;
}