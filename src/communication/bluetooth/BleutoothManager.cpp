#include "BluetoothManager.h"

// Define UUIDs for the service and characteristics
const char* BluetoothManager::SERVICE_UUID = "00000000-0000-1000-8000-00805f9b34fb";
const char* BluetoothManager::MEDIA_TITLE_CHARACTERISTIC_UUID = "00000001-0000-1000-8000-00805f9b34fb";

void BluetoothManager::begin() {
    // Initialize BLE device
    BLEDevice::init("ESP32 Media Controller");
    
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(*this));

    // Create the BLE Service
    pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristics
    pMediaTitleCharacteristic = pService->createCharacteristic(
        MEDIA_TITLE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    
    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("BLE Media Controller is now advertising!");
}

String BluetoothManager::getCurrentTrack() const {
    if (pMediaTitleCharacteristic) {
        return String((char*)pMediaTitleCharacteristic->getValue().c_str());
    }
    return String();
}

void BluetoothManager::update() {
    // Update blink state if needed
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        blinkState = !blinkState;
        lastBlinkTime = currentTime;
    }
    
    // Add any other periodic updates needed for Bluetooth functionality
}