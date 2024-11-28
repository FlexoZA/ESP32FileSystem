#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>

class BluetoothManager {
private:
    BLEServer* pServer;
    BLEHIDDevice* hid;
    BLECharacteristic* mediaControl;
    bool deviceConnected;
    bool blinkState;
    unsigned long lastBlinkTime;
    
    static const uint8_t MEDIA_KEYS_REPORT_MAP[];
    
public:
    BluetoothManager();
    void begin();
    void update();
    bool isDeviceConnected() const;
    bool getBlinkState() const;
    void playPause();
};

#endif