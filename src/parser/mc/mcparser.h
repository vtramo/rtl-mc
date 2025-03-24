/*!
* \file mcparser.h
 * \brief Functions for parsing a model-checking point (rational value).
 */

#pragma once

#include <antlr4-runtime.h>
#include <variant>

#include "PolyhedralSystemSymbolTable.h"
#include "ParserError.h"


/*!
 * \typedef McPointParsingResult
 * \brief Result type for point parsing operations.
 *
 * A variant type that represents either:
 * - A successfully parsed point as PPL::Generator, or
 * - A list of parsing errors as std::vector<ParserError>
 *
 * Usage example:
 * \code
 * auto result = parseMcPoint("[x=1/2, y=3/4]", symbolTable);
 * if (auto* point = std::get_if<PPL::Generator>(&result)) {
 *     // Use the point...
 * } else if (auto* errors = std::get_if<std::vector<ParserError>>(&result)) {
 *     // Handle errors...
 * }
 * \endcode
 */
using McPointParsingResult = std::variant<PPL::Generator, std::vector<ParserError>>;

/*!
 * \brief Parses a rational point from a string input.
 * \param input The input string in format "[var1=val1, var2=val2, ...]"
 * \param symbolTable The symbol table containing variable definitions
 * \return McPointParsingResult containing either the point or parsing errors
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
McPointParsingResult parseMcPoint(std::string_view input, const PolyhedralSystemSymbolTable& symbolTable);

/*!
 * \overload
 * \see parseMcPoint(std::string_view input, const PolyhedralSystemSymbolTable& symbolTable)
 */
McPointParsingResult parseMcPoint(std::istream& input, const PolyhedralSystemSymbolTable& symbolTable);

/*!
 * \overload
 * \see parseMcPoint(std::string_view input, const PolyhedralSystemSymbolTable& symbolTable)
 */
McPointParsingResult parseMcPoint(antlr4::ANTLRInputStream* input, const PolyhedralSystemSymbolTable& symbolTable);