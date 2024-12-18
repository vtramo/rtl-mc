//
// Created by vincenzo on 16/12/24.
//

#include "PolyhedralSystemSymbolTable.h"

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addVariable(const std::string_view id)
{
    if (containsVariable(id))
    {
        return *this;
    }

    const PPL::Variable variable { m_dimensions };
    const std::string idStr { id };
    m_variableById.insert(std::make_pair(idStr, variable));
    m_idByVariable.insert(std::make_pair(variable.space_dimension(), idStr));
    m_dimensions++;

    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addVariables(const std::initializer_list<std::string_view> ids)
{
    for (const auto &id: ids)
    {
        addVariable(id);
    }

    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addAtom(const std::string_view id)
{
    m_atomIds.insert(std::string { id });
    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addAtoms(const std::initializer_list<std::string_view> ids)
{
    for (const auto &id: ids)
    {
        addAtom(id);
    }

    return *this;
}

bool PolyhedralSystemSymbolTable::containsVariable(const std::string_view id) const
{
    return m_variableById.count(std::string { id });
}

bool PolyhedralSystemSymbolTable::containsAtom(const std::string_view id) const
{
    return m_atomIds.count(std::string { id });
}

std::optional<PPL::Variable> PolyhedralSystemSymbolTable::getVariable(const std::string_view id) const
{
    if (const auto it { m_variableById.find(std::string { id }) }; it != m_variableById.end()) {
        return it->second;
    }

    return std::nullopt;
}

std::optional<std::string> PolyhedralSystemSymbolTable::getVariableId(const PPL::Variable variable) const
{
    if (const auto it { m_idByVariable.find(variable.space_dimension()) }; it != m_idByVariable.end()) {
        return it->second;
    }

    return std::nullopt;
}

PPL::dimension_type PolyhedralSystemSymbolTable::getSpaceDimension() const
{
    return m_dimensions;
}

int PolyhedralSystemSymbolTable::getTotalAtoms() const
{
    return static_cast<int>(m_atomIds.size());
}

bool PolyhedralSystemSymbolTable::operator==(const PolyhedralSystemSymbolTable& other) const
{
    return m_dimensions == other.m_dimensions &&
           m_atomIds    == other.m_atomIds; // TODO: compare maps
}
