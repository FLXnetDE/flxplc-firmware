#include <Arduino.h>

#include <IOService.h>
#include <WaveshareIO.h>

#include <PLCRuntime.h>
#include <TestProgram.h>

WaveshareIO hardware;
IOService io(hardware);

TestProgram program;
PLCRuntime runtime(io, program, 20);

void setup() {
    Serial.begin(115200);

    delay(2000);

    Serial.println("flxplc starting...");

    if (!io.begin()) {
        Serial.println("IO initialization failed!");
        return;
    }

    if (!runtime.begin()) {
        Serial.println("Runtime initialization failed!");
        return;
    }

    Serial.println("Runtime initialized.");
    Serial.println("Commands: r=RUN, s=STOP");
}

void loop() {
    if (Serial.available()) {
        char command = Serial.read();

        switch (command) {
            case 'r':
                runtime.start();
                break;

            case 's':
                runtime.stop();
                break;
        }
    }

    static uint32_t lastTelemetry = 0;

    if (millis() - lastTelemetry >= 1000) {
        lastTelemetry = millis();

        RuntimeTelemetry t =
            runtime.getTelemetry();

        Serial.printf(
            "State=%d Cycles=%lu "
            "Last=%lu us Max=%lu us "
            "Overruns=%lu DI=%02X DO=%02X\n",
            static_cast<int>(t.state),
            static_cast<unsigned long>(t.cycleCount),
            static_cast<unsigned long>(t.lastCycleUs),
            static_cast<unsigned long>(t.maxCycleUs),
            static_cast<unsigned long>(t.overrunCount),
            t.inputs,
            t.outputs
        );
    }

    delay(10);
}