#pragma once

#include "BackwardNFA.h"
#include "PolyhedralSystem.h"
#include "Denot.h"
#include "RtlMcProgram.h"


std::unique_ptr<Denot> createDenot(
    const RtlMcProgram& rtlMcProgram,
    PolyhedralSystemSharedPtr polyhedralSystem,
    BackwardNFAConstSharedPtr backwardNfa
);

int onlyOnTheFlyDenotInterface(int argc, char *argv[]);