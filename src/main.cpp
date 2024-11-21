#include <Arduino.h>
#include <Wire.h>
#include "config/Config.h"
#include "input/InputManager.h"
#include "mode/ModeManager.h"
#include "display/DisplayManager.h"
#include "mode/media/MediaManager.h"

MediaManager mediaManager;
InputManager inputManager;
ModeManager modeManager;
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Setup starting...");
    Serial.flush();
    
    Wire.begin();
    mediaManager.begin();
    inputManager.begin();
    modeManager.begin();
    displayManager.begin();
    
    displayManager.drawDefaultScreen();
}

void loop() {
    inputManager.update();
    mediaManager.update();

    Serial.println("Loop running");
    
    modeManager.update(inputManager.getCurrentValue(), inputManager.isButtonPressed());
    displayManager.drawDefaultScreen();
}