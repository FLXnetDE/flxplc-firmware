#include "IOService.h"

IOService::IOService(IHardwareIO &hardware)
    : _hardware(hardware) {
}

bool IOService::begin() {
    _initialized = _hardware.begin();
    return _initialized;
}

bool IOService::update() {
    if (!_initialized) {
        return false;
    }

    uint8_t inputs;

    if (!_hardware.readInputs(inputs)) {
        return false;
    }

    _inputs = inputs;

    return _hardware.writeOutputs(_outputs);
}

bool IOService::getInput(uint8_t channel) const {
    if (channel >= 8) {
        return false;
    }

    return (_inputs & (1U << channel)) != 0;
}

void IOService::setOutput(
    uint8_t channel,
    bool state
) {
    if (channel >= 8) {
        return;
    }

    if (state) {
        _outputs |= (1U << channel);
    } else {
        _outputs &= ~(1U << channel);
    }
}

uint8_t IOService::getInputs() const {
    return _inputs;
}

uint8_t IOService::getOutputs() const {
    return _outputs;
}