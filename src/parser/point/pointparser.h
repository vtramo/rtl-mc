/*!
* \file pointparser.h
 * \brief Functions for parsing a rational point.
 */

#pragma once

#include <antlr4-runtime.h>
#include <variant>

#include "SymbolTable.h"
#include "ParserError.h"
#include "RationalPoint.h"

/*!
 * \typedef RationalPointParsingResult
 * \brief Result type for point parsing operations.
 *
 * A variant type that represents either:
 * - A successfully parsed point as RationalPoint, or
 * - A list of parsing errors as std::vector<ParserError>
 *
 * Usage example:
 * \code
 * auto result = parseRationalPoint("[x=1/2, y=3/4]", symbolTable);
 * if (auto* point = std::get_if<RationalPoint>(&result)) {
 *     // Use the point...
 * } else if (auto* errors = std::get_if<std::vector<ParserError>>(&result)) {
 *     // Handle errors...
 * }
 * \endcode
 */
using RationalPointParsingResult = std::variant<RationalPoint, std::vector<ParserError>>;

/*!
 * \brief Parses a rational point from a string input.
 * \param input The input string in format "[var1=val1, var2=val2, ...]"
 * \param symbolTable The symbol table containing variable definitions
 * \return RationalPointParsingResult containing either the point or parsing errors
 *
 * The input format requires:
 * - Square brackets enclosing the point definition
 * - Comma-separated variable assignments
 * - Values as integers or fractions (e.g. "3", "-1/2")
 * - All variables must exist in the symbol table
 *
 * Example valid inputs:
 * - "[x=1, y=2]"
 * - "[x=1/2, y=-3/4, z=5]"
 */
RationalPointParsingResult parseRationalPoint(std::string_view input, const SymbolTable& symbolTable);

/*!
 * \overload
 * \see parseRationalPoint(std::string_view input, const SymbolTable& symbolTable)
 */
RationalPointParsingResult parseRationalPoint(std::istream& input, const SymbolTable& symbolTable);

/*!
 * \overload
 * \see parseRationalPoint(std::string_view input, const SymbolTable& symbolTable)
 */
RationalPointParsingResult parseRationalPoint(antlr4::ANTLRInputStream* input, const SymbolTable& symbolTable);

/*!
 * \overload
 * \see parseRationalPoint(std::string_view input, const SymbolTable& symbolTable)
 */
RationalPointParsingResult parseRationalPoint(std::string_view input, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);

/*!
 * \overload
 * \see parseRationalPoint(std::string_view input, const SymbolTable& symbolTable)
 */
RationalPointParsingResult parseRationalPoint(std::istream& input, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);

/*!
 * \overload
 * \see parseRationalPoint(std::string_view input, const SymbolTable& symbolTable)
 */
RationalPointParsingResult parseRationalPoint(antlr4::ANTLRInputStream* input, const std::unordered_map<PPL::dimension_type, std::string>& symbolTable);