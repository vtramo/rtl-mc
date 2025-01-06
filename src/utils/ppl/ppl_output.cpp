/*! \brief
Declare a local variable named \p id, of type Coefficient, and containing
  an unknown initial value.

  Use of this macro to declare temporaries of type Coefficient results
  in decreased memory allocation overhead and in better locality.
*/
#define PPL_U(x) x

#define PPL_DIRTY_TEMP_COEFFICIENT(id) \
PPL_DIRTY_TEMP(Parma_Polyhedra_Library::Coefficient, id)

#define PPL_DIRTY_TEMP(T, id)                                           \
Parma_Polyhedra_Library::Dirty_Temp<PPL_U(T)> holder_ ## id;          \
PPL_U(T)& PPL_U(id) = holder_ ## id.item()

#include "ppl_output.h"

namespace PPLOutput
{
    std::string toString(const Powerset& powerset, const PolyhedralSystemSymbolTable& symbolTable)
    {
        std::string result{};

        if (powerset.is_bottom())
        {
            result += "false";
        }
        else if (powerset.is_top())
        {
            result += "true";
        }
        else
        {
            result += "( ";
            Powerset::const_iterator powersetIterator { powerset.begin() };
            Powerset::const_iterator end { powerset.end() };
            while (powersetIterator != end)
            {
                result += "{ " + toString(powersetIterator->pointset(), symbolTable) + " }";

                if (++powersetIterator != end)
                    result += ", ";
            }
            result += " )";
        }

        return result;
    }

    std::string toString(const Poly& poly, const PolyhedralSystemSymbolTable& symbolTable)
    {
        std::string result{};

        if (poly.is_empty())
            result += "false";
        else
            result += toString(poly.minimized_constraints(), symbolTable);

        return result;
    }

    std::string toString(const PPL::Constraint_System& system, const PolyhedralSystemSymbolTable& symbolTable)
    {
        std::string result{};
        PPL::Constraint_System_const_iterator constraintIterator { system.begin() };
        const PPL::Constraint_System_const_iterator end { system.end() };

        if (constraintIterator == end)
        {
            result += "true";
        }
        else
        {
            while (constraintIterator != end)
            {
                result += toString(*constraintIterator, symbolTable);

                if (++constraintIterator != end)
                {
                    result += " & ";
                }
            }
        }

        return result;
    }

    std::string toString(const PPL::Constraint& constraint, const PolyhedralSystemSymbolTable& symbolTable)
    {
        std::string result{};

        PPL_DIRTY_TEMP_COEFFICIENT(cv);
        PPL::Constraint::expr_type::const_iterator constraintExprIterator { constraint.expression().begin() };
        PPL::Constraint::expr_type::const_iterator end { constraint.expression().end() };

        bool first { true };
        while (constraintExprIterator != end)
        {
            cv = *constraintExprIterator;

            if (!first)
            {
                if (cv > 0)
                {
                    result += " + ";
                }
                else
                {
                    result += " - ";
                    PPL::neg_assign(cv);
                }
            }
            else
            {
                first = false;
            }


            if (cv == -1)
            {
                result += "-";
            }
            else if (cv != 1)
            {
                result += toString(cv) + "*";
            }

            result += toString(constraintExprIterator.variable(), symbolTable);
            ++constraintExprIterator;
        }

        if (first)
            result += toString(PPL::Coefficient_zero());

        const char* relation_symbol = nullptr;
        switch (constraint.type())
        {
        case PPL::Constraint::EQUALITY:
            relation_symbol = " = ";
            break;
        case PPL::Constraint::NONSTRICT_INEQUALITY:
            relation_symbol = " >= ";
            break;
        case PPL::Constraint::STRICT_INEQUALITY:
            relation_symbol = " > ";
            break;
        }

        result += relation_symbol + toString(-constraint.inhomogeneous_term());

        return result;
    }

    std::string toString(const PPL::Coefficient& coefficient)
    {
        return coefficient.get_str();
    }

    std::string toString(const PPL::Constraint::Type& constraintType)
    {
        const char* type = nullptr;

        switch (constraintType)
        {
        case PPL::Constraint::EQUALITY:
            type = "EQUALITY";
            break;
        case PPL::Constraint::NONSTRICT_INEQUALITY:
            type = "NONSTRICT_INEQUALITY";
            break;
        case PPL::Constraint::STRICT_INEQUALITY:
            type = "STRICT_INEQUALITY";
            break;
        }

        return type;
    }

    std::string toString(const PPL::Variable& variable, const PolyhedralSystemSymbolTable& symbolTable)
    {
        std::string result {};

        static constexpr char varNameLetters[] { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }; // :(
        constexpr PPL::dimension_type numLetters { sizeof(varNameLetters) - 1 };

        if (const auto variableName { symbolTable.getVariableName(variable) })
        {
            result += *variableName;
        }
        else
        {
            result += varNameLetters[variable.id() % numLetters];
            if (const PPL::dimension_type variableId = variable.id() / numLetters)
            {
                result += variableId;
            }
        }

        return result;
    }
}
