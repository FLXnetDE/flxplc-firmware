#pragma once

#include "IProgram.h"

class TestProgram : public IProgram {
public:
    void reset() override {
        // No internal state to reset.
    }

    bool execute(IOService &io) override {
        // Read digital input DI1.
        bool input1 = io.getInput(0);

        // Reset all outputs.
        io.setOutputs(0);

        // DI1 controls DO1.
        io.setOutput(0, input1);

        return true;
    }
};