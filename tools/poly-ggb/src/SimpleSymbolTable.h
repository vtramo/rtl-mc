#pragma once

#include "ppl_aliases.h"
#include <optional>

#include "SymbolTable.h"

class SimpleSymbolTable: public SymbolTable
{
public:

    [[nodiscard]] const PPL::dimension_type& spaceDimension() const override { return m_spaceDimension; }

    /*!
     * \brief Adds a new variable with a user-defined name to the symbol table.
     * \param id The user-defined name of the variable.
     * \return A reference to the current symbol table for method chaining.
     */
    SimpleSymbolTable& addVariable(const std::string_view id) override
    {
        if (containsVariable(id))
        {
            return *this;
        }

        const PPL::Variable variable { m_spaceDimension };
        const std::string idStr { id };
        m_variableByVariableName.insert({ idStr, variable });
        m_variableNameBySpaceDimension.insert({ variable.space_dimension(), idStr });
        m_spaceDimension++;

        return *this;
    }

    /*!
     * \brief Adds multiple variables with user-defined names to the symbol table.
     * \param ids A list of user-defined variable names.
     * \return A reference to the current symbol table for method chaining.
     */
    SimpleSymbolTable& addVariables(const std::initializer_list<std::string_view> ids) override
    {
        for (const auto &id: ids)
        {
            addVariable(id);
        }

        return *this;
    }

    [[nodiscard]] bool containsVariable(const std::string_view id) const override
    {
        return m_variableByVariableName.count(std::string { id });
    }

    /*!
     * \brief Retrieves the PPL variable corresponding to a user-defined variable name.
     * \param id The user-defined name of the variable.
     * \return An optional containing the PPL variable if the name exists, or \c std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<PPL::Variable> getVariable(const std::string_view id) const override
    {
        if (const auto it { m_variableByVariableName.find(std::string { id }) }; it != m_variableByVariableName.end())
        {
            return it->second;
        }

        return std::nullopt;

    }

    /*!
     * \brief Retrieves the user-defined name corresponding to a PPL variable.
     * \param variable The PPL variable.
     * \return An optional containing the user-defined name if the variable exists, or \c std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::string> getVariableName(const PPL::Variable& variable) const override
    {
        if (const auto it { m_variableNameBySpaceDimension.find(variable.space_dimension()) }; it != m_variableNameBySpaceDimension.end())
        {
            return it->second;
        }

        return std::nullopt;

    }

    /*!
     * \brief Returns the mapping between space dimensions and user-defined variable names.
     * \return An unordered map that associates each space dimension with a corresponding variable name.
     */
    [[nodiscard]] std::unordered_map<PPL::dimension_type, std::string> getVariableNameBySpaceDimension() const override
    {
        return m_variableNameBySpaceDimension;
    }

private:
    PPL::dimension_type m_spaceDimension {};
    std::unordered_map<PPL::dimension_type, std::string> m_variableNameBySpaceDimension {};
    std::unordered_map<std::string, PPL::Variable> m_variableByVariableName {};
};
