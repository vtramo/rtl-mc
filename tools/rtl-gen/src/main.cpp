#include "RtlGenProgram.h"
#include "FormulaFamilyVisitor.h"

#ifndef RTL_MC_VERSION
#define RTL_MC_VERSION "0.0.0"
#endif

int main(const int argc, char *argv[])
{
    RtlGenProgram rtlGenProgram { argc, argv, RTL_MC_VERSION };
    const spot::formula& formula { std::visit(FormulaFamilyVisitor{}, rtlGenProgram.familyParameters()) };
    std::cout << formula << std::endl;
    return 0;
}