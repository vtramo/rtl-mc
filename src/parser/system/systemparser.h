/*!
* \file PolyhedralSystemParser.h
 * \brief Functions for parsing polyhedral systems from various input sources.
 */

#pragma once

#include "PolyhedralSystemParsingResult.h"
#include <antlr4-runtime.h>

/*!
 * \brief Parses a polyhedral system from a string input.
 * \param input The input string containing the polyhedral system specification.
 * \return PolyhedralSystemParsingResult containing either the parsed system or error information.
 *
 * This function parses a polyhedral system specification from a string view.
 * The input should follow the grammar rules for polyhedral systems.
 * The result object can be checked for success and will contain either:
 * - The successfully parsed PolyhedralSystem (accessible via operator*)
 * - Detailed error information if parsing failed
 *
 * Example usage:
 * \code
 * auto result =
 *     parsePolyhedralSystem(
 *       "Inv ( { a >= 0 & b >= 0 } )"
 *       "Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }"
 *       "p { a >= b + 1 }"
 *       "q { b >= a + 1 }"
 *       "t0 { t = 0 }"
 *       "t1 { t <= 10 }"
 *     );
 *
 * if (result)
 * {
 *     PolyhedralSystem system = *result;
 *     // Use the system...
 * }
 * else
 * {
 *     std::cerr << "Error while parsing Polyhedral System" << std::endl;
 *     std::cerr << result;
 * }
 * \endcode
 */
PolyhedralSystemParsingResult parsePolyhedralSystem(std::string_view input);

/*!
 * \overload
 * \see parsePolyhedralSystem(std::string_view input)
 */
PolyhedralSystemParsingResult parsePolyhedralSystem(std::istream& input);


/*!
 * \overload
 * \see parsePolyhedralSystem(std::string_view input)
 */
PolyhedralSystemParsingResult parsePolyhedralSystem(antlr4::ANTLRInputStream* input);