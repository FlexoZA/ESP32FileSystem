#include "input/InputManager.h"

InputManager::InputManager() : 
    minValue(0),
    maxValue(100),
    currentValue(50),
    lastButtonState(HIGH),
    buttonState(HIGH),
    wasButtonPressed(false),
    allowButtonPress(true),
    lastDebounceTime(0),
    relayState(false)  // Initialize relay state to off
{
}

void InputManager::begin() {
    encoder.attachSingleEdge(ENCODER_DT_PIN, ENCODER_CLK_PIN);
    encoder.clearCount();
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
    pinMode(AD_KEYBOARD_PIN, INPUT);
    pinMode(RELAY, OUTPUT);  // Set relay pin as output
    digitalWrite(RELAY, LOW);  // Initialize relay to OFF
    adKeyValue = -1;
    lastAdKey = -1;
    lastAdKeyDebounceTime = 0;
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

    // Handle ADKeyboard with improved debouncing
    int adReading = analogRead(AD_KEYBOARD_PIN);
    int currentKey = -1;
    
    // Map analog values to buttons
    if (adReading < 100) currentKey = 1;        // Right (Quick Mode)
    else if (adReading < 500) currentKey = 2;   // Up (Next Track)
    else if (adReading < 1500) currentKey = 3;  // Down (Previous Track)
    else if (adReading < 2500) currentKey = 4;  // Left (Play/Pause)
    else if (adReading < 3500) currentKey = 5;  // Select
    
    // Quick mode change handling (Button 1)
    if (currentKey == 1 && lastAdKey != 1) {
        if (quickModeChangeCallback) {
            quickModeChangeCallback();
        }
    }

    // Only update if the reading is different
    if (currentKey != lastAdKey) {
        lastAdKeyDebounceTime = millis();
    }

    if ((millis() - lastAdKeyDebounceTime) > DEBOUNCE_DELAY) {
        // Only register a new press if we're coming from no press
        if (currentKey != -1 && adKeyValue == -1) {
            adKeyValue = currentKey;
            if (adKeyValue == 5) {  // Button 5 pressed
                toggleRelay();  // Toggle the relay
                Serial.print("Relay state changed to: ");
                Serial.println(relayState ? "ON" : "OFF");
            }
            if (adKeyValue != -1) {
                Serial.print("ADKey pressed: ");
                Serial.println(adKeyValue);
            }
        } else if (currentKey == -1) {
            adKeyValue = -1; // Reset when button is released
        }
    }
    
    lastAdKey = currentKey;
}

void InputManager::toggleRelay() {
    relayState = !relayState;  // Toggle the state
    digitalWrite(RELAY, relayState ? HIGH : LOW);  // Set the relay output
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

int InputManager::getADKeyPressed() {
    return adKeyValue;
}

void InputManager::setQuickModeChangeCallback(std::function<void()> callback) {
    quickModeChangeCallback = callback;
}