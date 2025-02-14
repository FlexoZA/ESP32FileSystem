// InputManager.h
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <functional>
#include "config/Config.h"

class InputManager {
private:
    ESP32Encoder encoder;
    int32_t minValue;
    int32_t maxValue;
    int32_t currentValue;
    
    // Enhanced button handling variables
    int lastButtonState;
    int buttonState;
    bool wasButtonPressed;
    bool allowButtonPress;
    unsigned long lastDebounceTime;

    // ADKeyboard variables
    int adKeyValue;
    int lastAdKey;
    unsigned long lastAdKeyDebounceTime;
    bool relayState;  // Track relay state
    
    std::function<void()> quickModeChangeCallback;
    
public:
    InputManager();
    void begin();
    void update();
    int32_t getCurrentValue() const;
    void setLimits(int32_t min, int32_t max);
    bool isButtonPressed();
    bool isButtonReleased();
    int getADKeyPressed();
    void setQuickModeChangeCallback(std::function<void()> callback);
    bool getRelayState() const { return relayState; }
    void toggleRelay();
};

#endif