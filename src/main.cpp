#include <Arduino.h>
#include "input/InputManager.h"

InputManager inputManager;

void setup() {
    Serial.begin(9600);
    delay(1000);

    inputManager.begin();
}

void loop() {
    inputManager.update();

    // Example of using the input manager
    Serial.print("Encoder value: ");
    Serial.println(inputManager.getCurrentValue());

    if (inputManager.isButtonPressed()) {
        Serial.println("Button pressed! Current value: " + String(inputManager.getCurrentValue()));
    }

    delay(100);
}
