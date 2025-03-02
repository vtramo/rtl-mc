#pragma once

#include <AtomInterpretation.h>

#include "PolyhedralSystemSymbolTable.h"

class PolyhedralSystem;

using Atom = std::string;

class PolyhedralSystemBuilder {
public:
    PolyhedralSystemBuilder& flow(const Poly& flow);
    PolyhedralSystemBuilder& flow(Poly&& flow);

    PolyhedralSystemBuilder& invariant(const Powerset& invariant);
    PolyhedralSystemBuilder& invariant(Powerset&& invariant);

    PolyhedralSystemBuilder& denotation(const std::unordered_map<std::string, Powerset>& denotation);
    PolyhedralSystemBuilder& denotation(std::unordered_map<std::string, Powerset>&& denotation);

    PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable);
    PolyhedralSystemBuilder& symbolTable(PolyhedralSystemSymbolTable&& polyhedralSystemSymbolTable);

    [[nodiscard]] PolyhedralSystem build() const;

    explicit PolyhedralSystemBuilder(const PolyhedralSystem& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (const PolyhedralSystem& polyhedralSystem) = delete;

    explicit PolyhedralSystemBuilder(PolyhedralSystem&& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (PolyhedralSystem&& polyhedralSystem) = delete;

    PolyhedralSystemBuilder() = default;
    ~PolyhedralSystemBuilder() = default;

private:
    std::unique_ptr<PolyhedralSystemSymbolTable> m_symbolTable { nullptr };
    std::unique_ptr<std::unordered_map<Atom, Powerset>> m_denotation { nullptr };
    std::unique_ptr<Powerset> m_invariant { nullptr };
    std::unique_ptr<Poly> m_flow { nullptr };

    [[nodiscard]] std::unordered_map<Atom, AtomInterpretation> buildDenotation() const;
    [[nodiscard]] PolyhedralSystem buildPolyhedralSystem() const;

    void assertInvariantIsNotNull() const;
    void assertSymbolTableIsNotNull() const;
    void assertFlowIsNotNull() const;
    void assertDenotationIsNotNull() const;
    void assertThatAllDimensionsAreEqual() const;
};