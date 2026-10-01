#pragma once

#include <stdint.h>
#include "IHardwareIO.h"

class IOService {
public:
    explicit IOService(IHardwareIO &hardware);

    bool begin();
    bool update();

    bool getInput(uint8_t channel) const;

    void setOutput(
        uint8_t channel,
        bool state
    );

    uint8_t getInputs() const;
    uint8_t getOutputs() const;

private:
    IHardwareIO &_hardware;

    uint8_t _inputs = 0;
    uint8_t _outputs = 0;
    bool _initialized = false;
};