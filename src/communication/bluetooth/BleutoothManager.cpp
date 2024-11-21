#include "BluetoothManager.h"

BluetoothManager::BluetoothManager() {
    calculateTextLength();
}

void BluetoothManager::begin() {
    // Bluetooth setup will go here later
    Serial.println("Bluetooth Manager Started");
    Serial.print("Current text length: ");
    Serial.println(textLength);
}

String BluetoothManager::getCurrentText() {
    return currentText;
}

int BluetoothManager::getTextLength() {
    return textLength;
}

void BluetoothManager::calculateTextLength() {
    textLength = currentText.length();
}
