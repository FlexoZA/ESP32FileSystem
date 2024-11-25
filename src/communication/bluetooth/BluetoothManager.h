#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// Forward declare the callback class
class MyServerCallbacks;

class BluetoothManager {
private:
    friend class MyServerCallbacks;  // Declare friendship
    bool connected = false;
    unsigned long lastBlinkTime = 0;
    bool blinkState = false;
    static const unsigned long BLINK_INTERVAL = 500;

    // Add BLE components
    BLEServer* pServer = nullptr;
    BLEService* pService = nullptr;
    BLECharacteristic* pMediaTitleCharacteristic = nullptr;
    
    // UUID constants
    static const char* SERVICE_UUID;
    static const char* MEDIA_TITLE_CHARACTERISTIC_UUID;

public:
    BluetoothManager() = default;
    void begin();
    void update();
    bool isConnected() const { return connected; }
    bool getBlinkState() const { return blinkState; }
    String getCurrentTrack() const;
};

// Define the callback class in the header
class MyServerCallbacks: public BLEServerCallbacks {
private:
    BluetoothManager& manager;

public:
    MyServerCallbacks(BluetoothManager& mgr) : manager(mgr) {}

    void onConnect(BLEServer* pServer) override {
        manager.connected = true;
    }

    void onDisconnect(BLEServer* pServer) override {
        manager.connected = false;
    }
};

#endif