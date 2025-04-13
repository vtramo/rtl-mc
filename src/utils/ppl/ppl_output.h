#pragma once

#include "PolyhedralSystemSymbolTable.h"
#include "ppl_aliases.h"

/*!
 *  \brief Provides utility functions to convert Parma Polyhedra Library (PPL) objects into human-readable string representations.
 *
 * This header file defines a set of functions that take PPL objects (such as polyhedra, constraints, and variables) and convert them
 * into string representations. The functions use a `PolyhedralSystemSymbolTable` to map PPL's internal variable names to user-defined
 * variable names, making the output more intuitive and readable.
 */
namespace PPLOutput
{
    const inline std::string g_defaultAndConstraints { "&" };
    inline std::string g_andConstraints { g_defaultAndConstraints };
    inline bool g_wrapConstraintInRoundBrackets { false };
    const inline std::string g_defaultGeqSign { ">=" };
    inline std::string g_geqSign { g_defaultGeqSign };

    /*!
     * \brief Converts a `Powerset` of polyhedra into a human-readable string representation.
     * \param powerset The `Powerset` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \param minimizeConstraints If `true`, the constraints are minimized before conversion.
     * \return A string representation of the `Powerset`.
     */
    std::string toString(const Powerset& powerset, const PolyhedralSystemSymbolTable& symbolTable, bool minimizeConstraints = true);

    /*!
     * \brief Converts a `Powerset` of polyhedra into a human-readable string representation.
     * \param powerset The `Powerset` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \param minimizeConstraints If `true`, the constraints are minimized before conversion.
     * \return A string representation of the `Powerset`.
     */
    std::string toString(const Powerset& powerset, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable, bool minimizeConstraints = true);

    /*!
     * \brief Converts a `Poly` (polyhedron) into a human-readable string representation.
     * \param poly The `Poly` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \param minimizeConstraints If `true`, the constraints are minimized before conversion.
     * \return A string representation of the `Poly`.
     */
    std::string toString(const Poly& poly, const PolyhedralSystemSymbolTable& symbolTable, bool minimizeConstraints = true);

    /*!
     * \brief Converts a `Poly` (polyhedron) into a human-readable string representation.
     * \param poly The `Poly` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \param minimizeConstraints If `true`, the constraints are minimized before conversion.
     * \return A string representation of the `Poly`.
     */
    std::string toString(const Poly& poly, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable, bool minimizeConstraints = true);

    /*!
     * \brief Converts a `Constraint_System` into a human-readable string representation.
     * \param system The `Constraint_System` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \return A string representation of the `Constraint_System`.
     */
    std::string toString(const PPL::Constraint_System& system, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);

    /*!
     * \brief Converts a single `Constraint` into a human-readable string representation.
     * \param constraint The `Constraint` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \return A string representation of the `Constraint`.
     */
    std::string toString(const PPL::Constraint& constraint, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);

    /*!
     * \brief Converts a `Constraint::Type` into a human-readable string representation.
     * \param constraintType The `Constraint::Type` object to convert.
     * \return A string representation of the `Constraint::Type`.
     */
    std::string toString(const PPL::Constraint::Type& constraintType);

    /*!
     * \brief Converts a `Coefficient` into a human-readable string representation.
     * \param coefficient The `Coefficient` object to convert.
     * \return A string representation of the `Coefficient`.
     */
    std::string toString(const PPL::Coefficient& coefficient);

    /*!
     * \brief Converts a `Variable` into a human-readable string representation.
     * \param variable The `Variable` object to convert.
     * \param symbolTable The symbol table used to map PPL variable names to user-defined names.
     * \return A string representation of the `Variable`.
     */
    std::string toString(const PPL::Variable& variable, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);
}