#include <Arduino.h>
#include <Wire.h>
#include "config/Config.h"
#include "input/InputManager.h"
#include "mode/ModeManager.h"
#include "display/DisplayManager.h"

InputManager inputManager;
ModeManager modeManager;
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    inputManager.begin();
    modeManager.begin();
    displayManager.begin();
    
    displayManager.drawDefaultScreen();
}

void loop() {
    inputManager.update();
    
    modeManager.update(inputManager.getCurrentValue(), inputManager.isButtonPressed());
}