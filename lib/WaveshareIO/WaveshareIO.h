#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <IHardwareIO.h>

class WaveshareIO : public IHardwareIO {
public:
    bool begin() override;

    bool readInputs(uint8_t &states) override;
    bool writeOutputs(uint8_t states) override;

private:
    bool writeRegister(
        uint8_t reg,
        uint8_t value
    );
};