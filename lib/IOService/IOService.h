#pragma once

#include <stdint.h>
#include "IHardwareIO.h"

class IOService {
public:
    explicit IOService(IHardwareIO &hardware);

    // Initialization
    bool begin();
    bool isInitialized() const;

    // Cyclic I/O operations
    bool readInputs();
    bool writeOutputs();

    // Reset all outputs to their inactive state
    bool resetOutputs();

    // Individual digital inputs and outputs
    bool getInput(uint8_t channel) const;
    void setOutput(uint8_t channel, bool state);

    // Complete digital process images
    uint8_t getInputs() const;
    uint8_t getOutputs() const;

    // Set complete output image
    void setOutputs(uint8_t states);

private:
    IHardwareIO &_hardware;

    // Digital process images
    uint8_t _inputs = 0;
    uint8_t _outputs = 0;

    bool _initialized = false;
};