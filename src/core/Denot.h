#pragma once

#include "ppl_aliases.h"

class Denot
{
public:
    virtual ~Denot() = default;

    PowersetUniquePtr operator() () { return run(); }
    virtual PowersetUniquePtr run() { return PowersetUniquePtr(); }
    virtual int totalIterations() const { return 0; }
    virtual int totalAcceptingTraces() const { return 0; }
};