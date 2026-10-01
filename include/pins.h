#pragma once

#include <Arduino.h>

namespace HardwarePins {
    constexpr uint8_t DIGITAL_INPUTS[8] = {
        4, 5, 6, 7, 8, 9, 10, 11
    };

    constexpr uint8_t I2C_SDA = 42;
    constexpr uint8_t I2C_SCL = 41;

    constexpr uint8_t IO_EXPANDER_ADDRESS = 0x20;
}