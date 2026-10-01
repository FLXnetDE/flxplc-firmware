#pragma once

#include <stdint.h>

enum class RuntimeState : uint8_t {
    STOPPED,
    RUNNING,
    ERROR
};

enum class RuntimeCommand : uint8_t {
    START,
    STOP
};

struct RuntimeTelemetry {
    RuntimeState state = RuntimeState::STOPPED;

    uint32_t cycleCount = 0;
    uint32_t lastCycleUs = 0;
    uint32_t maxCycleUs = 0;
    uint32_t overrunCount = 0;

    uint8_t inputs = 0;
    uint8_t outputs = 0;
};