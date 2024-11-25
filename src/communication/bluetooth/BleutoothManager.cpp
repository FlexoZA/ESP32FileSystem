#include "BluetoothManager.h"

void BluetoothManager::begin() {
    // Initialize Bluetooth here
    // For now, just set as disconnected
    connected = false;
}

void BluetoothManager::update() {
    // Update connection status here when you implement Bluetooth
    // For now, just handle the blinking logic
    if (!connected) {
        unsigned long currentTime = millis();
        if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
            blinkState = !blinkState;
            lastBlinkTime = currentTime;
        }
    }
}