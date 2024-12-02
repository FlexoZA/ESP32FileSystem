#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>

class BluetoothManager : public BLEServerCallbacks {
private:
    BLEServer* pServer;
    BLEHIDDevice* hid;
    BLECharacteristic* mediaControl;
    bool deviceConnected;
    bool oldDeviceConnected;
    bool blinkState;
    unsigned long lastBlinkTime;
    
    static const uint8_t MEDIA_KEYS_REPORT_MAP[];
    static const uint8_t DEFAULT_VOLUME_PERCENT = 30;  // Adjust this value as needed
    void setInitialVolume();
    void setVolumeToPercent(uint8_t targetPercent);
    
public:
    BluetoothManager();
    void begin();
    void update();
    bool isDeviceConnected() const;
    bool getBlinkState() const;
    void playPause();
    void nextTrack();
    void previousTrack();
    void restartTrack();
    void volumeUp();
    void volumeDown();
    
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
};

#endif