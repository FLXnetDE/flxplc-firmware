#include <Arduino.h>

#include <IOService.h>
#include <WaveshareIO.h>

WaveshareIO hardware;
IOService io(hardware);

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("flxplc starting...");

    if (!io.begin()) {
        Serial.println("IO initialization failed!");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("IO initialized.");
}

void loop() {
    if (!io.update()) {
        Serial.println("IO update failed!");
        delay(1000);
        return;
    }

    Serial.print("Digital Inputs: ");

    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(io.getInput(i) ? "1" : "0");
        Serial.print(" ");
    }

    Serial.println();

    delay(500);
}