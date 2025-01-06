#ifndef PPL_OUTPUT_H
#define PPL_OUTPUT_H

#include "PolyhedralSystemSymbolTable.h"
#include "ppl_aliases.h"

namespace PPLOutput
{
    std::string toString(const Powerset& powerset, const PolyhedralSystemSymbolTable& symbolTable, bool minimizeConstraints = true);
    std::string toString(const Poly& poly, const PolyhedralSystemSymbolTable& symbolTable, bool minimizeConstraints = true);
    std::string toString(const PPL::Constraint_System& system, const PolyhedralSystemSymbolTable& symbolTable);
    std::string toString(const PPL::Constraint& constraint, const PolyhedralSystemSymbolTable& symbolTable);
    std::string toString(const PPL::Constraint::Type& constraintType);
    std::string toString(const PPL::Coefficient& coefficient);
    std::string toString(const PPL::Variable& variable, const PolyhedralSystemSymbolTable& symbolTable);
}

#endif //PPL_OUTPUT_H
