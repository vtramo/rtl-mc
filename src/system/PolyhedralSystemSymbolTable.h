#ifndef POLYHEDRALSYSTEMSYMBOLTABLE_H
#define POLYHEDRALSYSTEMSYMBOLTABLE_H

#include "ppl_aliases.h"
#include <optional>
#include <spot/tl/apcollect.hh>

class PolyhedralSystemSymbolTable {
public:
  PolyhedralSystemSymbolTable& addVariable(std::string_view id);
  PolyhedralSystemSymbolTable& addVariables(std::initializer_list<std::string_view> ids);
  PolyhedralSystemSymbolTable& addAtom(std::string_view atom);
  PolyhedralSystemSymbolTable& addAtoms(std::initializer_list<std::string_view> atoms);
  [[nodiscard]] bool containsVariable(std::string_view id) const;
  [[nodiscard]] bool containsAtom(std::string_view atom) const;
  [[nodiscard]] const spot::atomic_prop_set& atoms() const;
  [[nodiscard]] std::optional<PPL::Variable> getVariable(std::string_view id) const;
  [[nodiscard]] std::optional<std::string> getVariableName(const PPL::Variable& variable) const;
  [[nodiscard]] std::vector<std::string> getVariableNames() const;
  [[nodiscard]] PPL::dimension_type getSpaceDimension() const;
  [[nodiscard]] int getTotalAtoms() const;

  friend bool operator== (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);
  friend bool operator!= (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2);

private:
  std::unordered_map<std::string, PPL::Variable> m_variableById {};
  std::unordered_map<PPL::dimension_type, std::string> m_idBySpaceDimension {};
  PPL::dimension_type m_dimensions {};
  spot::atomic_prop_set m_atoms {};
};

#endif //POLYHEDRALSYSTEMSYMBOLTABLE_H
