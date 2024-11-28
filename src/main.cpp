#include <Arduino.h>
#include <Wire.h>
#include "config/Config.h"
#include "input/InputManager.h"
#include "mode/ModeManager.h"
#include "display/DisplayManager.h"
#include "mode/media/MediaManager.h"
#include "sensor/SensorManager.h"
#include "communication/bluetooth/BluetoothManager.h"
#include "communication/wifi/WifiManager.h"
#include "time/TimeManager.h"

BluetoothManager bluetoothManager;
MediaManager mediaManager(bluetoothManager);
WifiManager wifiManager;
InputManager inputManager;
ModeManager modeManager;
TimeManager timeManager;
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, bluetoothManager, wifiManager, timeManager);
SensorManager sensorManager;

unsigned long lastTimeUpdate = 0;
const unsigned long TIME_UPDATE_INTERVAL = 1000;
long lastEncoderValue = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.flush();
    Wire.begin();
    
    bluetoothManager.begin();
    wifiManager.begin();
    mediaManager.begin();
    inputManager.begin();
    modeManager.begin();
    displayManager.begin();
    sensorManager.begin();
    
    // Initialize time after WiFi is connected
    if (wifiManager.isConnected()) {
        timeManager.begin();
    }
}

void loop() {
    inputManager.update();
    mediaManager.update();
    sensorManager.update();
    bluetoothManager.update();
    wifiManager.update();
    
    // Update time periodically
    unsigned long currentMillis = millis();
    if (currentMillis - lastTimeUpdate >= TIME_UPDATE_INTERVAL) {
        if (wifiManager.isConnected()) {
            timeManager.update();
        }
        lastTimeUpdate = currentMillis;
    }
    
    modeManager.update(inputManager.getCurrentValue(), inputManager.isButtonPressed());
    displayManager.drawDefaultScreen(
        sensorManager.getTemperature(),
        sensorManager.getHumidity()
    );
    
    // Handle quick mode change (Button 4)
    int pressedButton = inputManager.getADKeyPressed();
    if (pressedButton == 4) { // Button 4
        modeManager.setMediaMode(!modeManager.isMediaMode());
    }

    // Handle encoder for volume control when in media mode
    if (modeManager.isMediaMode()) {
        long encoderDelta = inputManager.getCurrentValue() - lastEncoderValue;
        if (encoderDelta != 0) {
            mediaManager.adjustVolume(encoderDelta);
            lastEncoderValue = inputManager.getCurrentValue();
        }
    }

    // Handle other media controls
    if (pressedButton != -1 && pressedButton != 4) {
        switch (pressedButton) {
            case 1:  // Play/Pause
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.togglePlayPause();
                }
                break;
                
            case 2:  // Next Track
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.nextTrack();
                }
                break;
                
            case 3:  // Previous/Restart Track
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.handlePreviousButton();
                }
                break;
        }
    }
}