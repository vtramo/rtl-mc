#include "SysGenProgram.h"
#include "tank_experiment.h"

#ifndef RTL_MC_VERSION
#define RTL_MC_VERSION "0.0.0"
#endif

int main(const int argc, char *argv[])
{
    SysGenProgram sysGenProgram { argc, argv, RTL_MC_VERSION };
    unsigned totalTanks { sysGenProgram.totalTanks() };
    PolyhedralSystem polyhedralSystem { gap(totalTanks, sysGenProgram.includeClock(), sysGenProgram.gapThickness()) };

    polyhedralSystem.setConstraintOutputMinimized(false);
    polyhedralSystem.setOutputExtraInformation(false);
    std::cout << polyhedralSystem << std::endl;

    return 0;
}