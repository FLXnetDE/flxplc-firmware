#pragma once

#include <Arduino.h>
#include <atomic>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <IOService.h>

#include "IProgram.h"
#include "RuntimeTypes.h"

class PLCRuntime {
public:
    explicit PLCRuntime(
        IOService &io,
        IProgram &program,
        uint32_t cycleTimeMs = 20
    );

    bool begin();

    bool start();
    bool stop();

    RuntimeState getState() const;
    RuntimeTelemetry getTelemetry() const;

private:
    static void taskEntry(void *parameter);

    void taskLoop();
    void processCommands();
    bool executeCycle();
    void enterError();

    IOService &_io;
    IProgram &_program;

    uint32_t _cycleTimeMs;

    TaskHandle_t _taskHandle = nullptr;
    QueueHandle_t _commandQueue = nullptr;

    std::atomic<RuntimeState> _state{
        RuntimeState::STOPPED
    };

    std::atomic<uint32_t> _cycleCount{0};
    std::atomic<uint32_t> _lastCycleUs{0};
    std::atomic<uint32_t> _maxCycleUs{0};
    std::atomic<uint32_t> _overrunCount{0};

    std::atomic<uint8_t> _inputs{0};
    std::atomic<uint8_t> _outputs{0};

    bool _initialized = false;
};