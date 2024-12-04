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
DisplayManager displayManager(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, bluetoothManager, wifiManager, timeManager, mediaManager);
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
    
    // Reset encoder value to prevent sudden changes when switching modes
    lastEncoderValue = inputManager.getCurrentValue();
}

void handleEncoderChange(int difference) {
    static unsigned long lastEncoderUpdate = 0;
    unsigned long currentTime = millis();
    
    // Add debounce for encoder
    if (currentTime - lastEncoderUpdate < 50) {
        return;
    }
    lastEncoderUpdate = currentTime;

    // Limit the maximum change per update to prevent large jumps
    difference = constrain(difference, -1, 1);

    // Debug print
    Serial.print("Encoder difference (constrained): ");
    Serial.println(difference);

    switch(currentQuickMode) {
        case QuickControlMode::VOLUME:
            {
                // Invert the difference if needed (depending on your encoder direction)
                // difference = -difference;  // Uncomment this line if directions are reversed
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

    // Handle encoder for volume/brightness control
    long encoderDelta = inputManager.getCurrentValue() - lastEncoderValue;
    if (encoderDelta != 0) {
        handleEncoderChange(encoderDelta);
        lastEncoderValue = inputManager.getCurrentValue();
    }

    // Handle other media controls
    if (pressedButton != -1 && pressedButton != 4) {
        switch (pressedButton) {
            case 3: // Previous/Restart Track (Button 1)
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.handlePreviousButton();
                }
                break;
                
            case 2:  // Next Track (Button 2)
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.nextTrack();
                }
                break;
                
            case 1: // Play/Pause (Button 3)
                if (bluetoothManager.isDeviceConnected()) {
                    mediaManager.togglePlayPause();
                }
                break;
        }
    }
}