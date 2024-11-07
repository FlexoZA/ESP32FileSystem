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
