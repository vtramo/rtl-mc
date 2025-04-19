#pragma once
#include <optional>
#include <unordered_map>
#include "ppl_aliases.h"

class SymbolTable
{
public:
    virtual ~SymbolTable() = default;

    [[nodiscard]] virtual std::unordered_map<PPL::dimension_type, std::string> getVariableNameBySpaceDimension() const = 0;
    [[nodiscard]] virtual std::optional<std::string> getVariableName(const PPL::Variable& variable) const = 0;
    [[nodiscard]] virtual std::optional<PPL::Variable> getVariable(std::string_view id) const = 0;
    [[nodiscard]] virtual bool containsVariable(std::string_view id) const = 0;
    virtual SymbolTable& addVariables(std::initializer_list<std::string_view> ids) = 0;
    virtual SymbolTable& addVariable(std::string_view id) = 0;
    [[nodiscard]] virtual const PPL::dimension_type& spaceDimension() const { return m_spaceDimension; }

protected:
    PPL::dimension_type m_spaceDimension {};
};
