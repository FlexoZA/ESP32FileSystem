#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>

class BluetoothManager {
private:
    bool connected = false;
    unsigned long lastBlinkTime = 0;
    bool blinkState = false;
    static const unsigned long BLINK_INTERVAL = 500; // 500ms blink rate

public:
    BluetoothManager() = default;
    void begin();
    void update();
    bool isConnected() const { return connected; }
    bool getBlinkState() const { return blinkState; }
};

#endif