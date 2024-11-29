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
#include "mode/led/LEDManager.h"

BluetoothManager bluetoothManager;
MediaManager mediaManager(bluetoothManager);
WifiManager wifiManager;
InputManager inputManager;
ModeManager modeManager;
TimeManager timeManager;
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, bluetoothManager, wifiManager, timeManager);
SensorManager sensorManager;
LEDManager ledManager(LED_PIN, NUM_LEDS);

unsigned long lastTimeUpdate = 0;
const unsigned long TIME_UPDATE_INTERVAL = 1000;
long lastEncoderValue = 0;
QuickControlMode currentQuickMode = QuickControlMode::VOLUME;

void handleQuickModeChange() {
    currentQuickMode = (currentQuickMode == QuickControlMode::VOLUME) ? 
                      QuickControlMode::BRIGHTNESS : 
                      QuickControlMode::VOLUME;
    displayManager.setQuickControlMode(currentQuickMode);
}

void handleEncoderChange(int difference) {
    switch(currentQuickMode) {
        case QuickControlMode::VOLUME:
            {
                mediaManager.adjustVolume(difference);
                displayManager.showProgress(mediaManager.getVolume());
            }
            break;
        case QuickControlMode::BRIGHTNESS:
            {
                int newBrightness = ledManager.getBrightness() + (difference * 5);
                newBrightness = constrain(newBrightness, 0, 255);
                ledManager.setBrightness(newBrightness);
                int brightnessPercent = (newBrightness * 100) / 255;
                displayManager.showProgress(brightnessPercent);
            }
            break;
    }
}

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
    ledManager.begin();
    
    // Initialize time after WiFi is connected
    if (wifiManager.isConnected()) {
        timeManager.begin();
    }
    inputManager.setQuickModeChangeCallback(handleQuickModeChange);
}

void loop() {
    inputManager.update();
    mediaManager.update();
    sensorManager.update();
    bluetoothManager.update();
    wifiManager.update();
    ledManager.update();
    
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

    // Handle encoder changes
    long newValue = inputManager.getCurrentValue();
    if (newValue != lastEncoderValue) {
        int difference = newValue - lastEncoderValue;
        handleEncoderChange(difference);
        lastEncoderValue = newValue;
    }
}