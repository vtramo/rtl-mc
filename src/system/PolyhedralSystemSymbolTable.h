//
// Created by vincenzo on 16/12/24.
//

#ifndef POLYHEDRALSYSTEMSYMBOLTABLE_H
#define POLYHEDRALSYSTEMSYMBOLTABLE_H

#include "ppl_aliases.h"
#include <optional>

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

  friend bool operator== (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);
  friend bool operator!= (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);

private:
  std::unordered_map<std::string, PPL::Variable> m_variableById {};
  std::unordered_map<PPL::dimension_type, std::string> m_idBySpaceDimension {};
  PPL::dimension_type m_dimensions {};
  std::set<std::string> m_atomIds {};
};

#endif //POLYHEDRALSYSTEMSYMBOLTABLE_H
