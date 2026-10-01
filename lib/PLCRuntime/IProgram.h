#pragma once

#include <IOService.h>

class IProgram {
public:
    virtual ~IProgram() = default;

    virtual void reset() = 0;

    virtual bool execute(IOService &io) = 0;
};