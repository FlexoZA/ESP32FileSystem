#include <Arduino.h>
#include <Wire.h>
#include "config/Config.h"
#include "input/InputManager.h"
#include "mode/ModeManager.h"
#include "display/DisplayManager.h"
#include "mode/media/MediaManager.h"
#include "sensor/SensorManager.h"
#include "communication/bluetooth/BluetoothManager.h"

BluetoothManager bluetoothManager;
MediaManager mediaManager;
InputManager inputManager;
ModeManager modeManager;
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, bluetoothManager);
SensorManager sensorManager;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Setup starting...");
    Serial.flush();
    
    Wire.begin();
    bluetoothManager.begin();
    mediaManager.begin();
    inputManager.begin();
    modeManager.begin();
    displayManager.begin();
    sensorManager.begin();
    displayManager.drawDefaultScreen(0.0, 0.0);
}

void loop() {
    inputManager.update();
    mediaManager.update();
    sensorManager.update();
    bluetoothManager.update();

    //Serial.println("Loop running");
    
    modeManager.update(inputManager.getCurrentValue(), inputManager.isButtonPressed());
    displayManager.drawDefaultScreen(
        sensorManager.getTemperature(),
        sensorManager.getHumidity()
    );
}