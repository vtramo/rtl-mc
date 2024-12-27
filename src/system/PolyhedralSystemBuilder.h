
//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMBUILDER_H
#define POLYHEDRALSYSTEMBUILDER_H

#include "PolyhedralSystemSymbolTable.h"

class PolyhedralSystem;

class PolyhedralSystemBuilder {
public:
    PolyhedralSystemBuilder& flow(const Poly& flow);
    PolyhedralSystemBuilder& invariant(const Powerset& invariant);
    PolyhedralSystemBuilder& denotation(const std::map<std::string, Powerset>& denotation);
    PolyhedralSystemBuilder& symbolTable(const PolyhedralSystemSymbolTable& polyhedralSystemSymbolTable);
    [[nodiscard]] PolyhedralSystem build() const;

    explicit PolyhedralSystemBuilder(const PolyhedralSystem& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (const PolyhedralSystem& polyhedralSystem) = delete;

    explicit PolyhedralSystemBuilder(PolyhedralSystem&& polyhedralSystem) = delete;
    PolyhedralSystemBuilder& operator= (PolyhedralSystem&& polyhedralSystem) = delete;

    PolyhedralSystemBuilder() = default;
    ~PolyhedralSystemBuilder() = default;

private:
    std::unique_ptr<PolyhedralSystemSymbolTable> m_symbolTable { nullptr };
    std::unique_ptr<std::map<std::string, Powerset>> m_denotation { nullptr };
    std::unique_ptr<Powerset> m_invariant { nullptr };
    std::unique_ptr<Poly> m_flow { nullptr };

    [[nodiscard]] PolyhedralSystem buildPolyhedralSystem() const;

    void assertInvariantIsNotNull() const;
    void assertSymbolTableIsNotNull() const;
    void assertFlowIsNotNull() const;
    void assertDenotationIsNotNull() const;
    void assertThatAllDimensionsAreEqual() const;
};



#endif //POLYHEDRALSYSTEMBUILDER_H
