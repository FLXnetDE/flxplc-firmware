#include "IOService.h"

IOService::IOService(IHardwareIO &hardware)
    : _hardware(hardware) {
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

bool IOService::begin() {
    if (_initialized) {
        return true;
    }

    _inputs = 0;
    _outputs = 0;

    if (!_hardware.begin()) {
        return false;
    }

    _initialized = true;

    // Ensure all logical outputs are inactive.
    if (!resetOutputs()) {
        _initialized = false;
        return false;
    }

    return true;
}

bool IOService::isInitialized() const {
    return _initialized;
}

// --------------------------------------------------
// Digital inputs
// --------------------------------------------------

bool IOService::readInputs() {
    if (!_initialized) {
        return false;
    }

    uint8_t states = 0;

    if (!_hardware.readInputs(states)) {
        return false;
    }

    // Update process image only after a
    // successful hardware read.
    _inputs = states;

    return true;
}

bool IOService::getInput(uint8_t channel) const {
    if (channel >= 8) {
        return false;
    }

    uint8_t mask = (1U << channel);

    return (_inputs & mask) != 0;
}

uint8_t IOService::getInputs() const {
    return _inputs;
}

// --------------------------------------------------
// Digital outputs
// --------------------------------------------------

void IOService::setOutput(
    uint8_t channel,
    bool state
) {
    if (channel >= 8) {
        return;
    }

    uint8_t mask = (1U << channel);

    if (state) {
        _outputs |= mask;
    } else {
        _outputs &= ~mask;
    }
}

void IOService::setOutputs(uint8_t states) {
    _outputs = states;
}

uint8_t IOService::getOutputs() const {
    return _outputs;
}

bool IOService::writeOutputs() {
    if (!_initialized) {
        return false;
    }

    return _hardware.writeOutputs(_outputs);
}

// --------------------------------------------------
// Output reset
// --------------------------------------------------

bool IOService::resetOutputs() {
    _outputs = 0;

    if (!_initialized) {
        return false;
    }

    return writeOutputs();
}