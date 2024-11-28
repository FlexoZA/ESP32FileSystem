#include "BluetoothManager.h"
#include <BLE2902.h>

// HID Report Map for consumer control
const uint8_t BluetoothManager::MEDIA_KEYS_REPORT_MAP[] = {
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        // Logical Minimum (0)
    0x25, 0x01,        // Logical Maximum (1)
    0x75, 0x01,        // Report Size (1)
    0x95, 0x01,        // Report Count (1)
    0x09, 0xCD,        // Usage (Play/Pause)
    0x81, 0x06,        // Input (Data,Var,Rel)
    0xC0              // End Collection
};

BluetoothManager::BluetoothManager() : deviceConnected(false) {
    hid = nullptr;
    mediaControl = nullptr;
}

void BluetoothManager::begin() {
    // Initialize BLE Device
    BLEDevice::init("ESP32-Media-Controller");
    
    // Create BLE Server
    pServer = BLEDevice::createServer();
    
    // Create HID Device
    hid = new BLEHIDDevice(pServer);
    if (hid == nullptr) {
        Serial.println("Failed to create HID device!");
        return;
    }
    
    // Set manufacturer name
    hid->manufacturer()->setValue("Espressif");
    
    // Set HID report map
    hid->reportMap((uint8_t*)MEDIA_KEYS_REPORT_MAP, sizeof(MEDIA_KEYS_REPORT_MAP));
    
    // Create media control characteristic
    mediaControl = hid->inputReport(1);
    
    // Start advertising
    pServer->getAdvertising()->start();
    
    Serial.println("Bluetooth HID Device ready");
}

void BluetoothManager::update() {
    // Handle any periodic Bluetooth tasks here
}

void BluetoothManager::playPause() {
    if (!deviceConnected) return;
    
    uint8_t command = 0x01;  // Play/Pause command
    mediaControl->setValue(&command, 1);
    mediaControl->notify();
    
    // Reset the command
    command = 0x00;
    mediaControl->setValue(&command, 1);
    mediaControl->notify();
    
    Serial.println("Play/Pause command sent");
}

// ... implement other media control methods similarly ...
bool BluetoothManager::isDeviceConnected() const {
    return deviceConnected;
}

bool BluetoothManager::getBlinkState() const {
    return blinkState;
}