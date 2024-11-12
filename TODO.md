## Code integration

I have debounce issues on my button press of my encoder. I want to use a method that i used in a previous project. The button press should only work once on press. so i want the button to function exactly like the desired method.

### current InputManager.h
```
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <ESP32Encoder.h>
#include "config/Config.h"

class InputManager {
private:
    ESP32Encoder encoder;
    int32_t minValue;
    int32_t maxValue;
    int32_t currentValue;
    
    // Button debounce variables
    bool lastButtonState;
    bool buttonState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    InputManager();
    void begin();
    void update();
    int32_t getCurrentValue() const;
    void setLimits(int32_t min, int32_t max);
    bool isButtonPressed();
};

#endif
 ```

### current InputManager.cpp

```
#include "input/InputManager.h"

InputManager::InputManager() : 
    minValue(0),
    maxValue(100),
    currentValue(50),
    lastButtonState(HIGH),
    buttonState(HIGH),
    lastDebounceTime(0),
    debounceDelay(50)
{
}

void InputManager::begin() {
    // Initialize encoder
    encoder.attachSingleEdge(ENCODER_DT_PIN, ENCODER_CLK_PIN);
    encoder.clearCount();

    // Set up encoder button pin
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);

    Serial.println("Input Manager initialized.");
}

void InputManager::update() {
    // Read the encoder position
    long position = encoder.getCount();

    // Update the current value based on the encoder position
    if (position > 0) {
        currentValue = min(maxValue, currentValue + 1); // Clockwise (more)
    } else if (position < 0) {
        currentValue = max(minValue, currentValue - 1); // Counterclockwise (less)
    }

    // Enforce the boundary limits
    currentValue = constrain(currentValue, minValue, maxValue);

    // Reset the encoder position
    encoder.clearCount();

    // Handle button press with debounce
    int reading = digitalRead(ENCODER_SW_PIN);

    // If the button state has changed, reset the debounce timer
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    // Check if enough time has passed since the last state change
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // If the button state has changed
        if (reading != buttonState) {
            buttonState = reading;
        }
    }

    // Save the button reading for the next loop
    lastButtonState = reading;
}

int32_t InputManager::getCurrentValue() const {
    return currentValue;
}

void InputManager::setLimits(int32_t min, int32_t max) {
    minValue = min;
    maxValue = max;
    currentValue = constrain(currentValue, minValue, maxValue);
}

bool InputManager::isButtonPressed() {
    return (buttonState == LOW);
}
```
## Current main.cpp
```
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
```
## current config.h
```
#define ENCODER_CLK_PIN 27  // CLK GPIO pin for encoder
#define ENCODER_DT_PIN 14   // DT GPIO pin for encoder
#define ENCODER_SW_PIN 13   // SW GPIO pin for encoder button

// Timing Constants
#define DEBOUNCE_DELAY 50
```


# desired method

## InputManager.cpp
```
#include "config/Config.h"
#include "input/InputManager.h"
#include "led/LEDManager.h"
#include "display/DisplayManager.h"
#include "time/TimeManager.h"
#include "temp/TempManager.h"  // Add this include
#include <WiFi.h>

InputManager inputManager(encoderButton_PIN, encoderDT_PIN, encoderCLK_PIN);
LEDManager ledManager(LED_PIN, NUM_LEDS);
DisplayManager displayManager;
TimeManager timeManager;
TempManager tempManager;  // Add this line

unsigned long lastBrightnessChangeTime = 0;
unsigned long lastModeChangeTime = 0;
unsigned long lastWifiCheckTime = 0;
unsigned long lastTempCheckTime = 0;  // Add this line
const unsigned long WIFI_CHECK_INTERVAL = 5000;
const unsigned long TEMP_CHECK_INTERVAL = 2000;  // Add this line

void setup() {
    Serial.begin(9600);
    
    // Initialize components
    inputManager.begin();
    ledManager.begin();
    displayManager.begin();
    tempManager.begin();  // Add this line
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
    } else {
        Serial.println("WiFi connection failed");
    }
    
    timeManager.begin();
}

void loop() {
    inputManager.update();
    
    // Periodically check WiFi status
    if (millis() - lastWifiCheckTime > WIFI_CHECK_INTERVAL) {
        timeManager.updateLocalTime();
        lastWifiCheckTime = millis();
    }
    
    // Update temperature readings
    if (millis() - lastTempCheckTime > TEMP_CHECK_INTERVAL) {
        tempManager.update();
        lastTempCheckTime = millis();
    }
    
    // Check for encoder button press
    if (inputManager.isEncoderButtonPressed()) {
        ledManager.setMode((ledManager.getMode() + 1) % 2);
        lastModeChangeTime = millis();
    }
    
    // Check for encoder button release
    if (inputManager.isEncoderButtonReleased()) {
        // Do nothing, wait for the next button press
    }
    
    // Check for encoder rotation
    int encoderDelta = inputManager.getEncoderDelta();
    if (encoderDelta != 0) {
        int newBrightness = constrain(ledManager.getBrightness() + encoderDelta, 0, 255);
        ledManager.setBrightness(newBrightness);
        lastBrightnessChangeTime = millis();
    }
    
    // Update components
    ledManager.update();
    displayManager.update(ledManager.getMode(),
                         ledManager.getBrightness(),
                         lastModeChangeTime,
                         lastBrightnessChangeTime,
                         timeManager.getStatusMessage());
}
```
## InputManager.h
```
#pragma once
#include <Arduino.h>
#include <ESP32Encoder.h>  // Include the ESP32Encoder library

class InputManager {
public:
    InputManager(int encoderButtonPin, int encoderDTPin, int encoderCLKPin);
    void begin();
    void update();
    bool isEncoderButtonPressed();
    bool isEncoderButtonReleased();
    int getEncoderDelta();

private:
    int _encoderButtonPin;
    int _encoderDTPin;
    int _encoderCLKPin;
    int _lastEncoderButtonState;
    int _encoderButtonState;
    bool _wasEncoderButtonPressed;
    bool _allowEncoderButtonPress;
    unsigned long _lastDebounceTime;
    ESP32Encoder encoder;  // Encoder object from ESP32Encoder library

    void checkEncoderButton();
    void checkEncoder();
};
```
## main.cpp
```
#include "config/Config.h"
#include "input/InputManager.h"
#include "led/LEDManager.h"
#include "display/DisplayManager.h"
#include "time/TimeManager.h"
#include "temp/TempManager.h"  // Add this include
#include <WiFi.h>

InputManager inputManager(encoderButton_PIN, encoderDT_PIN, encoderCLK_PIN);
LEDManager ledManager(LED_PIN, NUM_LEDS);
DisplayManager displayManager;
TimeManager timeManager;
TempManager tempManager;  // Add this line

unsigned long lastBrightnessChangeTime = 0;
unsigned long lastModeChangeTime = 0;
unsigned long lastWifiCheckTime = 0;
unsigned long lastTempCheckTime = 0;  // Add this line
const unsigned long WIFI_CHECK_INTERVAL = 5000;
const unsigned long TEMP_CHECK_INTERVAL = 2000;  // Add this line

void setup() {
    Serial.begin(9600);
    
    // Initialize components
    inputManager.begin();
    ledManager.begin();
    displayManager.begin();
    tempManager.begin();  // Add this line
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
    } else {
        Serial.println("WiFi connection failed");
    }
    
    timeManager.begin();
}

void loop() {
    inputManager.update();
    
    // Periodically check WiFi status
    if (millis() - lastWifiCheckTime > WIFI_CHECK_INTERVAL) {
        timeManager.updateLocalTime();
        lastWifiCheckTime = millis();
    }
    
    // Update temperature readings
    if (millis() - lastTempCheckTime > TEMP_CHECK_INTERVAL) {
        tempManager.update();
        lastTempCheckTime = millis();
    }
    
    // Check for encoder button press
    if (inputManager.isEncoderButtonPressed()) {
        ledManager.setMode((ledManager.getMode() + 1) % 2);
        lastModeChangeTime = millis();
    }
    
    // Check for encoder button release
    if (inputManager.isEncoderButtonReleased()) {
        // Do nothing, wait for the next button press
    }
    
    // Check for encoder rotation
    int encoderDelta = inputManager.getEncoderDelta();
    if (encoderDelta != 0) {
        int newBrightness = constrain(ledManager.getBrightness() + encoderDelta, 0, 255);
        ledManager.setBrightness(newBrightness);
        lastBrightnessChangeTime = millis();
    }
    
    // Update components
    ledManager.update();
    displayManager.update(ledManager.getMode(),
                         ledManager.getBrightness(),
                         lastModeChangeTime,
                         lastBrightnessChangeTime,
                         timeManager.getStatusMessage());
}
```