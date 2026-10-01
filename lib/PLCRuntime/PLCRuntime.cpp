#include "PLCRuntime.h"

PLCRuntime::PLCRuntime(
    IOService &io,
    IProgram &program,
    uint32_t cycleTimeMs
)
    : _io(io),
      _program(program),
      _cycleTimeMs(cycleTimeMs) {
}

bool PLCRuntime::begin() {
    if (_initialized) {
        return true;
    }

    if (!_io.isInitialized()) {
        return false;
    }

    if (_cycleTimeMs == 0) {
        return false;
    }

    if (pdMS_TO_TICKS(_cycleTimeMs) == 0) {
        return false;
    }

    _commandQueue = xQueueCreate(
        8,
        sizeof(RuntimeCommand)
    );

    if (_commandQueue == nullptr) {
        return false;
    }

    _program.reset();

    if (!_io.resetOutputs()) {
        vQueueDelete(_commandQueue);
        _commandQueue = nullptr;
        return false;
    }

    _initialized = true;

    BaseType_t result = xTaskCreate(
        taskEntry,
        "PLC Runtime",
        8192,
        this,
        3,
        &_taskHandle
    );

    if (result != pdPASS) {
        _initialized = false;

        vQueueDelete(_commandQueue);
        _commandQueue = nullptr;

        return false;
    }

    return true;
}

bool PLCRuntime::start() {
    if (!_initialized) {
        return false;
    }

    if (_state.load() == RuntimeState::ERROR) {
        return false;
    }

    RuntimeCommand command = RuntimeCommand::START;

    return xQueueSend(
        _commandQueue,
        &command,
        0
    ) == pdTRUE;
}

bool PLCRuntime::stop() {
    if (!_initialized) {
        return false;
    }

    RuntimeCommand command = RuntimeCommand::STOP;

    return xQueueSendToFront(
        _commandQueue,
        &command,
        0
    ) == pdTRUE;
}

void PLCRuntime::taskEntry(void *parameter) {
    auto *runtime = static_cast<PLCRuntime *>(
        parameter
    );

    runtime->taskLoop();

    vTaskDelete(nullptr);
}

void PLCRuntime::processCommands() {
    RuntimeCommand command;

    while (
        xQueueReceive(
            _commandQueue,
            &command,
            0
        ) == pdTRUE
    ) {
        switch (command) {
            case RuntimeCommand::START:
                if (
                    _state.load() ==
                    RuntimeState::STOPPED
                ) {
                    _program.reset();

                    _state.store(
                        RuntimeState::RUNNING
                    );
                }
                break;

            case RuntimeCommand::STOP:
                if (!_io.resetOutputs()) {
                    enterError();
                    return;
                }

                _outputs.store(0);

                if (
                    _state.load() !=
                    RuntimeState::ERROR
                ) {
                    _state.store(
                        RuntimeState::STOPPED
                    );
                }
                break;
        }
    }
}

bool PLCRuntime::executeCycle() {
    if (!_io.readInputs()) {
        return false;
    }

    _inputs.store(_io.getInputs());

    if (!_program.execute(_io)) {
        return false;
    }

    if (!_io.writeOutputs()) {
        return false;
    }

    _outputs.store(_io.getOutputs());

    return true;
}

void PLCRuntime::taskLoop() {
    const TickType_t cycleTicks =
        pdMS_TO_TICKS(_cycleTimeMs);

    TickType_t lastWakeTime = xTaskGetTickCount();

    RuntimeState previousState =
        RuntimeState::STOPPED;

    while (true) {
        processCommands();

        RuntimeState state = _state.load();

        if (state != RuntimeState::RUNNING) {
            previousState = state;

            vTaskDelay(pdMS_TO_TICKS(10));

            lastWakeTime = xTaskGetTickCount();
            continue;
        }

        if (previousState != RuntimeState::RUNNING) {
            lastWakeTime = xTaskGetTickCount();
        }

        previousState = RuntimeState::RUNNING;

        uint32_t startUs = micros();

        if (!executeCycle()) {
            enterError();
            continue;
        }

        uint32_t durationUs =
            static_cast<uint32_t>(
                micros() - startUs
            );

        _lastCycleUs.store(durationUs);
        _cycleCount.fetch_add(1);

        uint32_t previousMax = _maxCycleUs.load();

        while (
            durationUs > previousMax &&
            !_maxCycleUs.compare_exchange_weak(
                previousMax,
                durationUs
            )
        ) {
        }

        if (
            durationUs >
            (_cycleTimeMs * 1000UL)
        ) {
            _overrunCount.fetch_add(1);
        }

        vTaskDelayUntil(
            &lastWakeTime,
            cycleTicks
        );
    }
}

void PLCRuntime::enterError() {
    _state.store(RuntimeState::ERROR);

    _outputs.store(0);

    if (!_io.resetOutputs()) {
        Serial.println(
            "[PLC] Output reset failed!"
        );
    }

    Serial.println(
        "[PLC] Runtime entered ERROR state"
    );
}

RuntimeState PLCRuntime::getState() const {
    return _state.load();
}

RuntimeTelemetry PLCRuntime::getTelemetry() const {
    RuntimeTelemetry telemetry;

    telemetry.state = _state.load();

    telemetry.cycleCount = _cycleCount.load();
    telemetry.lastCycleUs = _lastCycleUs.load();
    telemetry.maxCycleUs = _maxCycleUs.load();
    telemetry.overrunCount = _overrunCount.load();

    telemetry.inputs = _inputs.load();
    telemetry.outputs = _outputs.load();

    return telemetry;
}