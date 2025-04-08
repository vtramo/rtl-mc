#include "SysGenProgram.h"
#include "tank_experiment.h"

#ifndef RTL_MC_VERSION
#define RTL_MC_VERSION "0.0.0"
#endif


int main(const int argc, char *argv[])
{
    PolyhedralSystem polyhedralSystem { gap(2) };
    std::cout << polyhedralSystem << std::endl;
    return 0;
}