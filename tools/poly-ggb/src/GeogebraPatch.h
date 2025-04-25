#pragma once

#include "SimpleSymbolTable.h"
#include "ppl_utils.h"
#include "RGBColor.h"
#include "ppl_output.h"

class GeogebraPatch
{
public:
    const static inline std::string s_and { "∧" };
    const static inline std::string s_or { "∨" };
    const static inline std::string s_geq { "≥" };
    const static inline std::string s_leq { "≤" };

    GeogebraPatch(const std::string_view label, const Poly& patch, const RGBColor color)
        : m_label { label }
        , m_patch { patch }
        , m_color { color }
    {
        PPL::Constraint_System constraintSystem {};
        constraintSystem.set_space_dimension(patch.space_dimension());
        m_patchConstraints = extractGeogebraCompatibleConstraints(patch);
    }

    [[nodiscard]] std::string label() const { return m_label; }
    [[nodiscard]] Poly patch() const { return Poly { m_patch }; }
    [[nodiscard]] RGBColor color() const { return m_color; }
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_patch.space_dimension(); }

    [[nodiscard]] std::string toGeogebraExpression(
        SimpleSymbolTable symbolTable,
        std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo> fixedValues,
        const PPL::dimension_type x = 0,
        const PPL::dimension_type y = 1
    ) const
    {
        if (symbolTable.spaceDimension() == 0)
        {
            throw std::invalid_argument("SymbolTable space dimension is zero!");
        }

        if (symbolTable.spaceDimension() != spaceDimension())
        {
            throw std::invalid_argument("Space dimension mismatch!");
        }

        if (x >= spaceDimension())
        {
            throw std::invalid_argument("x is out of space dimension");
        }

        if (spaceDimension() == 1)
        {
            std::vector<std::pair<PPL::Variable, bool>> variables {};
            variables.push_back({ PPL::Variable { 0 }, true });
            return buildFullGeogebraExpression(symbolTable, variables, fixedValues);
        }

        if (y >= spaceDimension())
        {
            throw std::invalid_argument("y is out of space dimension");
        }

        std::vector<std::pair<PPL::Variable, bool>> variables {};
        variables.reserve(spaceDimension());
        for (PPL::dimension_type dim { 0 }; dim < spaceDimension(); ++dim)
        {
            const bool isSliceVariable { dim == x || dim == y };
            variables.push_back({ PPL::Variable { dim }, isSliceVariable });
        }

        return buildFullGeogebraExpression(symbolTable, variables, fixedValues);
    }

    std::string buildFullGeogebraExpression(
        SimpleSymbolTable symbolTable,
        std::vector<std::pair<PPL::Variable, bool>> variables,
        std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo> fixedValues
    ) const
    {
        std::string geogebraExpression { buildPatchGeogebraExpression(symbolTable, variables, fixedValues) };
        if (m_isEmpty) return geogebraExpression;
        std::string geogebraFunctionExpression { buildFunctionSignatureGeogebra(symbolTable, variables) + " = " };
        geogebraFunctionExpression += geogebraExpression;
        return geogebraFunctionExpression;
    }

    static std::string buildUniverseGeogebraExpression(
        SimpleSymbolTable symbolTable,
        const std::vector<std::pair<PPL::Variable, bool>>& variables
    )
    {
        std::string geogebraExpression {};

        bool first { true };
        for (auto [variable, isSliceVariable]: variables)
        {
            if (!isSliceVariable) continue;
            std::string variableName { *symbolTable.getVariableName(variable) };
            geogebraExpression += (first ? "" : (" " + s_and + " ")) + "(" + variableName + " " + s_geq + " 0) " + s_and + " ";
            geogebraExpression += "(" + variableName + " " + s_leq + " 0)";
            first = false;
        }

        return geogebraExpression;
    }

    std::string buildFunctionSignatureGeogebra(
        SimpleSymbolTable symbolTable,
        const std::vector<std::pair<PPL::Variable, bool>>& variables
    ) const
    {
        std::string geogebraExpression {};
        bool first { true };
        geogebraExpression = { label() + "(" };
        for (auto [variable, isSliceVariable]: variables)
        {
            if (!isSliceVariable) continue;
            std::string variableName { symbolTable.getVariableName(variable).value() };
            geogebraExpression += (first ? "" : ", ") + variableName;
            first = false;
        }
        geogebraExpression += ")";
        return geogebraExpression;
    }

    std::string buildPatchGeogebraExpression(
        SimpleSymbolTable symbolTable,
        const std::vector<std::pair<PPL::Variable, bool>>& variables,
        const std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>& fixedValues
    ) const
    {
        if (m_isEmpty) return "{}";
        if (m_isUniverse) return buildUniverseGeogebraExpression(symbolTable, variables);

        auto slicedConstraints { slicePolyhedron(m_patchConstraints, variables, fixedValues) };
        if (slicedConstraints.is_universe())
        {
            return buildUniverseGeogebraExpression(symbolTable, variables);
        }

        std::string polyGeogebraExpression {};
        PPLOutput::g_wrapConstraintInRoundBrackets = true;
        PPLOutput::g_andConstraints = s_and;
        PPLOutput::g_geqSign = s_geq;
        polyGeogebraExpression += PPLOutput::toString(slicedConstraints.minimized_constraints(), symbolTable.getVariableNameBySpaceDimension());
        PPLOutput::g_wrapConstraintInRoundBrackets = false;
        PPLOutput::g_andConstraints = PPLOutput::g_defaultAndConstraints;
        PPLOutput::g_geqSign = PPLOutput::g_defaultGeqSign;
        return polyGeogebraExpression;
    }

private:
    std::string m_label {};
    bool m_isEmpty {};
    bool m_isUniverse {};
    Poly m_patch {};
    PPL::Constraint_System m_patchConstraints {};
    RGBColor m_color { 0, 0, 255 };

    PPL::Constraint_System extractGeogebraCompatibleConstraints(const Poly& patch)
    {
        PPL::Constraint_System constraintSystem {};

        for (const PPL::Constraint& constraint: patch.constraints())
        {
            if (constraint.is_inequality())
            {
                constraintSystem.insert(constraint);
            }
            else
            {
                PPL::Linear_Expression expression {};
                for (PPL::dimension_type dim { 0 }; dim < spaceDimension(); dim++)
                {
                    PPL::Variable variable { dim };
                    auto varCoefficient { constraint.coefficient(variable) };
                    expression += varCoefficient * variable;
                }

                constraintSystem.insert(expression >= constraint.inhomogeneous_term());
                constraintSystem.insert(expression <= constraint.inhomogeneous_term());
            }
        }

        return constraintSystem;
    }

    static Poly slicePolyhedron(
        const PPL::Constraint_System& constraintSystem,
        const std::vector<std::pair<PPL::Variable, bool>>& variables,
        const std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>& fixedValues
    )
    {
        PPL::Constraint_System slicedConstraintSystem {};
        slicedConstraintSystem.set_space_dimension(variables.size() - fixedValues.size());

        for (const auto& constraint: constraintSystem)
        {
            PPL::Linear_Expression expression {};
            expression.set_space_dimension(variables.size() - fixedValues.size());

            for (auto [variable, isSliceVariable]: variables)
            {
                auto varCoefficient {
                    isSliceVariable
                        ? constraint.coefficient(variable)
                        : fixedValues.at(variable)
                };

                if (isSliceVariable)
                {
                    expression += varCoefficient * variable;
                }
                else
                {
                    expression += varCoefficient;
                }
            }

            auto finalConstraint { expression >= -constraint.inhomogeneous_term() };
            if (!finalConstraint.is_inconsistent())
            {
                slicedConstraintSystem.insert(finalConstraint);
            }
        }

        return Poly { slicedConstraintSystem };
    }
};