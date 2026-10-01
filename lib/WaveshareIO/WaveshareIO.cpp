#include "WaveshareIO.h"
#include <pins.h>

namespace {
    constexpr uint8_t REG_OUTPUT = 0x01;
    constexpr uint8_t REG_CONFIGURATION = 0x03;
}

bool WaveshareIO::begin() {
    // Configure digital inputs.
    for (uint8_t pin : HardwarePins::DIGITAL_INPUTS) {
        pinMode(pin, INPUT_PULLUP);
    }

    // Initialize I2C.
    Wire.begin(
        HardwarePins::I2C_SDA,
        HardwarePins::I2C_SCL
    );

    Wire.setClock(100000);

    // Prepare all outputs as OFF.
    if (!writeRegister(REG_OUTPUT, 0x00)) {
        return false;
    }

    // Configure all expander pins as outputs.
    return writeRegister(REG_CONFIGURATION, 0x00);
}

bool WaveshareIO::readInputs(uint8_t &states) {
    uint8_t result = 0;

    for (uint8_t i = 0; i < 8; i++) {
        if (digitalRead(
            HardwarePins::DIGITAL_INPUTS[i]
        ) == LOW) {
            result |= (1U << i);
        }
    }

    states = result;
    return true;
}

bool WaveshareIO::writeOutputs(uint8_t states) {
    return writeRegister(REG_OUTPUT, ~states);
}

bool WaveshareIO::writeRegister(
    uint8_t reg,
    uint8_t value
) {
    Wire.beginTransmission(
        HardwarePins::IO_EXPANDER_ADDRESS
    );

    Wire.write(reg);
    Wire.write(value);

    return Wire.endTransmission() == 0;
}