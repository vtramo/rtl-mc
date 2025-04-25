#include "PolyhedralSystemSymbolTable.h"
#include "spot_utils.h"
#include "ppl_utils.h"
#include "formula.h"

namespace
{
    bool areVariableByIdMapsEqual(
        const std::unordered_map<std::string, PPL::Variable>& map1,
        const std::unordered_map<std::string, PPL::Variable>& map2
    );
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addVariable(const std::string_view id)
{
    if (containsVariable(id))
    {
        return *this;
    }

    const PPL::Variable variable { m_dimensions };
    const std::string idStr { id };
    m_variableById.insert(std::make_pair(idStr, variable));
    m_idBySpaceDimension.insert({ variable.space_dimension(), idStr });
    m_dimensions++;

    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addVariables(const std::initializer_list<std::string_view> ids)
{
    for (const auto &id: ids)
        addVariable(id);

    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addAtom(const std::string_view atom)
{
    return addAtom(ap(atom));
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addAtom(const spot::formula& atom)
{
    m_atoms.insert(atom);
    return *this;
}

PolyhedralSystemSymbolTable& PolyhedralSystemSymbolTable::addAtoms(const std::initializer_list<std::string_view> atoms)
{
    for (const auto &atom: atoms)
        addAtom(atom);

    return *this;
}

bool PolyhedralSystemSymbolTable::containsVariable(const std::string_view id) const
{
    return m_variableById.count(std::string { id });
}

bool PolyhedralSystemSymbolTable::containsAtom(const std::string_view atom) const
{
    return m_atoms.count(ap(atom));
}

const spot::atomic_prop_set& PolyhedralSystemSymbolTable::atoms() const
{
    return m_atoms;
}

std::optional<PPL::Variable> PolyhedralSystemSymbolTable::getVariable(const std::string_view id) const
{
    if (const auto it { m_variableById.find(std::string { id }) }; it != m_variableById.end()) {
        return it->second;
    }

    return std::nullopt;
}

std::optional<std::string> PolyhedralSystemSymbolTable::getVariableName(const PPL::Variable& variable) const
{
    if (const auto it { m_idBySpaceDimension.find(variable.space_dimension()) }; it != m_idBySpaceDimension.end()) {
        return it->second;
    }

    return std::nullopt;
}

const PPL::dimension_type& PolyhedralSystemSymbolTable::spaceDimension() const
{
    return m_dimensions;
}

int PolyhedralSystemSymbolTable::getTotalAtoms() const
{
    return static_cast<int>(m_atoms.size());
}

bool operator== (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2)
{
    return symbolTable1.m_dimensions == symbolTable2.m_dimensions &&
           symbolTable1.m_atoms    == symbolTable2.m_atoms &&
           areVariableByIdMapsEqual(symbolTable1.m_variableById, symbolTable2.m_variableById) &&
           symbolTable1.m_idBySpaceDimension == symbolTable2.m_idBySpaceDimension;
}

bool operator!= (const PolyhedralSystemSymbolTable& symbolTable1, const PolyhedralSystemSymbolTable& symbolTable2)
{
    return !(symbolTable1 == symbolTable2);
}

namespace
{
    bool areVariableByIdMapsEqual(
        const std::unordered_map<std::string, PPL::Variable>& map1,
        const std::unordered_map<std::string, PPL::Variable>& map2
    )
    {
        if (map1.size() != map2.size()) return false;

        return std::all_of(map1.begin(), map1.end(), [&map2](const auto& pair) {
            const auto& varId = pair.first;
            const auto& var1 = pair.second;
            auto it = map2.find(varId);
            return it != map2.end() && PPLUtils::haveSameSpaceDimension(var1, it->second);
        });
    }
}

std::vector<std::string> PolyhedralSystemSymbolTable::getVariableNames() const
{
    std::vector<std::string> variableNames {};
    variableNames.reserve(m_variableById.size());
    for (const auto& [variableName, _]: m_variableById)
        variableNames.push_back(variableName);
    return variableNames;
}
