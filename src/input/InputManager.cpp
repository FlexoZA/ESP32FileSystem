#include "input/InputManager.h"

InputManager::InputManager() : 
    minValue(0),
    maxValue(100),
    currentValue(50),
    lastButtonState(HIGH),
    buttonState(HIGH),
    wasButtonPressed(false),
    allowButtonPress(true),
    lastDebounceTime(0)
{
}

void InputManager::begin() {
    encoder.attachSingleEdge(ENCODER_DT_PIN, ENCODER_CLK_PIN);
    encoder.clearCount();
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
}

void InputManager::update() {
    // Read the encoder position
    long position = encoder.getCount();

    // Update the current value based on the encoder position
    if (position > 0) {
        currentValue = min(maxValue, currentValue + 1);
    } else if (position < 0) {
        currentValue = max(minValue, currentValue - 1);
    }

    // Enforce the boundary limits
    currentValue = constrain(currentValue, minValue, maxValue);

    // Reset the encoder position
    encoder.clearCount();

    // Handle button press with improved debounce
    int reading = digitalRead(ENCODER_SW_PIN);

    // If the button state has changed, reset the debounce timer
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    // Check if enough time has passed since the last state change
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // If the button state has changed
        if (reading != buttonState) {
            buttonState = reading;
            
            // Handle button press
            if (buttonState == LOW && allowButtonPress) {
                wasButtonPressed = true;
                allowButtonPress = false;
            }
            // Handle button release
            else if (buttonState == HIGH) {
                wasButtonPressed = false;
                allowButtonPress = true;
            }
        }
    }

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
    if (wasButtonPressed) {
        wasButtonPressed = false;  // Clear the flag after reading
        return true;
    }
    return false;
}

bool InputManager::isButtonReleased() {
    return (buttonState == HIGH && lastButtonState == HIGH);
}