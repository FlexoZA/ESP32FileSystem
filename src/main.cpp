#include <Arduino.h>
#include "input/InputManager.h"
#include "mode/ModeManager.h"

InputManager inputManager;
ModeManager modeManager;

void setup() {
    Serial.begin(9600);
    delay(1000);
    inputManager.begin();
    modeManager.begin();
}

void loop() {
    inputManager.update();
    
    // Update mode manager with current input state
    modeManager.update(inputManager.getCurrentValue(), inputManager.isButtonPressed());
    
    delay(100);
}