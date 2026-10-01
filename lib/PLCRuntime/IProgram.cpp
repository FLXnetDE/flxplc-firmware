#pragma once

#include "IProgram.h"

class TestProgram : public IProgram {
public:
    void reset() override {
    }

    bool execute(IOService &io) override {
        // DI1 controls DO1.
        io.setOutput(0, io.getInput(0));

        return true;
    }
};