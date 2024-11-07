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
