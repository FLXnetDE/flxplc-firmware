#pragma once

#include <stdint.h>

class IHardwareIO {
public:
    virtual ~IHardwareIO() = default;

    virtual bool begin() = 0;

    virtual bool readInputs(uint8_t &states) = 0;

    virtual bool writeOutputs(uint8_t states) = 0;
};