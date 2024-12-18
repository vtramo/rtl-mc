//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMSYMBOLTABLE_H
#define POLYHEDRALSYSTEMSYMBOLTABLE_H

#include "ppl.hh"
#include <optional>

namespace PPL = Parma_Polyhedra_Library;

class PolyhedralSystemSymbolTable {
public:
  PolyhedralSystemSymbolTable& addVariable(std::string_view id);
  PolyhedralSystemSymbolTable& addVariables(std::initializer_list<std::string_view> ids);
  PolyhedralSystemSymbolTable& addAtom(std::string_view id);
  PolyhedralSystemSymbolTable& addAtoms(std::initializer_list<std::string_view> ids);
  [[nodiscard]] bool containsVariable(std::string_view id) const;
  [[nodiscard]] bool containsAtom(std::string_view id) const;
  [[nodiscard]] std::optional<PPL::Variable> getVariable(std::string_view id) const;
  [[nodiscard]] std::optional<std::string> getVariableId(PPL::Variable variable) const;
  [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
  [[nodiscard]] int getTotalAtoms() const;

  bool operator==(const PolyhedralSystemSymbolTable& other) const;

private:
  std::map<std::string, PPL::Variable> m_variableById {};
  std::map<PPL::dimension_type, std::string> m_idByVariable {};
  PPL::dimension_type m_dimensions {};
  std::set<std::string> m_atomIds {};
};

#endif //POLYHEDRALSYSTEMSYMBOLTABLE_H
